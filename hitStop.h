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
	static void stop(long stopTimeMiliSec, long stopSpeed);

private:
	static void stopVanilla(long stopTimeMiliSec, long stopSpeed);
	static void stopMCO(long stopTimeMiliSec, long stopSpeed);
	static void hitStopBehaviorOp(RE::BSFixedString animVariable, long stopTimeMiliSec, float stopSpeed);
	static void hitStopSGTMOp(long stopTimeMiliSec, long stopSpeed);
};
	

