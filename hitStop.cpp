#include "hitStop.h"

inline void SGTM(float a_in) {
	static float* g_SGTM = (float*)REL::ID(511883).address();
	*g_SGTM = a_in;
	using func_t = decltype(SGTM);
	REL::Relocation<func_t> func{ REL::ID(66989) };
	return;
}

/*actual operation in the hitstop.
@param animVariable animation variable related to weapon speed.
@param stopTimeMiliSec how long ni miliseconds is the hitstop.
@param stopSpeed weapon speed during hitstop. Suggested to be lower than 0.2*/
void hitStop::hitStopBehaviorOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	if (a_actor) {
		//DEBUG("executing stop! by speed {}", stopSpeed);
		a_actor->SetGraphVariableFloat("SkySA_weaponSpeedMult", stopSpeed);
		a_actor->SetGraphVariableFloat("SkySA_dwweaponsSpeedMult", stopSpeed);
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		a_actor->SetGraphVariableFloat("SkySA_weaponSpeedMult", 1);
		a_actor->SetGraphVariableFloat("SkySA_dwweaponsSpeedMult", 1);
	}
}

void hitStop::hitStopSGTMOp(int stopTimeMiliSec, float stopSpeed) {
	//DEBUG("executing stop! by speed {}", stopSpeed);
	SGTM(stopSpeed);
	std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
	SGTM(1);
}

void hitStop::hitStopVanillaOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	if (a_actor) {
		//DEBUG("executing stop! by speed {}", stopSpeed);
		float orgSpd = a_actor->GetActorValue(RE::ActorValue::kWeaponSpeedMult);
		float orgSpdL = a_actor->GetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply);
		a_actor->SetActorValue(RE::ActorValue::kWeaponSpeedMult, stopSpeed);
		a_actor->SetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, stopSpeed);
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		a_actor->SetActorValue(RE::ActorValue::kWeaponSpeedMult, orgSpd);
		a_actor->SetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, orgSpdL);
	}
}

/*asf is a bit special. So we instead of directly setting the speed, subtract
the difference between the current and desired speed. */
void hitStop::hitStopASFOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	if (a_actor) {
		//DEBUG("executing stop! by speed {}", stopSpeed);
		float speedDiff = stopSpeed - a_actor->GetActorValue(RE::ActorValue::kWeaponSpeedMult);
		float speedDiffL = stopSpeed - a_actor->GetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply);
		a_actor->ModActorValue(RE::ActorValue::kWeaponSpeedMult, speedDiff);
		a_actor->ModActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, speedDiffL);
		//DEBUG("speed right after execution is {}", a_actor->GetActorValue(RE::ActorValue::kWeaponSpeedMult));
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		a_actor->ModActorValue(RE::ActorValue::kWeaponSpeedMult, -speedDiff);
		a_actor->ModActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, -speedDiff);
	}
}

void hitStop::stopASF(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop ASF!");
	std::jthread hitstopThread = std::jthread(hitStopASFOp, stopTimeMiliSec, stopSpeed, a_actor);
	hitstopThread.detach();
}

void hitStop::stopVanilla(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop Vanilla!");
	std::jthread hitstopThread = std::jthread(hitStopVanillaOp, stopTimeMiliSec, stopSpeed, a_actor);
	hitstopThread.detach();
}

void hitStop::stopMCO(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop MCO!");
	std::jthread hitstopThread = std::jthread(hitStopBehaviorOp, stopTimeMiliSec, stopSpeed, a_actor);
	hitstopThread.detach();
}

void hitStop::stopSGTM(int stopTimeMiliSec, float stopSpeed) {
	DEBUG("stop SGTM!");
	if (RE::PlayerCharacter::GetSingleton()->HasEffectWithArchetype(RE::MagicTarget::Archetype::kSlowTime)) {
		return;
	}
	std::jthread hitstopThread = std::jthread(hitStopSGTMOp, stopTimeMiliSec, stopSpeed);
	hitstopThread.detach();
}


/*call this to start a new hitstop. Also checks if a_actor is in the process of hitstop. 
@param a_actor: the actor who will experience hitstop effect.*/
void hitStop::stop(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("finalizing hitstop. stop time: {}, stop speed: {}, stop actor: {}", stopTimeMiliSec, stopSpeed, a_actor->GetName());
	using method = dataHandler::combatFrameWork;
	switch (settings::currFramework) {
	case method::MCO: stopMCO(stopTimeMiliSec, stopSpeed, a_actor); break;
	case method::ASF: stopASF(stopTimeMiliSec, stopSpeed, a_actor); break;
	case method::Vanilla: stopVanilla(stopTimeMiliSec, stopSpeed, a_actor); break;
	case method::STGM: stopSGTM(stopTimeMiliSec, stopSpeed); break;
	default: stopVanilla(stopTimeMiliSec, stopSpeed, a_actor); break;
	}
	
}

