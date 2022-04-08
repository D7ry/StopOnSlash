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

	bool pcHitStop = true;

	bool SGTMStop = false;

	bool npcHitStop = true;

	bool stopOnCreature = true;

	bool stopOnObject = true;

	bool stopOnDead = true;

	bool stopOnBash = true;

	bool stopOnBlocked = true;


	bool pcShake = true;
	bool shakeOnLight = false;

	bool shakeOnCreature = true;

	bool shakeOnObject = true;

	bool shakeOnDead = true;

	bool shakeOnBash = true;

	bool shakeOnBlocked = true;

	namespace stopTime
	{
		float objectStopTime = 0.1;
		float bashStopTime = 0.1;
		float blockedStopTime = 0.1;
		float creatureStopTime = 0.1;

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

	namespace shakeTime
	{
		float objectShakeTime = 0.3;
		float bashShakeTime = 0.3;
		float blockedShakeTime = 0.3;
		float creatureShakeTime = 0.3;

		float ShakeTimePowerMult = 1;

		float ShakeTimeHandToHandMult = 1;
		float ShakeTimeDaggerMult = 1;
		float ShakeTimeSwordMult = 1;
		float ShakeTimeGreatSwordMult = 1;
		float ShakeTimeAxeMult = 1;
		float ShakeTimeMaceMult = 1;
		float ShakeTime2hwMult = 1;
	}

	namespace shakeMagnitude
	{
		float objectShakeMagnitude = 0.3;
		float bashShakeMagnitude = 0.3;
		float blockedShakeMagnitude = 0.3;
		float creatureShakeMagnitude = 0.3;

		float ShakeMagnitudePowerMult = 1;

		float ShakeMagnitudeHandToHandMult = 1;
		float ShakeMagnitudeDaggerMult = 1;
		float ShakeMagnitudeSwordMult = 1;
		float ShakeMagnitudeGreatSwordMult = 1;
		float ShakeMagnitudeAxeMult = 1;
		float ShakeMagnitudeMaceMult = 1;
		float ShakeMagnitude2hwMult = 1;
	}
}


