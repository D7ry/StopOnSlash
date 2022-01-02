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
	static void stop(long stopTimeMiliSec, float stopSpeed);

private:
	static void hitStopBehaviorOp(RE::BSFixedString animVariable, long stopTimeMiliSec, float stopSpeed);
	static void hitStopAnimSpeedOp(long stopTimeMiliSec, float stopSpeed);
};
	

