#pragma once
#include "dataHandler.h"
#include <boost/unordered_map.hpp>
#include <boost/container/set.hpp>
namespace Utils
{
	static float* g_deltaTime = (float*)REL::ID(523660).address();                            // 2F6B948
	static float* g_deltaTimeRealTime = (float*)REL::ID(523661).address();                  // 2F6B94C
}



class hitStop
{
public:


	/*Mapping of all actors in hitstop to the remaining time of their hitstop.*/
	boost::unordered_map<RE::Actor*, float> actorsInHitstop;
	/*Mapping of all actors and their left/right hand speed before hitstop.*/
	boost::unordered_map<RE::Actor*, std::pair<float, float>> ogSpeedMap;

	static hitStop* GetSingleton()
	{
		static hitStop singleton;
		return  std::addressof(singleton);
	}

	void update() {
		auto it = actorsInHitstop.begin();
		while (it != actorsInHitstop.end()) {
			auto actor = it->first;
			if (!actor || it->second <= 0) {
				DEBUG("{}'s hitstop ended", actor->GetName());
				it = actorsInHitstop.erase(it);
				switch (settings::currFramework) {
				case dataHandler::combatFrameWork::Vanilla: revertVanilla(actor); break;
				case dataHandler::combatFrameWork::STGM: revertSGTM(); break;
				case dataHandler::combatFrameWork::MCO:	revertMCO(actor); break;
				}
				continue;
			}
			it->second -= *Utils::g_deltaTimeRealTime;
			++it;
		}

	}
	enum STOPTYPE
	{
		objectStop = 0,
		bashStop = 1,
		blockedStop = 2,
		creatureStop = 3
	};

	void calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType);

	void calculateShake(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType);

	inline void stop(float stopTime, float stopSpeed, RE::Actor* a_actor);


private:

	void stopSGTM(int stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopVanilla(int stopTime, float stopSpeed, RE::Actor* a_actor);
	void stopMCO(int stopTime, float stopSpeed, RE::Actor* a_actor);

	void revertSGTM();
	void revertVanilla(RE::Actor* a_actor);
	void revertMCO(RE::Actor* a_actor);

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
	

class MainUpdateHook
{
public:
	static void InstallHook()
	{
		auto& trampoline = SKSE::GetTrampoline();

		REL::Relocation<uintptr_t> hook{ REL::ID(35551) };  // 5AF3D0, main loop

		_Update = trampoline.write_call<5>(hook.address() + 0x11F, Update);
	}

private:
	static void Update(RE::Main* a_this, float a2) {
		hitStop::GetSingleton()->update();
		_Update(a_this, a2);
	}
	static inline REL::Relocation<decltype(Update)> _Update;

};
