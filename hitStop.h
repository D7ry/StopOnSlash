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
	static void stop(int stopTimeMiliSec, float stopSpeed);

private:
	static void stopVanilla(int stopTimeMiliSec, float stopSpeed);
	static void stopSGTM(int stopTimeMiliSec, float stopSpeed);
	static void stopMCO(int stopTimeMiliSec, float stopSpeed);
	static void stopASF(int stopTimeMiliSec, float stopSpeed);


	static void hitStopBehaviorOp(RE::BSFixedString animVariable, int stopTimeMiliSec, float stopSpeed);
	static void hitStopSGTMOp(int stopTimeMiliSec, float stopSpeed);
	static void hitStopVanillaOp(int stopTimeMiliSec, float stopSpeed);
	static void hitStopASFOp(int stopTimeMiliSec, float stopSpeed);
};
	

