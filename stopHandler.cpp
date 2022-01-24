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







