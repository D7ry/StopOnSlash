#include "hitStop.h"
static inline bool hitStopping;

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
void hitStop::hitStopBehaviorOp(RE::BSFixedString animVariable, int stopTimeMiliSec, float stopSpeed) {
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		DEBUG("executing stop! by speed {}", stopSpeed);
		pc->SetGraphVariableFloat(animVariable, stopSpeed);
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		pc->SetGraphVariableFloat(animVariable, 1);
	}
}

void hitStop::hitStopSGTMOp(int stopTimeMiliSec, float stopSpeed) {
	SGTM(stopSpeed);
	std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
	SGTM(1);
}

void hitStop::hitStopVanillaOp(int stopTimeMiliSec, float stopSpeed) {
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc && !hitStopping) {
		DEBUG("executing stop! by speed {}", stopSpeed);
		float orgSpd = pc->GetActorValue(RE::ActorValue::kWeaponSpeedMult);
		pc->SetActorValue(RE::ActorValue::kWeaponSpeedMult, stopSpeed);
		hitStopping = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		pc->SetActorValue(RE::ActorValue::kWeaponSpeedMult, orgSpd);
		hitStopping = false;
	}
}

/*asf is a bit special. So we instead of directly setting the speed, subtract
the difference between the current and desired speed. */
void hitStop::hitStopASFOp(int stopTimeMiliSec, float stopSpeed) {
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc && !hitStopping) {
		DEBUG("executing stop! by speed {}", stopSpeed);
		float speedDiff = stopSpeed - pc->GetActorValue(RE::ActorValue::kWeaponSpeedMult);
		DEBUG("current speed is {}", pc->GetActorValue(RE::ActorValue::kWeaponSpeedMult));
		pc->ModActorValue(RE::ActorValue::kWeaponSpeedMult, speedDiff);
		DEBUG("speed right after execution is {}", pc->GetActorValue(RE::ActorValue::kWeaponSpeedMult));
		hitStopping = true;
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		pc->ModActorValue(RE::ActorValue::kWeaponSpeedMult, -speedDiff);
		hitStopping = false;
	}
}

void hitStop::stopASF(int stopTimeMiliSec, float stopSpeed) {
	DEBUG("stop ASF!");
	std::jthread hitstopThread = std::jthread(hitStopASFOp, stopTimeMiliSec, stopSpeed / 100);
	hitstopThread.detach();
}

void hitStop::stopVanilla(int stopTimeMiliSec, float stopSpeed) {
	DEBUG("stop Vanilla!");
	std::jthread hitstopThread = std::jthread(hitStopVanillaOp, stopTimeMiliSec, stopSpeed / 100);
	hitstopThread.detach();
}

void hitStop::stopMCO(int stopTimeMiliSec, float stopSpeed) {
	DEBUG("stop MCO!");
	std::jthread hitstopThread = std::jthread(hitStopBehaviorOp, "SkySA_weaponSpeedMult", stopTimeMiliSec, stopSpeed / 100);
	hitstopThread.detach();
}

void hitStop::stopSGTM(int stopTimeMiliSec, float stopSpeed) {
	DEBUG("stop SGTM!");
	std::jthread hitstopThread = std::jthread(hitStopSGTMOp, stopTimeMiliSec, stopSpeed/100);
	hitstopThread.detach();
}


/*call this to start a new hitstop*/
void hitStop::stop(int stopTimeMiliSec, float stopSpeed) {
	using method = dataHandler::combatFrameWork;
	switch (dataHandler::GetSingleton()->currFrameWork) {
	case method::MCO: stopMCO(stopTimeMiliSec, stopSpeed); break;
	case method::ASF: stopASF(stopTimeMiliSec, stopSpeed); break;
	case method::Vanilla: stopVanilla(stopTimeMiliSec, stopSpeed); break;
	case method::STGM: stopSGTM(stopTimeMiliSec, stopSpeed); break;
	default: stopVanilla(stopTimeMiliSec, (float) stopSpeed); break;
	}
	
}

