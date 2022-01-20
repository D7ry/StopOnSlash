#include "onHitEventHandler.h"
#include "stopHandler.h"
#include "dataHandler.h"
using EventResult = RE::BSEventNotifyControl;
using HitFlag = RE::TESHitEvent::Flag;
EventResult onHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
	if (!a_event || !a_eventSource) {
		ERROR("Event Source not found");
		return EventResult::kContinue;
	}
	if (!a_event->cause || !a_event->target || !a_event->source) {
		DEBUG("invalid hit event!");
		return EventResult::kContinue;
	}

	auto hitsource = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);

	if (!hitsource) {
		DEBUG("Weapon Hit Source Not Found!");
		return EventResult::kContinue;
	}

	if (!hitsource->IsWeapon() || !hitsource->IsMelee()) {
		DEBUG("Hit Source Is Not Melee Weapon!");
		return EventResult::kContinue;
	}

	if (a_event->cause->IsPlayerRef()) {
		processMeleeHit(a_event, a_event->cause->As<RE::Actor>(), hitsource);
	}
	else if (a_event->cause->formType == RE::FormType::ActorCharacter && settings::NPCstop && settings::currFramework != dataHandler::combatFrameWork::STGM) {
		processMeleeHit(a_event, a_event->cause->As<RE::Actor>(), hitsource);
	}

	return EventResult::kContinue;
}

void onHitEventHandler::processMeleeHit(const RE::TESHitEvent* a_event, RE::Actor* hitter, RE::TESObjectWEAP* weapon) {
	DEBUG("melee hit!");
	auto target = a_event->target.get();
	if (!target) {
		return;
	}

	if (!hitter->Is3DLoaded()) {
		return;
	}
	bool isPower = a_event->flags.any(RE::TESHitEvent::Flag::kPowerAttack);

	if (!isPower) {
		if (hitter && hitter->currentProcess && hitter->currentProcess->high && hitter->currentProcess->high->attackData && hitter->currentProcess->high->attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
			isPower = true;
		}
	}

	//iff target is object
	if (isObject(target)) {
		if (!settings::stopOnObject) {
			DEBUG("object, no hit stop!");
			return;
		}
		DEBUG("stop on object");
		stopHandler::calculateStop(isPower, hitter, weapon, stopHandler::STOPTYPE::objectStop);
		return;
	}


	//target now has to be a creature.
	if (!settings::stopOnCreature) {
		DEBUG("hit creature, no hitstop!");
		return;
	}

	auto targetActor = target->As<RE::Actor>();
	if (!isAlive(targetActor)) {
		if (!settings::stopOnDead) {
			DEBUG("dead actor, no hit stop!");
			return;
		}
		DEBUG("stop on dead creature!");
		stopHandler::calculateStop(isPower, hitter, weapon, stopHandler::STOPTYPE::creatureStop);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kBashAttack)) {
		if (!settings::stopOnBash) {
			DEBUG("bash hit, no hit stop!");
			return;
		}
		DEBUG("stop on bash!");
		stopHandler::calculateStop(isPower, hitter, weapon, stopHandler::STOPTYPE::bashStop);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked)) {
		if (!settings::stopOnBlocked) {
			DEBUG("hit blocked, no hit stop!");
			return;
		}
		DEBUG("stop on blocked attack!");
		stopHandler::calculateStop(isPower, hitter, weapon, stopHandler::STOPTYPE::blockedStop);
		return;
	}


	//iff all above are checked, it can only be a living creature.
	DEBUG("stop on creature!");
	stopHandler::calculateStop(isPower, hitter, weapon, stopHandler::STOPTYPE::creatureStop);
}



bool onHitEventHandler::isObject(RE::TESObjectREFR* a_obj) {
	if (a_obj->formType == RE::FormType::ActorCharacter) {
		DEBUG("Target is actor!");
		return false;
	}
	return true;
}


bool onHitEventHandler::isAlive(RE::Actor* a_target) {				//stolen from Maxsu's OHAF

	if (a_target->IsDead()) {
		DEBUG("Target Actor is Dead!");
		return false;
	}
	if (a_target->IsInKillMove()) {
		DEBUG("Target Actor Is in Kill Move!");
		return false;
	}
	return true;
}

