#pragma once
#include "hitStop.h"

inline void SGTM(float a_in) {
	static float* g_SGTM = (float*)REL::ID(511883).address();
	*g_SGTM = a_in;
	using func_t = decltype(SGTM);
	REL::Relocation<func_t> func{ REL::ID(66989) };
	return;
}

typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
static REL::Relocation<_shakeCamera> shakeCamera{ REL::ID(32275) };


void hitStop::stopVanilla(int stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop Vanilla!");
	float speedDiff = stopSpeed - a_actor->GetActorValue(RE::ActorValue::kWeaponSpeedMult);
	float speedDiffL = stopSpeed - a_actor->GetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply);
	a_actor->ModActorValue(RE::ActorValue::kWeaponSpeedMult, speedDiff);
	a_actor->ModActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, speedDiffL);
	ogSpeedMap.emplace(a_actor, std::pair(speedDiffL, speedDiff));
}

void hitStop::revertVanilla(RE::Actor* a_actor) {
	DEBUG("revert vanilla");
	auto speedDiffPair = ogSpeedMap.find(a_actor)->second;
	a_actor->ModActorValue(RE::ActorValue::kWeaponSpeedMult, speedDiffPair.second);
	a_actor->ModActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, speedDiffPair.first);
	ogSpeedMap.erase(a_actor);
}

void hitStop::stopMCO(int stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop MCO!");
	a_actor->SetGraphVariableFloat("SkySA_weaponSpeedMult", stopSpeed);
	a_actor->SetGraphVariableFloat("SkySA_dwweaponsSpeedMult", stopSpeed);

}

void hitStop::revertMCO(RE::Actor* a_actor) {
	DEBUG("revert MCO");
	a_actor->SetGraphVariableFloat("SkySA_weaponSpeedMult", 1);
	a_actor->SetGraphVariableFloat("SkySA_dwweaponsSpeedMult", 1);
}

void hitStop::stopSGTM(int stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop SGTM!");
	if (a_actor->HasEffectWithArchetype(RE::MagicTarget::Archetype::kSlowTime)) {
		return;
	}
	SGTM(stopSpeed);
	

}

void hitStop::revertSGTM() {
	DEBUG("revert SGTM");
	SGTM(1);
}

void hitStop::calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType) {
	RE::WEAPON_TYPE wpnType = weapon->GetWeaponType();
	stop(getStopTime(wpnType, stopType, isPowerAtk), getStopSpeed(wpnType, stopType, isPowerAtk), hitter);
	//FIXME: fix this quick workaround
	if (hitter->IsPlayerRef() && settings::pcShake) {
		if (!isPowerAtk) {
			if (!settings::shakeOnLight) {
				return;
			}
		}
		switch (stopType) {
		case STOPTYPE::objectStop:
			if (settings::shakeOnObject) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		case STOPTYPE::bashStop:
			if (settings::shakeOnBash) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		case STOPTYPE::blockedStop:
			if (settings::shakeOnBlocked) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		case STOPTYPE::creatureStop:
			if (settings::shakeOnCreature) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		}
	}


}

void hitStop::calculateShake(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType) {

}

/*call this to start a new hitstop. Also checks if a_actor is in the process of hitstop. 
@param a_actor: the actor who will experience hitstop effect.*/
void hitStop::stop(float stopTime, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("finalizing hitstop. stop time: {}, stop speed: {}, stop actor: {}", stopTime, stopSpeed, a_actor->GetName());
	if (actorsInHitstop.find(a_actor) != actorsInHitstop.end()) { //actor is hit stopping
		DEBUG("actor is already in hitstop!");
		return;
	}
	using method = dataHandler::combatFrameWork;
	switch (settings::currFramework) {
	case method::MCO: stopMCO(stopTime, stopSpeed, a_actor); break;
	case method::Vanilla: stopVanilla(stopTime, stopSpeed, a_actor); break;
	case method::STGM: 
		if (a_actor->IsPlayerRef()) {
			stopSGTM(stopTime, stopSpeed, a_actor);
		}
		break; //SGTM Stop only happens to player. A check is placed in onHitEventHandler to make sure it doesn't happen to NPC.
	}
	actorsInHitstop.emplace(a_actor, stopTime);
}

