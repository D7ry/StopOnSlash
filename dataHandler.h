#pragma once
#include "SimpleIni.h"
#include <iostream>
#include <cctype>

class dataHandler
{
public:
	enum combatFrameWork
	{
		//FIXME: fix MCM implentation
		Vanilla = 0,
		Skysa2 = 1,
		SGTM = 2,
		MCO = 3
	};


	static void readSettings();
	static void initGlobals();
	static void updateGlobals();

private:
	static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
	static void ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, int& a_setting);
	static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
};

class globals {
public:

	static inline RE::TESGlobal* glob_Nemesis_MCO;
	static inline RE::TESGlobal* glob_Nemesis_SkysaT;
	static inline RE::TESGlobal* glob_Nemesis_Vanilla;
};
namespace settings
{
	extern dataHandler::combatFrameWork currFramework;
	extern bool SGTMStop;

	extern int activeThreadCt;

	extern bool pcHitStop;

	extern bool npcHitStop;


	extern bool stopOnCreature;

	extern bool stopOnObject;

	extern bool stopOnDead;

	extern bool stopOnBash;

	extern bool stopOnBlocked;

	extern bool pcShake;
	extern bool shakeOnLight;

	extern bool shakeOnCreature;

	extern bool shakeOnObject;

	extern bool shakeOnDead;

	extern bool shakeOnBash;

	extern bool shakeOnBlocked;

	namespace stopTime
	{
		extern float objectStopTime;
		extern float bashStopTime;
		extern float blockedStopTime;
		extern float creatureStopTime;

		extern float stopTimePowerMult;

		extern float stopTimeHandToHandMult;
		extern float stopTimeDaggerMult;
		extern float stopTimeSwordMult;
		extern float stopTimeGreatSwordMult;
		extern float stopTimeAxeMult;
		extern float stopTimeMaceMult;
		extern float stopTime2hwMult;
	}

	namespace stopSpeed
	{
		extern float objectStopSpeedPercent;
		extern float bashStopSpeedPercent;
		extern float blockedStopSpeedPercent;
		extern float creatureStopSpeedPercent;

		extern float stopSpeedPowerMult;

		extern float stopSpeedHandToHandMult;
		extern float stopSpeedDaggerMult;
		extern float stopSpeedSwordMult;
		extern float stopSpeedGreatSwordMult;
		extern float stopSpeedAxeMult;
		extern float stopSpeedMaceMult;
		extern float stopSpeed2hwMult;
	}

	namespace shakeTime
	{
		extern float objectShakeTime;
		extern float bashShakeTime;
		extern float blockedShakeTime;
		extern float creatureShakeTime;

		extern float ShakeTimePowerMult;

		extern float ShakeTimeHandToHandMult;
		extern float ShakeTimeDaggerMult;
		extern float ShakeTimeSwordMult;
		extern float ShakeTimeGreatSwordMult;
		extern float ShakeTimeAxeMult;
		extern float ShakeTimeMaceMult;
		extern float ShakeTime2hwMult;
	}

	namespace shakeMagnitude
	{
		extern float objectShakeMagnitude;
		extern float bashShakeMagnitude;
		extern float blockedShakeMagnitude;
		extern float creatureShakeMagnitude;

		extern float ShakeMagnitudePowerMult;

		extern float ShakeMagnitudeHandToHandMult;
		extern float ShakeMagnitudeDaggerMult;
		extern float ShakeMagnitudeSwordMult;
		extern float ShakeMagnitudeGreatSwordMult;
		extern float ShakeMagnitudeAxeMult;
		extern float ShakeMagnitudeMaceMult;
		extern float ShakeMagnitude2hwMult;
	}
}
