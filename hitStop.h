#pragma once
#include "dataHandler.h"
#include "thread_pool.hpp"

inline thread_pool pool; //thread pool for NPC hitstops.

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

	/*reset amount of thread in the thread pool into THREADCT*/
	static void resetPool(int threadCt) {
		pool.reset(threadCt);
		DEBUG("reset thread pool's available thread to {}", threadCt);
	}

private:
	static void stopSGTM(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void stopVanilla(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void stopMCO(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void stopASF(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);


	static void hitStopSGTMOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopBehaviorOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopVanillaOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopASFOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);



	static bool isActorHitstopping(RE::Actor* a_actor);
	static void registerActor(RE::Actor* a_actor);
	static void checkOutActor(RE::Actor* a_actor);
};
	

