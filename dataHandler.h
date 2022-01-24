#pragma once
#include "SimpleIni.h"
#include <iostream>
#include <cctype>

class dataHandler
{
public:
	enum combatFrameWork
	{
		Vanilla = 0,
		ASF = 1,
		MCO = 2,
		STGM = 3
	};


	static void readSettings();



private:
	static void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
	static void ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, int& a_setting);
	static void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
};


namespace settings
{
	extern dataHandler::combatFrameWork currFramework;

	extern int activeThreadCt;

	extern bool pcHitStop;

	extern bool npcHitStop;


	extern bool stopOnCreature;

	extern bool stopOnObject;

	extern bool stopOnDead;

	extern bool stopOnBash;

	extern bool stopOnBlocked;

	namespace stopTimeMs
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
}
