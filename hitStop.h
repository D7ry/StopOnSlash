#pragma once
#include "dataHandler.h"
#include "lib/robin_hood.h"

class hitStop
{
public:

	robin_hood::unordered_set <RE::Actor*> hitStoppingActors;
	
	static hitStop* GetSingleton()
	{
		static hitStop singleton;
		return  std::addressof(singleton);
	}

	enum STOPTYPE
	{
		objectStop = 0,
		bashStop = 1,
		blockedStop = 2,
		creatureStop = 3
	};
	
	/*Initialize a hitstop.*/
	void initStopAndShake(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType);

	void stop(bool isPowerAtk, RE::Actor* hitter, STOPTYPE stopType, RE::WEAPON_TYPE wpnType);

	void shake(bool isPowerAtk, RE::Actor* hitter, STOPTYPE stopType, RE::WEAPON_TYPE wpnType);



private:
	static void asyncRevertFunc(int stopTime, float speedDiffL, float speedDiffR, RE::Actor* a_actor);
	inline void behaviorHS(int stopTime, float stopSpeed, RE::Actor* a_actor, RE::BSFixedString graphVariableFloat);
	void stopSGTM(int stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopVanilla(int stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopMCO(int stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopDistar(int stopTime, float stopSpeed, RE::Actor* a_actor);

	inline float getStopSpeed(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
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

	inline float getStopTime(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float stopTime;
		using namespace settings::stopTime;
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

	inline float getShakeTime(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float shakeTime;
		using namespace settings::shakeTime;
		switch (stopType)
		{
		case STOPTYPE::bashStop: shakeTime = bashShakeTime; break;
		case STOPTYPE::blockedStop: shakeTime = blockedShakeTime; break;
		case STOPTYPE::creatureStop: shakeTime = creatureShakeTime; break;
		case STOPTYPE::objectStop: shakeTime = objectShakeTime; break;
		default: shakeTime = 0;
		}
		switch (wpnType)
		{
		case RE::WEAPON_TYPE::kHandToHandMelee: shakeTime *= ShakeTimeHandToHandMult; break;
		case RE::WEAPON_TYPE::kOneHandDagger: shakeTime *= ShakeTimeDaggerMult; break;
		case RE::WEAPON_TYPE::kOneHandSword: shakeTime *= ShakeTimeSwordMult; break;
		case RE::WEAPON_TYPE::kOneHandMace: shakeTime *= ShakeTimeMaceMult; break;
		case RE::WEAPON_TYPE::kOneHandAxe: shakeTime *= ShakeTimeAxeMult; break;
		case RE::WEAPON_TYPE::kTwoHandAxe: shakeTime *= ShakeTime2hwMult; break;
		case RE::WEAPON_TYPE::kTwoHandSword: shakeTime *= ShakeTimeGreatSwordMult; break;
		}
		if (isPower) {
			shakeTime *= ShakeTimePowerMult;
		}
		return shakeTime;
	}

	inline float getShakeMagnitude(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float shakeMag;
		using namespace settings::shakeMagnitude;
		switch (stopType) {
		case STOPTYPE::bashStop: shakeMag = bashShakeMagnitude; break;
		case STOPTYPE::blockedStop: shakeMag = blockedShakeMagnitude; break;
		case STOPTYPE::creatureStop: shakeMag = creatureShakeMagnitude; break;
		case STOPTYPE::objectStop: shakeMag = objectShakeMagnitude; break;
		default: shakeMag = 0;
		}
		switch (wpnType) {
		case RE::WEAPON_TYPE::kHandToHandMelee: shakeMag *= ShakeMagnitudeHandToHandMult; break;
		case RE::WEAPON_TYPE::kOneHandDagger: shakeMag *= ShakeMagnitudeDaggerMult; break;
		case RE::WEAPON_TYPE::kOneHandSword: shakeMag *= ShakeMagnitudeSwordMult; break;
		case RE::WEAPON_TYPE::kOneHandMace: shakeMag *= ShakeMagnitudeMaceMult; break;
		case RE::WEAPON_TYPE::kOneHandAxe: shakeMag *= ShakeMagnitudeAxeMult; break;
		case RE::WEAPON_TYPE::kTwoHandAxe: shakeMag *= ShakeMagnitude2hwMult; break;
		case RE::WEAPON_TYPE::kTwoHandSword: shakeMag *= ShakeMagnitudeGreatSwordMult; break;
		}
		if (isPower) {
			shakeMag *= ShakeMagnitudePowerMult;
		}
		return shakeMag;
	}

};
	