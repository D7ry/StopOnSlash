#include "stopHandler.h"
#include "hitStop.h"
#include "SimpleIni.h"
/*
//returns primary weapon pc is holding. Any failure would return H2H weapon.
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

RE::WEAPON_TYPE getWpnType(RE::Actor* a_actor) {
	if (!a_actor) {
		return RE::WEAPON_TYPE::kHandToHandMelee;
	}
	auto wpnR = a_actor->GetEquippedObject(false);
	auto wpnL = a_actor->GetEquippedObject(true);
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
} */


void stopHandler::calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType) {
	RE::WEAPON_TYPE wpnType = weapon->GetWeaponType();
	hitStop::stop(getStopTime(wpnType, stopType, isPowerAtk), getStopSpeed(wpnType, stopType, isPowerAtk), hitter);
}

float stopHandler::getStopSpeed(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
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

float stopHandler::getStopTime(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
	float stopTime;
	using namespace settings::stopTimeMs;
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



