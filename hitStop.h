#pragma once
#include "dataHandler.h"

namespace Utils
{
	/*
	void SGTM(float a_out) {
		static float* g_SGTM = (float*)REL::ID(511883).address();
		*g_SGTM = a_out;
	}*/
}

class hitStop
{
public:
	static void stop(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);

private:
	static void stopSGTM(int stopTimeMiliSec, float stopSpeed);
	static void stopVanilla(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void stopMCO(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void stopASF(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);


	static void hitStopSGTMOp(int stopTimeMiliSec, float stopSpeed);
	static void hitStopBehaviorOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopVanillaOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopASFOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);

	/*map containing all actors that are experiencing hitstop */
	//static std::map<RE::Actor, bool> hitstopMap;
};
	

