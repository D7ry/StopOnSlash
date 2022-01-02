#include "dataHandler.h"
#include "stopHandler.h"
void dataHandler::readSettings() {
	DEBUG("getting settings!");
	CSimpleIniA ini;
	#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\dTryHitStop.ini"
	ini.LoadFile(SETTINGFILE_PATH);

	int frameworkInt = (int)ini.GetLongValue("General", "FrameWork", 0);
	switch (frameworkInt) {
	case 0: currFrameWork = combatFrameWork::Vanilla; DEBUG("using vanilla framework!"); break;
	case 1: currFrameWork = combatFrameWork::SkySa; DEBUG("using Skysa framework!"); break;
	case 2: currFrameWork = combatFrameWork::ABR; DEBUG("using ABR framework!"); break;
	case 3: currFrameWork = combatFrameWork::MCO; DEBUG("using MCO framework!"); break;
	default: currFrameWork = combatFrameWork::Vanilla; DEBUG("invalid framework setting. Using Skysa framework."); break;
	}

	stopOnCreature = ini.GetBoolValue("General", "StopOnCreature", true);
	stopOnObject = ini.GetBoolValue("General", "StopOnObject", true);
	stopOnDead = ini.GetBoolValue("General", "StopOnDead", true);
	stopOnBash = ini.GetBoolValue("General", "StopOnBash", true);
	stopOnBlocked = ini.GetBoolValue("General", "StopOnBlockedHit", true);

	stopHandler::refreshVal();


}