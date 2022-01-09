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

	if (hitsource->formType != RE::FormType::Weapon) {
		DEBUG("Hit Source Is Not Weapon!");
		return EventResult::kContinue;
	}

	if (a_event->cause->IsPlayerRef()) {
		playerMeleeHit(a_event);
	}

	return EventResult::kContinue;
}

void onHitEventHandler::playerMeleeHit(const RE::TESHitEvent* a_event) {
	DEBUG("player melee hit!");
	auto target = a_event->target.get();
	if (!target) {
		DEBUG("Target Actor Not Found!");
		return;
	}

	bool isPower = a_event->flags.any(RE::TESHitEvent::Flag::kPowerAttack);

	if (!isPower) {
		auto pc = RE::PlayerCharacter::GetSingleton();
		if (pc && pc->currentProcess && pc->currentProcess->high && pc->currentProcess->high->attackData && pc->currentProcess->high->attackData->data.flags.any(RE::AttackData::AttackFlag::kPowerAttack)) {
			isPower = true;
		}
	}

	//iff target is object
	if (isObject(target)) {
		if (!dataHandler::GetSingleton()->stopOnObject) {
			DEBUG("object, no hit stop!");
			return;
		}
		DEBUG("stop on object");
		stopHandler::objectStop(isPower);
		return;
	}


	//target now has to be a creature.
	if (!dataHandler::GetSingleton()->stopOnCreature) {
		DEBUG("hit creature, no hitstop!");
		return;
	}

	auto targetActor = target->As<RE::Actor>();
	if (!isAlive(targetActor)) {
		if (!dataHandler::GetSingleton()->stopOnDead) {
			DEBUG("dead actor, no hit stop!");
			return;
		}
		DEBUG("stop on dead creature!");
		stopHandler::creatureStop(isPower);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kBashAttack)) {
		if (!dataHandler::GetSingleton()->stopOnBash) {
			DEBUG("bash hit, no hit stop!");
			return;
		}
		DEBUG("stop on bash!");
		stopHandler::bashStop(isPower);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked)) {
		if (!dataHandler::GetSingleton()->stopOnBlocked) {
			DEBUG("hit blocked, no hit stop!");
			return;
		}
		DEBUG("stop on blocked attack!");
		stopHandler::blockedStop(isPower);
		return;
	}


	//iff all above are checked, it can only be a living creature.
	DEBUG("stop on creature!");
	stopHandler::creatureStop(isPower);
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

