#pragma once
#include "hitStop.h"
namespace offsets
{
	inline void SGTM(float a_in) {
#if ANNIVERSARY_EDITION
		static float* g_SGTM = (float*)REL::ID(388443).address();
		*g_SGTM = a_in;
		using func_t = decltype(SGTM);
		REL::Relocation<func_t> func{ REL::ID(68246) };
#else
		static float* g_SGTM = (float*)REL::ID(511883).address();
		*g_SGTM = a_in;
		using func_t = decltype(SGTM);
		REL::Relocation<func_t> func{ REL::ID(66989) };
#endif
		return;
	}

	
#if ANNIVERSARY_EDITION
	typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
	static REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(33012) };
#else
	typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
	static REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(32275) };
#endif
};

static inline const RE::BSFixedString GVF_MCOSpeed = "MCO_AttackSpeed";
static inline const RE::BSFixedString GVF_SkysaSpeed = "SkySA_weaponSpeedMult";


inline void revertBehavior(float speedDiff, RE::Actor* a_actor, RE::BSFixedString GVF) {
	float currSpeed;
	if (!a_actor->GetGraphVariableFloat(GVF, currSpeed)) {
		INFO("Error: Failed to get graph variable float");
		return;
	}
	a_actor->SetGraphVariableFloat(GVF, currSpeed + speedDiff);
}
inline void revertMCO(float speedDiffR, RE::Actor* a_actor) {
	revertBehavior(speedDiffR, a_actor, GVF_MCOSpeed);
}
inline void revertDistar(float speedDiffR, RE::Actor* a_actor) {
	revertBehavior(speedDiffR, a_actor, GVF_SkysaSpeed);
}
inline void revertVanilla(float speedDiffL, float speedDiffR, RE::Actor* a_actor) {
	a_actor->ModActorValue(RE::ActorValue::kWeaponSpeedMult, speedDiffR);
	a_actor->ModActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, speedDiffL);
}
inline void revertSGTM() {
	offsets::SGTM(1);
}
/*Asynchronously revert a hitstop's effect, chancing actor's speed back to original.
@param stopTime: (in seconds) how long should the revert function wait before reverting speed.
@param speedDiffL: left hand speed difference between current speed and original speed.
@param speedDiffR: right hand speed difference between current speed and original speed.
@param a_actor: actor whose attack speed will be reverted.*/
void hitStop::asyncRevertFunc(float stopTime, float speedDiffL, float speedDiffR, RE::Actor* a_actor) {
	//DEBUG("waiting for {}", stopTime * 1000);
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(stopTime * 1000)));
	switch (settings::currFramework) {
	case dataHandler::combatFrameWork::MCO: revertMCO(speedDiffR, a_actor); break;
	case dataHandler::combatFrameWork::Skysa2: revertDistar(speedDiffR, a_actor); break;
	case dataHandler::combatFrameWork::Vanilla: revertVanilla(speedDiffL, speedDiffR, a_actor); break;
	case dataHandler::combatFrameWork::SGTM: revertSGTM(); break;
	}
	hitStop::GetSingleton()->hitStoppingActors.erase(a_actor);
}

/*An async fov func.
@param time: total time for the FOV change.
@param FOVStep: change in FOV each tick.*/
void hitStop::asyncFOVFunc(float time, float FOVStep) {
	auto cam = RE::PlayerCamera::GetSingleton();
	float half = time / 2;

	while (time > half) {
		INFO(time);
		cam->worldFOV -= FOVStep;
		cam->firstPersonFOV -= FOVStep;
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
		time -= 0.017;
	}
	while (time > 0) {
		INFO(time);
		cam->worldFOV += FOVStep;
		cam->firstPersonFOV += FOVStep;
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
		time -= 0.017;
	}
	

}

void hitStop::stopVanilla(float stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop Vanilla!");
	float speedDiffR = a_actor->GetActorValue(RE::ActorValue::kWeaponSpeedMult) - stopSpeed;
	float speedDiffL = a_actor->GetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply) - stopSpeed;
	a_actor->ModActorValue(RE::ActorValue::kWeaponSpeedMult, -speedDiffR);
	a_actor->ModActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, -speedDiffL);
	std::jthread t(asyncRevertFunc, stopTime, speedDiffL, speedDiffR, a_actor);
	t.detach();
}

void hitStop::behaviorHS(float stopTime, float stopSpeed, RE::Actor* a_actor, RE::BSFixedString graphVariableFloat) {
	float ogSpeed;
	if (!a_actor->GetGraphVariableFloat(graphVariableFloat, ogSpeed)) {
		INFO("Error: Failed to get graph variable float");
		return;
	}
	float speedDiff = ogSpeed - stopSpeed;
	a_actor->SetGraphVariableFloat(graphVariableFloat, stopSpeed);
	std::jthread t(asyncRevertFunc, stopTime, speedDiff, speedDiff, a_actor);
	t.detach();
}

