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
void hitStop::hitStopBehaviorOp(RE::BSFixedString animVariable, long stopTimeMiliSec, float stopSpeed) {
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		DEBUG("executing stop! by speed {}", stopSpeed);
		pc->SetGraphVariableFloat(animVariable, stopSpeed);
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		pc->SetGraphVariableFloat(animVariable, 1);
	}
}

void hitStop::hitStopSGTMOp(long stopTimeMiliSec, long stopSpeed) {
	SGTM(stopSpeed);
	std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
	SGTM(1);
}


void hitStop::stopMCO(long stopTimeMiliSec, long stopSpeed) {
	DEBUG("stop MCO! stop time: {}, stop speed: {}", stopTimeMiliSec, stopSpeed);
	DEBUG("stop speed in float is{}", (float)stopSpeed);
	float newSpeed = ((float) stopSpeed )/ 100;
	DEBUG("new speed is {}", newSpeed);
	std::jthread hitstopThread = std::jthread(hitStopBehaviorOp, "SkySA_weaponSpeedMult", stopTimeMiliSec, newSpeed);
	hitstopThread.detach();
}

void hitStop::stopVanilla(long stopTimeMiliSec, long stopSpeed) {
	DEBUG("stop vanilla! stop time: {}, stop speed: {}", stopTimeMiliSec, stopSpeed);
	std::jthread hitstopThread = std::jthread(hitStopSGTMOp, stopTimeMiliSec, stopSpeed/100);
	hitstopThread.detach();
}


/*call this to start a new hitstop*/
void hitStop::stop(long stopTimeMiliSec, long stopSpeed) {
	DEBUG(dataHandler::GetSingleton()->currFrameWork);
	switch (dataHandler::GetSingleton()->currFrameWork) {
	case dataHandler::combatFrameWork::MCO: stopMCO(stopTimeMiliSec, stopSpeed); break;
	default: stopVanilla(stopTimeMiliSec, (float) stopSpeed); break;
	}
	
}

