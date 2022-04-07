#pragma once
using EventResult = RE::BSEventNotifyControl;
class onHitEventHandler : public RE::BSTEventSink<RE::TESHitEvent>
{
public:

	virtual EventResult ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>* a_eventSource);


	static bool Register()
	{
		static onHitEventHandler singleton;
		auto ScriptEventSource = RE::ScriptEventSourceHolder::GetSingleton();

		if (!ScriptEventSource) {
			ERROR("Script event source not found");
			return false;
		}

		ScriptEventSource->AddEventSink(&singleton);

		INFO("Register {}.", typeid(singleton).name());

		return true;
	}

private:
	inline void processMeleeHit(const RE::TESHitEvent* a_event, RE::Actor* hitter, RE::TESObjectWEAP* weapon);
	onHitEventHandler() = default;
	~onHitEventHandler() = default;
	onHitEventHandler(const onHitEventHandler&) = delete;
	onHitEventHandler(onHitEventHandler&&) = delete;
	onHitEventHandler& operator=(const onHitEventHandler&) = delete;
	onHitEventHandler& operator=(onHitEventHandler&&) = delete;

	inline bool isObject(RE::TESObjectREFR* a_obj) {
		if (a_obj->formType == RE::FormType::ActorCharacter) {
			//DEBUG("Target is actor!");
			return false;
		}
		return true;
	}


	inline bool isAlive(RE::Actor* a_target) {				//stolen from Maxsu's OHAF

		if (a_target->IsDead()) {
			//DEBUG("Target Actor is Dead!");
			return false;
		}
		return true;
	}
};