#include "hitStop.h"

/*actual operation in the hitstop.
@param animVariable animation variable related to weapon speed.
@param stopTimeMiliSec how long ni miliseconds is the hitstop.
@param stopSpeed weapon speed during hitstop. Suggested to be lower than 0.2*/
void hitStop::hitStopBehaviorOp(RE::BSFixedString animVariable, long stopTimeMiliSec, float stopSpeed) {
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		pc->SetGraphVariableFloat(animVariable, stopSpeed);
		std::this_thread::sleep_for(std::chrono::milliseconds(stopTimeMiliSec));
		pc->SetGraphVariableFloat(animVariable, 1);
	}
}



/*call this to start a new hitstop*/
void hitStop::stop(long stopTimeMiliSec, float stopSpeed) {
	RE::BSFixedString animVariable;
	switch (dataHandler::GetSingleton()->currFrameWork) {
	case 0: animVariable = "SkySA_weaponSpeedMult"; break;
	case 1: animVariable = "SkySA_weaponSpeedMult"; break;
	case 2: animVariable = "SkySA_weaponSpeedMult"; break;
	case 3: animVariable = "SkySA_weaponSpeedMult"; break;
	default: animVariable = "SkySA_weaponSpeedMult"; break;
	}
	std::jthread hitstopThread = std::jthread(hitStopBehaviorOp, animVariable, stopTimeMiliSec, stopSpeed);
	hitstopThread.detach();
}