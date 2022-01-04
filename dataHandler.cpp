#include "dataHandler.h"
#include "stopHandler.h"
void dataHandler::readSettings() {
	DEBUG("getting settings!");
	CSimpleIniA ini;
#define SETTINGFILE_PATH "Data\\MCM\\Settings\\StopOnSlash.ini"
	ini.LoadFile(SETTINGFILE_PATH);

	int frameworkInt;
	ReadIntSetting(ini, "General", "iFrameWork", frameworkInt);
	INFO("frameworkInt set to {}", frameworkInt);
	switch (frameworkInt) {
	case 0: currFrameWork = combatFrameWork::Vanilla; DEBUG("using vanilla framework!"); break;
	case 1: currFrameWork = combatFrameWork::ASF; DEBUG("using ASF framework!"); break;
	case 2: currFrameWork = combatFrameWork::MCO; DEBUG("using MCO framework!"); break;
	case 3: currFrameWork = combatFrameWork::STGM; DEBUG("using global time framework!"); break;
	default: currFrameWork = combatFrameWork::Vanilla; DEBUG("invalid framework setting. Using Skysa framework."); break;
	}
	ReadBoolSetting(ini, "General", "bStopOnCreature", stopOnCreature);
	ReadBoolSetting(ini, "General", "bStopOnDead", stopOnDead);
	ReadBoolSetting(ini, "General", "bStopOnObject", stopOnObject);
	ReadBoolSetting(ini, "General", "bStopOnBash", stopOnBash);
	ReadBoolSetting(ini, "General", "bStopOnBlocked", stopOnBlocked);
	stopHandler::refreshVal();
}

void dataHandler::ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound)
	{
		a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
	}
}

void dataHandler::ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, int& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = static_cast<int>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}