void hitStop::stopMCO(float stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop MCO!");
	behaviorHS(stopTime, stopSpeed, a_actor, GVF_MCOSpeed);
}

/*Distar is a hippie*/
void hitStop::stopSkysaT(float stopTime, float stopSpeed, RE::Actor* a_actor) {
	behaviorHS(stopTime, stopSpeed, a_actor, GVF_SkysaSpeed);
}


void hitStop::stopSGTM(float stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop SGTM!");
	if (a_actor->HasEffectWithArchetype(RE::MagicTarget::Archetype::kSlowTime)) {
		return;
	}
	offsets::SGTM(stopSpeed);
	std::jthread t(asyncRevertFunc, stopTime, 0, 0, a_actor);
	t.detach();
}

void hitStop::initStopAndShake(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, SOS_HITATTR stopType) {
	RE::WEAPON_TYPE wpnType = weapon->GetWeaponType();
	if (!hitStoppingActors.contains(hitter)) {
		switch (stopType) {
		case SOS_HITATTR::bash:
			if (settings::stopOnBash) {
				stop(isPowerAtk, hitter, stopType, wpnType);
			} break;
		case SOS_HITATTR::blocked:
			if (settings::stopOnBlocked) {
				stop(isPowerAtk, hitter, stopType, wpnType);
			} break;
		case SOS_HITATTR::creature:
			if (settings::stopOnCreature) {
				stop(isPowerAtk, hitter, stopType, wpnType);
			} break;
		case SOS_HITATTR::deadCreature:
			if (settings::stopOnDead) {
				stop(isPowerAtk, hitter, SOS_HITATTR::creature, wpnType);
			} break;
		case SOS_HITATTR::object:
			if (settings::stopOnObject) {
				stop(isPowerAtk, hitter, stopType, wpnType);
			} break;
		}
		hitStoppingActors.insert(hitter);
	}
	if (hitter->IsPlayerRef()) {
		if (settings::pcShake) {
			switch (stopType) {
			case SOS_HITATTR::bash:
				if (settings::shakeOnBash) {
					shake(isPowerAtk, hitter, stopType, wpnType);
				} break;
			case SOS_HITATTR::blocked:
				if (settings::shakeOnBlocked) {
					shake(isPowerAtk, hitter, stopType, wpnType);
				} break;
			case SOS_HITATTR::creature:
				if (settings::shakeOnCreature) {
					shake(isPowerAtk, hitter, stopType, wpnType);
				} break;
			case SOS_HITATTR::deadCreature:
				if (settings::shakeOnDead) {
					stop(isPowerAtk, hitter, SOS_HITATTR::creature, wpnType);
				}
			case SOS_HITATTR::object:
				if (settings::shakeOnObject) {
					shake(isPowerAtk, hitter, stopType, wpnType);
				} break;
			}
		}
		/*if (settings::pcFOV) {
			FOV(isPowerAtk, hitter, stopType, wpnType);
		}*/
	}
}
void hitStop::stop(bool isPowerAtk, RE::Actor* hitter, SOS_HITATTR stopType, RE::WEAPON_TYPE wpnType) {
	
	float stopTime = getStopTime(wpnType, stopType, isPowerAtk);
	float stopSpeed = getStopSpeed(wpnType, stopType, isPowerAtk);
	DEBUG("finalizing hitstop. stop time: {}, stop speed: {}, stop actor: {}", stopTime, stopSpeed, hitter->GetName());
	using method = dataHandler::combatFrameWork;
	switch (settings::currFramework) {
	case method::MCO: stopMCO(stopTime, stopSpeed, hitter); break;
	case method::Vanilla: stopVanilla(stopTime, stopSpeed, hitter); break;
	case method::Skysa2: stopSkysaT(stopTime, stopSpeed, hitter); break;
	case method::SGTM:
		if (hitter->IsPlayerRef()) { //SGTM is player only.
			stopSGTM(stopTime, stopSpeed, hitter);
		}
		break;
	}
	
}

void hitStop::shake(bool isPowerAtk, RE::Actor* hitter, SOS_HITATTR stopType, RE::WEAPON_TYPE wpnType) {
	if (!isPowerAtk) {
		if (!settings::shakeOnLight) {
			return;
		}
	}
	switch (stopType) {
	case SOS_HITATTR::object:
		if (settings::shakeOnObject) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	case SOS_HITATTR::bash:
		if (settings::shakeOnBash) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	case SOS_HITATTR::blocked:
		if (settings::shakeOnBlocked) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	case SOS_HITATTR::creature:
		if (settings::shakeOnCreature) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	}
}


void hitStop::FOV(bool isPowerAtk, RE::Actor* hitter, SOS_HITATTR stopType, RE::WEAPON_TYPE wpnType) {
	std::jthread t(asyncFOVFunc, 0.1, 3);
	t.detach();
}
