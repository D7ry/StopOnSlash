#include "onHitEventHandler.h"
#include "dataHandler.h"
#include "hitStop.h"
using EventResult = RE::BSEventNotifyControl;
using HitFlag = RE::TESHitEvent::Flag;

EventResult onHitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) {
	if (!a_event || !a_eventSource) {
		return EventResult::kContinue;
	}

	auto cause = a_event->cause;
	if (!cause || !a_event->target || !a_event->source) {
		return EventResult::kContinue;
	}

	auto hitsource = RE::TESForm::LookupByID<RE::TESObjectWEAP>(a_event->source);

	if (!hitsource) {
		return EventResult::kContinue;
	}

	if (!hitsource->IsWeapon() || !hitsource->IsMelee()) {
		return EventResult::kContinue;
	}

	
	if (cause->IsPlayerRef()) {
		if (!settings::pcHitStop) {
			return EventResult::kContinue;
		}
	}
	else if (cause->formType == RE::FormType::ActorCharacter) {
		if (!settings::npcHitStop || settings::currFramework == dataHandler::stopMethod::SGTM) {
			return EventResult::kContinue;
		}
	}

	processMeleeHit(a_event, cause->As<RE::Actor>(), hitsource);
	return EventResult::kContinue;
}

static bool isPowerAttacking(RE::Actor* a_actor)
{
	auto currentProcess = a_actor->GetActorRuntimeData().currentProcess;
	if (currentProcess) {
		auto highProcess = currentProcess->high;
		if (highProcess) {
			auto attackData = highProcess->attackData;
			if (attackData) {
				auto flags = attackData->data.flags;
				return flags.any(RE::AttackData::AttackFlag::kPowerAttack) && !flags.any(RE::AttackData::AttackFlag::kBashAttack);
			}
		}
	}
	return false;
}

void onHitEventHandler::processMeleeHit(const RE::TESHitEvent* a_event, RE::Actor* hitter, RE::TESObjectWEAP* weapon) {
	auto target = a_event->target.get();
	if (!target) {
		return;
	}

	if (!hitter->Is3DLoaded()) {
		return;
	}
	bool isPower = a_event->flags.any(RE::TESHitEvent::Flag::kPowerAttack);

	if (!isPower) {
		isPower = isPowerAttacking(hitter);
	}

	//iff target is object
	if (isObject(target)) {
		if (!settings::stopOnObject) {
			return;
		}
		hitStop::GetSingleton()->calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::objectStop);
		return;
	}


	//target now has to be a creature.
	if (!settings::stopOnCreature) {
		return;
	}

	auto targetActor = target->As<RE::Actor>();
	if (!isAlive(targetActor)) {
		if (!settings::stopOnDead) {
			return;
		}
		hitStop::GetSingleton()->calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::creatureStop);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kBashAttack)) {
		if (!settings::stopOnBash) {
			return;
		}
		hitStop::GetSingleton()->calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::bashStop);
		return;
	}

	if (a_event->flags.any(RE::TESHitEvent::Flag::kHitBlocked)) {
		if (!settings::stopOnBlocked) {
			return;
		}
		hitStop::GetSingleton()->calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::blockedStop);
		return;
	}


	//iff all above are checked, it can only be a living creature.
	hitStop::GetSingleton()->calculateStop(isPower, hitter, weapon, hitStop::STOPTYPE::creatureStop);
}





