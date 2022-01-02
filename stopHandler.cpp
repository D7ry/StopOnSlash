#include "C:/Users/Ty/Documents/SKSEPlugins/Build/Plugins/dTryHitStop/CMakeFiles/dTryHitStop.dir/Debug/cmake_pch.hxx"
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
		return wpnR->As<RE::TESObjectWEAP>()->GetWeaponType();
	}
	else if (wpnL && wpnL->IsWeapon()) {
		return wpnL->As<RE::TESObjectWEAP>()->GetWeaponType();
	}
	else {
		return RE::WEAPON_TYPE::kHandToHandMelee;
	}
}


void stopHandler::objectStop(bool isPowerAtk) {
	powerMult(stopTimeMs::objectStopTime * getTimeMult(), stopSpeed::objectStopSpeedPercent * getSpeedMult(), isPowerAtk);
}

void stopHandler::bashStop(bool isPowerAtk) {
	powerMult(stopTimeMs::bashStopTime * getTimeMult(), stopSpeed::bashStopSpeedPercent * getSpeedMult(), isPowerAtk);
}

void stopHandler::blockedStop(bool isPowerAtk) {
	powerMult(stopTimeMs::blockedStopTime * getTimeMult(), stopSpeed::blockedStopSpeedPercent * getSpeedMult(), isPowerAtk);
}

void stopHandler::creatureStop(bool isPowerAtk) {
	powerMult(stopTimeMs::creatureStopTime * getTimeMult(), stopSpeed::creatureStopSpeedPercent * getSpeedMult(), isPowerAtk);
}

void stopHandler::powerMult(float stopTime, float stopSpeed, float isPower) {
	if (isPower) {
		hitStop::stop(stopTime * stopTimeMs::stopTimePowerMult, stopSpeed * stopSpeed::stopSpeedPowerMult);
	}
	else {
		hitStop::stop(stopTime, stopSpeed);
	}
}



float stopHandler::getSpeedMult() {
	using weapon = RE::WEAPON_TYPE;
	using namespace stopSpeed;
	switch (getPcWpnEnum())
	{
	case weapon::kHandToHandMelee: return stopSpeedHandToHandMult; break;
	case weapon::kOneHandSword: return stopSpeedSwordMult; break;
	case weapon::kOneHandMace: return stopSpeedMaceMult; break;
	case weapon::kOneHandAxe: return stopSpeedAxeMult; break;
	case weapon::kTwoHandAxe: return stopSpeed2hwMult; break;
	case weapon::kTwoHandSword: return stopSpeedGreatSwordMult; break;
	default: return stopSpeedHandToHandMult; break;
	}
}

float stopHandler::getTimeMult() {
	using weapon = RE::WEAPON_TYPE;
	using namespace stopTimeMs;
	switch (getPcWpnEnum())
	{
	case weapon::kHandToHandMelee: return stopTimeHandToHandMult; break;
	case weapon::kOneHandSword: return stopTimeSwordMult; break;
	case weapon::kOneHandMace: return stopTimeMaceMult; break;
	case weapon::kOneHandAxe: return stopTimeAxeMult; break;
	case weapon::kTwoHandAxe: return stopTime2hwMult; break;
	case weapon::kTwoHandSword: return stopTimeGreatSwordMult; break;
	default: return stopTimeHandToHandMult; break;
	}
}

void stopHandler::refreshVal() {
	CSimpleIniA ini;
	#define SETTINGFILE_PATH "Data\\SKSE\\Plugins\\dTryHitStop.ini"
	ini.LoadFile(SETTINGFILE_PATH);
	using namespace stopTimeMs;
	bashStopTime = ini.GetDoubleValue("StopOnBash", "StopTimeMs", 0);
	objectStopTime = ini.GetDoubleValue("StopOnObject", "StopTimeMs", 0);
	DEBUG("reading from ini, obejct stop time is supposed to be {}", ini.GetDoubleValue("StopOnObject", "StopTimeMs", 0));
	DEBUG("object actual stop time is {}", objectStopTime);
	blockedStopTime = ini.GetDoubleValue("StopOnBlocked", "StopTimeMs", 0);
	creatureStopTime = ini.GetDoubleValue("StopOnCreature", "StopTimeMs", 0);

	stopTimePowerMult = ini.GetDoubleValue("Multiplier", "stopTimePowerMult", 1);

	stopTimeHandToHandMult = ini.GetDoubleValue("Multiplier", "stopTimeHandToHandMult", 1);
	stopTimeDaggerMult = ini.GetDoubleValue("Multiplier", "stopTimeDaggerMult", 1);
	stopTimeSwordMult = ini.GetDoubleValue("Multiplier", "stopTimeSwordMult", 1);
	stopTimeGreatSwordMult = ini.GetDoubleValue("Multiplier", "stopTimeGreatSwordMult", 1);
	stopTimeAxeMult = ini.GetDoubleValue("Multiplier", "stopTimeAxeMult", 1);
	stopTime2hwMult = ini.GetDoubleValue("Multiplier", "stopTime2hwMult", 1);
	stopTimeMaceMult = ini.GetDoubleValue("Multiplier", "stopTimeMaceMult", 1);

	using namespace stopSpeed;
	bashStopSpeedPercent = ini.GetDoubleValue("StopOnBash", "StopSpeedPercent", 100);
	objectStopSpeedPercent = ini.GetDoubleValue("StopOnObject", "StopSpeedPercent", 100);
	DEBUG("reading from ini, obejct stop speed is set to be {}", ini.GetDoubleValue("StopOnObject", "StopSpeedPercent", 100));
	blockedStopSpeedPercent = ini.GetDoubleValue("StopOnBlocked", "StopSpeedPercent", 100);
	creatureStopSpeedPercent = ini.GetDoubleValue("StopOnCreature", "StopSpeedPercent", 100);

	stopSpeedPowerMult = ini.GetDoubleValue("Multiplier", "stopSpeedPowerMult", 1);

	stopSpeedHandToHandMult = ini.GetDoubleValue("Multiplier", "stopSpeedHandToHandMult", 1);
	stopSpeedDaggerMult = ini.GetDoubleValue("Multiplier", "stopSpeedDaggerMult", 1);
	stopSpeedSwordMult = ini.GetDoubleValue("Multiplier", "stopSpeedSwordMult", 1);
	stopSpeedGreatSwordMult = ini.GetDoubleValue("Multiplier", "stopSpeedGreatSwordMult", 1);
	stopSpeedAxeMult = ini.GetDoubleValue("Multiplier", "stopSpeedAxeMult", 1);
	stopSpeed2hwMult = ini.GetDoubleValue("Multiplier", "stopSpeed2hwMult", 1);
	stopSpeedMaceMult = ini.GetDoubleValue("Multiplier", "stopSpeedMaceMult", 1);
}