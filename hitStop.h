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

	enum SOS_HITATTR
	{
		object = 0,
		bash = 1,
		blocked = 2,
		creature = 3,
		deadCreature = 4
	};
	
	/*Initialize a hitstop/screen shake*/
	void initStopAndShake(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, SOS_HITATTR stopType);

	void stop(bool isPowerAtk, RE::Actor* hitter, SOS_HITATTR stopType, RE::WEAPON_TYPE wpnType);

	void shake(bool isPowerAtk, RE::Actor* hitter, SOS_HITATTR stopType, RE::WEAPON_TYPE wpnType);

	void FOV(bool isPowerAtk, RE::Actor* hitter, SOS_HITATTR stopType, RE::WEAPON_TYPE wpnType);

private:
	static void asyncRevertFunc(float stopTime, float speedDiffL, float speedDiffR, RE::Actor* a_actor);
	static void asyncFOVFunc(float time, float magnitude);
	inline void behaviorHS(float stopTime, float stopSpeed, RE::Actor* a_actor, RE::BSFixedString graphVariableFloat);
	void stopSGTM(float stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopVanilla(float stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopMCO(float stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopSkysaT(float stopTime, float stopSpeed, RE::Actor* a_actor);

	inline float getStopSpeed(RE::WEAPON_TYPE wpnType, SOS_HITATTR stopType, bool isPower) {
		float stopSpeed;
		using namespace settings::stopSpeed;
		switch (stopType)
		{
		case SOS_HITATTR::bash: stopSpeed = bashStopSpeedPercent; break;
		case SOS_HITATTR::blocked: stopSpeed = blockedStopSpeedPercent; break;
		case SOS_HITATTR::creature: stopSpeed = creatureStopSpeedPercent; break;
		case SOS_HITATTR::object: stopSpeed = objectStopSpeedPercent; break;
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

	inline float getStopTime(RE::WEAPON_TYPE wpnType, SOS_HITATTR stopType, bool isPower) {
		float stopTime;
		using namespace settings::stopTime;
		switch (stopType)
		{
		case SOS_HITATTR::bash: stopTime = bashStopTime; break;
		case SOS_HITATTR::blocked: stopTime = blockedStopTime; break;
		case SOS_HITATTR::creature: stopTime = creatureStopTime; break;
		case SOS_HITATTR::object: stopTime = objectStopTime; break;
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

	inline float getShakeTime(RE::WEAPON_TYPE wpnType, SOS_HITATTR stopType, bool isPower) {
		float shakeTime;
		using namespace settings::shakeTime;
		switch (stopType)
		{
		case SOS_HITATTR::bash: shakeTime = bashShakeTime; break;
		case SOS_HITATTR::blocked: shakeTime = blockedShakeTime; break;
		case SOS_HITATTR::creature: shakeTime = creatureShakeTime; break;
		case SOS_HITATTR::object: shakeTime = objectShakeTime; break;
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

	inline float getShakeMagnitude(RE::WEAPON_TYPE wpnType, SOS_HITATTR stopType, bool isPower) {
		float shakeMag;
		using namespace settings::shakeMagnitude;
		switch (stopType) {
		case SOS_HITATTR::bash: shakeMag = bashShakeMagnitude; break;
		case SOS_HITATTR::blocked: shakeMag = blockedShakeMagnitude; break;
		case SOS_HITATTR::creature: shakeMag = creatureShakeMagnitude; break;
		case SOS_HITATTR::object: shakeMag = objectShakeMagnitude; break;
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
	