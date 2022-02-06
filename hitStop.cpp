#pragma once
#include "hitStop.h"
#include <unordered_set>
#include <mutex>

std::mutex mtx;
/*set of actors experiencing hitstop*/
static inline std::unordered_set<RE::Actor*> hitStoppingActors;

inline void SGTM(float a_in) {
	static float* g_SGTM = (float*)REL::ID(511883).address();
	*g_SGTM = a_in;
	using func_t = decltype(SGTM);
	REL::Relocation<func_t> func{ REL::ID(66989) };
	return;
}

bool hitStop::isActorHitstopping(RE::Actor* a_actor) { //checks if actor is in hitstop
	mtx.lock();
	bool isHitStopping = (hitStoppingActors.find(a_actor) != hitStoppingActors.end());
	mtx.unlock();
	return isHitStopping;
}

void hitStop::registerActor(RE::Actor* a_actor) {
	mtx.lock();
	hitStoppingActors.insert(a_actor);
	mtx.unlock();
}

void hitStop::checkOutActor(RE::Actor* a_actor) {
	mtx.lock();
	hitStoppingActors.erase(a_actor);
	mtx.unlock();
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
		checkOutActor(a_actor);
	}
}

void hitStop::hitStopSGTMOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	//DEBUG("executing stop! by speed {}", stopSpeed);
	SGTM(stopSpeed);
	std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
	SGTM(1);
	checkOutActor(a_actor);
}

void hitStop::hitStopVanillaOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	if (a_actor) {
		//DEBUG("executing stop! by speed {}", stopSpeed);
		float orgSpd = a_actor->GetActorValue(RE::ActorValue::kWeaponSpeedMult);
		float orgSpdL = a_actor->GetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply);
		DEBUG("original speed mult for both hands are: {} and {}", orgSpd, orgSpdL);
		DEBUG("setting speed mult for both hands to {}", stopSpeed);
		a_actor->SetActorValue(RE::ActorValue::kWeaponSpeedMult, stopSpeed);
		a_actor->SetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, stopSpeed);
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		DEBUG("setting speed back to {} and {}", orgSpd, orgSpdL);
		a_actor->SetActorValue(RE::ActorValue::kWeaponSpeedMult, orgSpd);
		a_actor->SetActorValue(RE::ActorValue::kLeftWeaponSpeedMultiply, orgSpdL);
		checkOutActor(a_actor);
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
		checkOutActor(a_actor);
	}
}

void hitStop::stopASF(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop ASF!");
	pool.push_task(hitStopASFOp, stopTimeMiliSec, stopSpeed, a_actor);
}

void hitStop::stopVanilla(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop Vanilla!");
	pool.push_task(hitStopASFOp, stopTimeMiliSec, stopSpeed, a_actor);
}

void hitStop::stopMCO(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop MCO!");
	pool.push_task(hitStopBehaviorOp, stopTimeMiliSec, stopSpeed, a_actor);
}

void hitStop::stopSGTM(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("stop SGTM!");
	if (a_actor->HasEffectWithArchetype(RE::MagicTarget::Archetype::kSlowTime)) {
		return;
	}
	pool.push_task(hitStopSGTMOp, stopTimeMiliSec, stopSpeed, a_actor);
}

void hitStop::calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType) {
	RE::WEAPON_TYPE wpnType = weapon->GetWeaponType();
	stop(getStopTime(wpnType, stopType, isPowerAtk), getStopSpeed(wpnType, stopType, isPowerAtk), hitter);
}

/*call this to start a new hitstop. Also checks if a_actor is in the process of hitstop. 
@param a_actor: the actor who will experience hitstop effect.*/
void hitStop::stop(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor) {
	DEBUG("finalizing hitstop. stop time: {}, stop speed: {}, stop actor: {}", stopTimeMiliSec, stopSpeed, a_actor->GetName());
	if (isActorHitstopping(a_actor)) { //check iff actor is in hitstop already.
		return;
	}
	//DEBUG("current thread available: {}", pool.get_thread_count());
	registerActor(a_actor);
	using method = dataHandler::combatFrameWork;
	switch (settings::currFramework) {
	case method::MCO: stopMCO(stopTimeMiliSec, stopSpeed, a_actor); break;
	case method::ASF: stopASF(stopTimeMiliSec, stopSpeed, a_actor); break;
	case method::Vanilla: stopASF(stopTimeMiliSec, stopSpeed, a_actor); break;
	case method::STGM: stopSGTM(stopTimeMiliSec, stopSpeed, a_actor); break; //SGTM Stop only happens to player. A check is placed in onHitEventHandler to make sure it doesn't happen to NPC.
	default: stopVanilla(stopTimeMiliSec, stopSpeed, a_actor); break;
	}
	
}

