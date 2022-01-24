#include "dataHandler.h"
#include "hitStop.h"
#pragma region settingReader
void dataHandler::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound)
	{
		INFO("found {} with value {}", a_settingName, bFound);
		a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
	}
	else {
		INFO("failed to find {} from {}", a_settingName, a_sectionName);
	}
}

void dataHandler::ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, int& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		INFO("found {} with value {}", a_settingName, bFound);
		a_setting = static_cast<int>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
	else {
		INFO("failed to find {} from {}", a_settingName, a_sectionName);
	}
}

void dataHandler::ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		INFO("found {} with value {}", a_settingName, bFound);
		a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
	else {
		INFO("failed to find {} from {}", a_settingName, a_sectionName);
	}
}
#pragma endregion

namespace settings {
	dataHandler::combatFrameWork currFramework = dataHandler::combatFrameWork::Vanilla;

	int activeThreadCt = 4;

	bool pcHitStop = true;

	bool npcHitStop = true;

	bool stopOnCreature = true;

	bool stopOnObject = true;

	bool stopOnDead = true;

	bool stopOnBash = true;

	bool stopOnBlocked = true;

	namespace stopTimeMs
	{
		float objectStopTime = 100;
		float bashStopTime = 100;
		float blockedStopTime = 100;
		float creatureStopTime = 100;

		float stopTimePowerMult = 1;

		float stopTimeHandToHandMult = 1;
		float stopTimeDaggerMult = 1;
		float stopTimeSwordMult = 1;
		float stopTimeGreatSwordMult = 1;
		float stopTimeAxeMult = 1;
		float stopTimeMaceMult = 1;
		float stopTime2hwMult = 1;
	}

	namespace stopSpeed
	{
		float objectStopSpeedPercent = 0.1;
		float bashStopSpeedPercent = 0.1;
		float blockedStopSpeedPercent = 0.1;
		float creatureStopSpeedPercent = 0.1;

		float stopSpeedPowerMult = 1;

		float stopSpeedHandToHandMult = 1;
		float stopSpeedDaggerMult = 1;
		float stopSpeedSwordMult = 1;
		float stopSpeedGreatSwordMult = 1;
		float stopSpeedAxeMult = 1;
		float stopSpeedMaceMult = 1;
		float stopSpeed2hwMult = 1;
	}
}


void dataHandler::readSettings() {
	DEBUG("getting settings!");
	CSimpleIniA ini;
#define SETTINGFILE_PATH "Data\\MCM\\Settings\\StopOnSlash.ini"
	ini.LoadFile(SETTINGFILE_PATH);

	int frameworkInt;
	ReadIntSetting(ini, "General", "iFrameWork", frameworkInt);

	using namespace settings;
	INFO("frameworkInt set to {}", frameworkInt);
	switch (frameworkInt) {
	case 0: currFramework = combatFrameWork::Vanilla; DEBUG("using vanilla framework!"); break;
	case 1: currFramework = combatFrameWork::ASF; DEBUG("using ASF framework!"); break;
	case 2: currFramework = combatFrameWork::MCO; DEBUG("using MCO framework!"); break;
	case 3: currFramework = combatFrameWork::STGM; DEBUG("using global time framework!"); break;
	default: currFramework = combatFrameWork::Vanilla; DEBUG("invalid framework setting. Using Skysa framework."); break;
	}
	ReadBoolSetting(ini, "General", "bPChitStop", pcHitStop);
	ReadBoolSetting(ini, "General", "bNPChitStop", npcHitStop);
	ReadBoolSetting(ini, "General", "bStopOnCreature", stopOnCreature);
	ReadBoolSetting(ini, "General", "bStopOnDead", stopOnDead);
	ReadBoolSetting(ini, "General", "bStopOnObject", stopOnObject);
	ReadBoolSetting(ini, "General", "bStopOnBash", stopOnBash);
	ReadBoolSetting(ini, "General", "bStopOnBlocked", stopOnBlocked);

	ReadIntSetting(ini, "General", "iConcurrentHitStopCt", activeThreadCt);

	//iff there's no NPC stop/framework is set to global time mult, no need for more than one thread.
	if (!npcHitStop || currFramework == combatFrameWork::STGM) {
		DEBUG("forcing active thread count to 1");
		hitStop::resetPool(1);
	}
	hitStop::resetPool(activeThreadCt);

	using namespace stopTimeMs;
	ReadFloatSetting(ini, "StopTime", "fstopOnBashMs", bashStopTime);
	ReadFloatSetting(ini, "StopTime", "fstopOnObjectMs", objectStopTime);
	ReadFloatSetting(ini, "StopTime", "fstopOnBlockedMs", blockedStopTime);
	ReadFloatSetting(ini, "StopTime", "fstopOnCreatureMs", creatureStopTime);

	ReadFloatSetting(ini, "Multiplier", "fstopTimePowerMult", stopTimePowerMult);

	ReadFloatSetting(ini, "Multiplier", "fstopTimeHandToHandMult", stopTimeHandToHandMult);
	ReadFloatSetting(ini, "Multiplier", "fstopTimeDaggerMult", stopTimeDaggerMult);
	ReadFloatSetting(ini, "Multiplier", "fstopTimeSwordMult", stopTimeSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fstopTimeGreatSwordMult", stopTimeGreatSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fstopTimeAxeMult", stopTimeAxeMult);
	ReadFloatSetting(ini, "Multiplier", "fstopTimeMaceMult", stopTimeMaceMult);
	ReadFloatSetting(ini, "Multiplier", "fstopTime2hwMult", stopTime2hwMult);


	using namespace stopSpeed;
	ReadFloatSetting(ini, "StopSpeed", "fstopOnBashSpd", bashStopSpeedPercent);
	ReadFloatSetting(ini, "StopSpeed", "fstopOnObjectSpd", objectStopSpeedPercent);
	ReadFloatSetting(ini, "StopSpeed", "fstopOnBlockedSpd", blockedStopSpeedPercent);
	ReadFloatSetting(ini, "StopSpeed", "fstopOnCreatureSpd", creatureStopSpeedPercent);

	ReadFloatSetting(ini, "Multiplier", "fstopSpeedPowerMult", stopSpeedPowerMult);

	ReadFloatSetting(ini, "Multiplier", "fstopSpeedHandToHandMult", stopSpeedHandToHandMult);
	ReadFloatSetting(ini, "Multiplier", "fstopSpeedDaggerMult", stopSpeedDaggerMult);
	ReadFloatSetting(ini, "Multiplier", "fstopSpeedSwordMult", stopSpeedSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fstopSpeedGreatSwordMult", stopSpeedGreatSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fstopSpeedAxeMult", stopSpeedAxeMult);
	ReadFloatSetting(ini, "Multiplier", "fstopSpeedMaceMult", stopSpeedMaceMult);
	ReadFloatSetting(ini, "Multiplier", "fstopSpeed2hwMult", stopSpeed2hwMult);
}

