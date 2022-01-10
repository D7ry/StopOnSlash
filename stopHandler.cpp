#include "stopHandler.h"
#include "hitStop.h"
#include "SimpleIni.h"

/*returns primary weapon pc is holding. Any failure would return H2H weapon.*/
RE::WEAPON_TYPE getPcWpnEnum() {
	auto pc = RE::PlayerCharacter::GetSingleton();
	if (!pc) {
		DEBUG("pc not found!");
		return RE::WEAPON_TYPE::kHandToHandMelee;
	}
	auto wpnR = pc->GetEquippedObject(false);
	auto wpnL = pc->GetEquippedObject(true);
	if (wpnR && wpnR->IsWeapon()) {
		RE::WEAPON_TYPE Rtype = wpnR->As<RE::TESObjectWEAP>()->GetWeaponType();
		if (Rtype != RE::WEAPON_TYPE::kHandToHandMelee) {
			return Rtype;
		}
	}
	if (wpnL && wpnL->IsWeapon()) {
		return wpnL->As<RE::TESObjectWEAP>()->GetWeaponType();
	}
	return RE::WEAPON_TYPE::kUnknown;
}


void stopHandler::objectStop(bool isPowerAtk) {
	RE::WEAPON_TYPE wpn = getPcWpnEnum();
	powerMult(stopTimeMs::objectStopTime * getTimeMult(wpn), stopSpeed::objectStopSpeedPercent * getSpeedMult(wpn), isPowerAtk);
}

void stopHandler::bashStop(bool isPowerAtk) {
	RE::WEAPON_TYPE wpn = getPcWpnEnum();
	powerMult(stopTimeMs::bashStopTime * getTimeMult(wpn), stopSpeed::bashStopSpeedPercent * getSpeedMult(wpn), isPowerAtk);
}

void stopHandler::blockedStop(bool isPowerAtk) {
	RE::WEAPON_TYPE wpn = getPcWpnEnum();
	powerMult(stopTimeMs::blockedStopTime * getTimeMult(wpn), stopSpeed::blockedStopSpeedPercent * getSpeedMult(wpn), isPowerAtk);
}

void stopHandler::creatureStop(bool isPowerAtk) {
	RE::WEAPON_TYPE wpn = getPcWpnEnum();
	powerMult(stopTimeMs::creatureStopTime * getTimeMult(wpn), stopSpeed::creatureStopSpeedPercent * getSpeedMult(wpn), isPowerAtk);
}

void stopHandler::powerMult(float stopTime, float stopSpeed, bool isPower) {
	if (isPower) {
		DEBUG("applying power offset!");
		DEBUG("stop time is multiplied by {}, and stop speed is multiplied by {}", stopTimeMs::stopTimePowerMult, stopSpeed::stopSpeedPowerMult);
		hitStop::stop(stopTime * stopTimeMs::stopTimePowerMult, stopSpeed * stopSpeed::stopSpeedPowerMult);
	}
	else {
		hitStop::stop(stopTime, stopSpeed);
	}
}



float stopHandler::getSpeedMult(RE::WEAPON_TYPE wpn) {
	using weapon = RE::WEAPON_TYPE;
	using namespace stopSpeed;
	switch (wpn)
	{
	case weapon::kHandToHandMelee: return stopSpeedHandToHandMult; break;
	case weapon::kOneHandDagger: return stopSpeedDaggerMult; break;
	case weapon::kOneHandSword: return stopSpeedSwordMult; break;
	case weapon::kOneHandMace: return stopSpeedMaceMult; break;
	case weapon::kOneHandAxe: return stopSpeedAxeMult; break;
	case weapon::kTwoHandAxe: return stopSpeed2hwMult; break;
	case weapon::kTwoHandSword: return stopSpeedGreatSwordMult; break;
	default: return 1; break;
	}
}

float stopHandler::getTimeMult(RE::WEAPON_TYPE wpn) {
	using weapon = RE::WEAPON_TYPE;
	using namespace stopTimeMs;
	switch (wpn)
	{
	case weapon::kHandToHandMelee: return stopTimeHandToHandMult; break;
	case weapon::kOneHandDagger: return stopTimeDaggerMult; break;
	case weapon::kOneHandSword: return stopTimeSwordMult; break;
	case weapon::kOneHandMace: return stopTimeMaceMult; break;
	case weapon::kOneHandAxe: return stopTimeAxeMult; break;
	case weapon::kTwoHandAxe: return stopTime2hwMult; break;
	case weapon::kTwoHandSword: return stopTimeGreatSwordMult; break;
	default: return 1; break;
	}
}


static inline void ReadFloatSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
	DEBUG("reading float setting!");
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		DEBUG("found {} with value {}", a_sectionName, bFound);
		a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
	else {
		DEBUG("failed to find {} from {}", a_settingName, a_sectionName);
	}
}

void stopHandler::refreshVal() {
	CSimpleIniA ini;
	#define SETTINGFILE_PATH "Data\\MCM\\Settings\\StopOnSlash.ini"
	ini.LoadFile(SETTINGFILE_PATH);
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