void dataHandler::readSettings() {
	DEBUG("getting settings!");
	CSimpleIniA ini;
#define SETTINGFILE_PATH "Data\\MCM\\Settings\\StopOnSlash.ini"
	ini.LoadFile(SETTINGFILE_PATH);

	//int frameworkInt;
	//ReadIntSetting(ini, "General", "iFrameWork", frameworkInt);

	using namespace settings;
	//INFO("frameworkInt set to {}", frameworkInt);
	/*switch (frameworkInt) {
	case 0: currFramework = combatFrameWork::Vanilla; DEBUG("using vanilla framework!"); break;
	case 1: currFramework = combatFrameWork::Skysa2; DEBUG("using Skysa2 framework!"); break;
	case 2: currFramework = combatFrameWork::MCO; DEBUG("using global time framework!"); break;
	case 3: currFramework = combatFrameWork::SGTM; DEBUG("using global time framework!"); break;
	default: currFramework = combatFrameWork::Vanilla; DEBUG("invalid framework setting. Using Vanilla framework."); break;
	}*/
	ReadBoolSetting(ini, "General", "bPChitStop", pcHitStop);
	ReadBoolSetting(ini, "General", "bNPChitStop", npcHitStop);
	ReadBoolSetting(ini, "General", "bStopOnCreature", stopOnCreature);
	ReadBoolSetting(ini, "General", "bStopOnDead", stopOnDead);
	ReadBoolSetting(ini, "General", "bStopOnObject", stopOnObject);
	ReadBoolSetting(ini, "General", "bStopOnBash", stopOnBash);
	ReadBoolSetting(ini, "General", "bStopOnBlocked", stopOnBlocked);

	ReadBoolSetting(ini, "General", "bpcShake", pcShake);
	ReadBoolSetting(ini, "General", "bshakeOnLight", shakeOnLight);
	ReadBoolSetting(ini, "General", "bshakeOnCreature", shakeOnCreature);
	ReadBoolSetting(ini, "General", "bshakeOnDead", shakeOnDead);
	ReadBoolSetting(ini, "General", "bshakeOnObject", shakeOnObject);
	ReadBoolSetting(ini, "General", "bshakeOnBash", shakeOnBash);
	ReadBoolSetting(ini, "General", "bshakeOnBlocked", shakeOnBlocked);
	using namespace stopTime;
	ReadFloatSetting(ini, "StopTime", "fstopOnBash", bashStopTime);
	ReadFloatSetting(ini, "StopTime", "fstopOnObject", objectStopTime);
	ReadFloatSetting(ini, "StopTime", "fstopOnBlocked", blockedStopTime);
	ReadFloatSetting(ini, "StopTime", "fstopOnCreature", creatureStopTime);

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

	using namespace shakeMagnitude;
	ReadFloatSetting(ini, "ShakeMagnitude", "fbashShakeMagnitude", bashShakeMagnitude);
	ReadFloatSetting(ini, "ShakeMagnitude", "fobjectShakeMagnitude", objectShakeMagnitude);
	ReadFloatSetting(ini, "ShakeMagnitude", "fblockedShakeMagnitude", blockedShakeMagnitude);
	ReadFloatSetting(ini, "ShakeMagnitude", "fcreatureShakeMagnitude", creatureShakeMagnitude);

	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitudePowerMult", ShakeMagnitudePowerMult);

	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitudeHandToHandMult", ShakeMagnitudeHandToHandMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitudeDaggerMult", ShakeMagnitudeDaggerMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitudeSwordMult", ShakeMagnitudeSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitudeGreatSwordMult", ShakeMagnitudeGreatSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitudeAxeMult", ShakeMagnitudeAxeMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitudeMaceMult", ShakeMagnitudeMaceMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeMagnitude2hwMult", ShakeMagnitude2hwMult);

	using namespace shakeTime;
	ReadFloatSetting(ini, "ShakeTime", "fShakeOnBash", bashShakeTime);
	ReadFloatSetting(ini, "ShakeTime", "fShakeOnObject", objectShakeTime);
	ReadFloatSetting(ini, "ShakeTime", "fShakeOnBlocked", blockedShakeTime);
	ReadFloatSetting(ini, "ShakeTime", "fShakeOnCreature", creatureShakeTime);

	ReadFloatSetting(ini, "Multiplier", "fShakeTimePowerMult", ShakeTimePowerMult);

	ReadFloatSetting(ini, "Multiplier", "fShakeTimeHandToHandMult", ShakeTimeHandToHandMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeTimeDaggerMult", ShakeTimeDaggerMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeTimeSwordMult", ShakeTimeSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeTimeGreatSwordMult", ShakeTimeGreatSwordMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeTimeAxeMult", ShakeTimeAxeMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeTimeMaceMult", ShakeTimeMaceMult);
	ReadFloatSetting(ini, "Multiplier", "fShakeTime2hwMult", ShakeTime2hwMult);
}

void dataHandler::initGlobals() {
	INFO("Initilize global settings...");
	auto dataHandler = RE::TESDataHandler::GetSingleton();
	if (dataHandler) {
#define LOOKGLOBAL dataHandler->LookupForm<RE::TESGlobal>
		globals::glob_Nemesis_SkysaT = LOOKGLOBAL(0x803, "StopOnSlash.esp");
		globals::glob_Nemesis_MCO = LOOKGLOBAL(0x804, "StopOnSlash.esp");
		globals::glob_Nemesis_Vanilla = LOOKGLOBAL(0x805, "StopOnSlash.esp");
	}
	INFO("...done");
}

void dataHandler::updateGlobals() {
	INFO("Update global settings...");

	auto pc = RE::PlayerCharacter::GetSingleton();
	if (pc) {
		float bDummy1;
		bool SkysaT = false;
		SkysaT = pc->GetGraphVariableFloat("SkySA_weaponSpeedMult", bDummy1);

		float bDummy2;
		bool MCO = false;
		MCO = pc->GetGraphVariableFloat("MCO_AttackSpeed", bDummy2);
		if (SkysaT) {
			globals::glob_Nemesis_SkysaT->value = true;
			globals::glob_Nemesis_MCO->value = false;
			globals::glob_Nemesis_Vanilla->value = false;
			settings::currFramework = dataHandler::combatFrameWork::Skysa2;
		} else if (MCO) {
			globals::glob_Nemesis_MCO->value = true;
			globals::glob_Nemesis_SkysaT->value = false;
			globals::glob_Nemesis_Vanilla->value = false;
			settings::currFramework = dataHandler::combatFrameWork::MCO;
		}
		else {
			globals::glob_Nemesis_MCO->value = false;
			globals::glob_Nemesis_SkysaT->value = false;
			globals::glob_Nemesis_Vanilla->value = true;
			settings::currFramework = dataHandler::combatFrameWork::Vanilla;
		}

		if (settings::SGTMStop) {
			settings::currFramework = dataHandler::combatFrameWork::SGTM;
		}
			
	}
	INFO("...done");

}