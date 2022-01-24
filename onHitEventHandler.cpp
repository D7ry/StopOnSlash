#include "onHitEventHandler.h"
#include "stopHandler.h"
#include "dataHandler.h"
#include "hitStop.h"
using EventResult = RE::BSEventNotifyControl;
using HitFlag = RE::TESHitEvent::Flag;



EventResult onHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
	if (!a_event || !a_eventSource) {
		ERROR("Event Source not found");
		return EventResult::kContinue;
	}

	auto cause = a_event->cause;
	if (!cause || !a_event->target || !a_event->source) {
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
		
	}

	
	if (cause->IsPlayerRef()) {
		if (!settings::pcHitStop) {
			return EventResult::kContinue;
		}
	}
	else if (cause->formType == RE::FormType::ActorCharacter) {
		if (!settings::npcHitStop || settings::currFramework == dataHandler::combatFrameWork::STGM) {
			return EventResult::kContinue;
		}
	}

	processMeleeHit(a_event, cause->As<RE::Actor>(), hitsource);
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
		hitStop::calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::objectStop);
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
		hitStop::calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::creatureStop);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kBashAttack)) {
		if (!settings::stopOnBash) {
			DEBUG("bash hit, no hit stop!");
			return;
		}
		DEBUG("stop on bash!");
		hitStop::calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::bashStop);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked)) {
		if (!settings::stopOnBlocked) {
			DEBUG("hit blocked, no hit stop!");
			return;
		}
		DEBUG("stop on blocked attack!");
		hitStop::calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::blockedStop);
		return;
	}


	//iff all above are checked, it can only be a living creature.
	DEBUG("stop on creature!");
	hitStop::calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::creatureStop);
}





