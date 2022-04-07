#pragma once
#include "hitStop.h"
static const RE::BSFixedString GVF_MCOSpeed = "MCO_AttackSpeed";
static const RE::BSFixedString GVF_SkysaSpeed = "SkySA_weaponSpeedMult";

namespace offsets
{
	inline void SGTM(float a_in) {
		static float* g_SGTM = (float*)REL::ID(511883).address();
		*g_SGTM = a_in;
		using func_t = decltype(SGTM);
		REL::Relocation<func_t> func{ REL::ID(66989) };
		return;
	}

	typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
	static REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(32275) };

};
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
void hitStop::asyncRevertFunc(int stopTime, float speedDiffL, float speedDiffR, RE::Actor* a_actor) {
	DEBUG("waiting for {}", stopTime * 1000);
	std::this_thread::sleep_for(std::chrono::milliseconds(stopTime * 1000));
	switch (settings::currFramework) {
	case dataHandler::combatFrameWork::MCO: revertMCO(speedDiffR, a_actor); break;
	case dataHandler::combatFrameWork::Skysa2: revertDistar(speedDiffR, a_actor); break;
	case dataHandler::combatFrameWork::Vanilla: revertVanilla(speedDiffL, speedDiffR, a_actor); break;
	case dataHandler::combatFrameWork::SGTM: revertSGTM(); break;
	}
	hitStop::GetSingleton()->hitStoppingActors.erase(a_actor);
}

void hitStop::stopVanilla(int stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop Vanilla!");
	float speedDiffR = a_actor->GetActorValue(RE::ActorValue::kWeaponSpeedMult) - stopSpeed;
	float speedDiffL = a_actor->GetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply) - stopSpeed;
	a_actor->ModActorValue(RE::ActorValue::kWeaponSpeedMult, -speedDiffR);
	a_actor->ModActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, -speedDiffL);
	std::jthread t(asyncRevertFunc, stopTime, speedDiffL, speedDiffR, a_actor);
	t.detach();
}

void hitStop::behaviorHS(int stopTime, float stopSpeed, RE::Actor* a_actor, RE::BSFixedString graphVariableFloat) {
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

void hitStop::stopMCO(int stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop MCO!");
	behaviorHS(stopTime, stopSpeed, a_actor, GVF_MCOSpeed);
}

/*Distar is a hippie*/
void hitStop::stopDistar(int stopTime, float stopSpeed, RE::Actor* a_actor) {
	behaviorHS(stopTime, stopSpeed, a_actor, GVF_SkysaSpeed);
}


void hitStop::stopSGTM(int stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop SGTM!");
	if (a_actor->HasEffectWithArchetype(RE::MagicTarget::Archetype::kSlowTime)) {
		return;
	}
	offsets::SGTM(stopSpeed);
	std::jthread t(asyncRevertFunc, stopTime, 0, 0, a_actor);
	t.detach();
}

void hitStop::initStopAndShake(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType) {
	RE::WEAPON_TYPE wpnType = weapon->GetWeaponType();
	if (!hitStoppingActors.contains(hitter)) {
		stop(isPowerAtk, hitter, stopType, wpnType);
		hitStoppingActors.insert(hitter);
	}
	if (hitter->IsPlayerRef() && settings::pcShake) {
		shake(isPowerAtk, hitter, stopType, wpnType);
	}
}
void hitStop::stop(bool isPowerAtk, RE::Actor* hitter, STOPTYPE stopType, RE::WEAPON_TYPE wpnType) {
	
	float stopTime = getStopTime(wpnType, stopType, isPowerAtk);
	float stopSpeed = getStopSpeed(wpnType, stopType, isPowerAtk);
	DEBUG("finalizing hitstop. stop time: {}, stop speed: {}, stop actor: {}", stopTime, stopSpeed, hitter->GetName());
	using method = dataHandler::combatFrameWork;
	switch (settings::currFramework) {
	case method::MCO: stopMCO(stopTime, stopSpeed, hitter); break;
	case method::Vanilla: stopVanilla(stopTime, stopSpeed, hitter); break;
	case method::Skysa2: stopDistar(stopTime, stopSpeed, hitter); break;
	case method::SGTM:
		if (hitter->IsPlayerRef()) {
			stopSGTM(stopTime, stopSpeed, hitter);
		}
		break; //SGTM Stop only happens to player. A check is placed in onHitEventHandler to make sure it doesn't happen to NPC.
	}
	
}

void hitStop::shake(bool isPowerAtk, RE::Actor* hitter, STOPTYPE stopType, RE::WEAPON_TYPE wpnType) {
	if (!isPowerAtk) {
		if (!settings::shakeOnLight) {
			return;
		}
	}
	switch (stopType) {
	case STOPTYPE::objectStop:
		if (settings::shakeOnObject) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	case STOPTYPE::bashStop:
		if (settings::shakeOnBash) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	case STOPTYPE::blockedStop:
		if (settings::shakeOnBlocked) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	case STOPTYPE::creatureStop:
		if (settings::shakeOnCreature) {
			offsets::shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
		}
	}
}


