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

	enum STOPTYPE
	{
		objectStop = 0,
		bashStop = 1,
		blockedStop = 2,
		creatureStop = 3
	};

	static void calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType);

	inline static void stop(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);

	/*reset amount of thread in the thread pool into THREADCT*/
	static void resetPool(int threadCt) {
		pool.reset(threadCt);
		INFO("reset thread pool's available thread to {}", threadCt);
	}

private:
	static void hitStopSGTMOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopBehaviorOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopVanillaOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	static void hitStopASFOp(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);

	inline static void stopSGTM(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	inline static void stopVanilla(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	inline static void stopMCO(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);
	inline static void stopASF(int stopTimeMiliSec, float stopSpeed, RE::Actor* a_actor);


	inline static bool isActorHitstopping(RE::Actor* a_actor);
	inline static void registerActor(RE::Actor* a_actor);
	inline static void checkOutActor(RE::Actor* a_actor);

	inline static float getStopSpeed(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float stopSpeed;
		using namespace settings::stopSpeed;
		switch (stopType)
		{
		case STOPTYPE::bashStop: stopSpeed = bashStopSpeedPercent; break;
		case STOPTYPE::blockedStop: stopSpeed = blockedStopSpeedPercent; break;
		case STOPTYPE::creatureStop: stopSpeed = creatureStopSpeedPercent; break;
		case STOPTYPE::objectStop: stopSpeed = objectStopSpeedPercent; break;
		default: stopSpeed = 1;
		}
		switch (wpnType)
		{
		case RE::WEAPON_TYPE::kHandToHandMelee: stopSpeed *= stopSpeedHandToHandMult; break;
		case RE::WEAPON_TYPE::kOneHandDagger: stopSpeed *= stopSpeedDaggerMult; break;
		case RE::WEAPON_TYPE::kOneHandSword: stopSpeed *= stopSpeedSwordMult; break;
		case RE::WEAPON_TYPE::kOneHandMace: stopSpeed *= stopSpeedMaceMult; break;
		case RE::WEAPON_TYPE::kOneHandAxe: stopSpeed *= stopSpeedAxeMult; break;
		case RE::WEAPON_TYPE::kTwoHandAxe: stopSpeed *= stopSpeed2hwMult; break;
		case RE::WEAPON_TYPE::kTwoHandSword: stopSpeed *= stopSpeedGreatSwordMult; break;
		}
		if (isPower) {
			stopSpeed *= stopSpeedPowerMult;
		}
		return stopSpeed;
	}

	inline static float getStopTime(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float stopTime;
		using namespace settings::stopTimeMs;
		switch (stopType)
		{
		case STOPTYPE::bashStop: stopTime = bashStopTime; break;
		case STOPTYPE::blockedStop: stopTime = blockedStopTime; break;
		case STOPTYPE::creatureStop: stopTime = creatureStopTime; break;
		case STOPTYPE::objectStop: stopTime = objectStopTime; break;
		default: stopTime = 1;
		}
		switch (wpnType)
		{
		case RE::WEAPON_TYPE::kHandToHandMelee: stopTime *= stopTimeHandToHandMult; break;
		case RE::WEAPON_TYPE::kOneHandDagger: stopTime *= stopTimeDaggerMult; break;
		case RE::WEAPON_TYPE::kOneHandSword: stopTime *= stopTimeSwordMult; break;
		case RE::WEAPON_TYPE::kOneHandMace: stopTime *= stopTimeMaceMult; break;
		case RE::WEAPON_TYPE::kOneHandAxe: stopTime *= stopTimeAxeMult; break;
		case RE::WEAPON_TYPE::kTwoHandAxe: stopTime *= stopTime2hwMult; break;
		case RE::WEAPON_TYPE::kTwoHandSword: stopTime *= stopTimeGreatSwordMult; break;
		}
		if (isPower) {
			stopTime *= stopTimePowerMult;
		}
		return stopTime;
	}

};
	

