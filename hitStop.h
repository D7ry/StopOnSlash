#pragma once
#include "dataHandler.h"
namespace Utils
{
	inline static float* g_deltaTime = (float*)RELOCATION_ID(523660, 410199).address();  // 2F6B948
	inline static float* g_deltaTimeRealTime = (float*)RELOCATION_ID(523661, 410200).address();  // 2F6B94C
}

class AnimSpeedManager
{
	class on_updateAnimation_internal;
	class on_updateAnimation_player;

public:
	static void setAnimSpeed(RE::ActorHandle a_actorHandle, float a_speedMult, float a_dur);
	static void revertAnimSpeed(RE::ActorHandle a_actorHandle);
	static void revertAllAnimSpeed();

	static void init()
	{
		on_updateAnimation_internal::install();
		on_updateAnimation_player::install();
	}

private:
	struct AnimSpeedData
	{
		float speedMult;
		float dur;
	};
	static inline std::unordered_map<RE::ActorHandle, AnimSpeedData> _animSpeeds = {};
	static inline std::mutex _animSpeedsLock = std::mutex();

	static void update(RE::ActorHandle a_actorHandle, float& a_deltaTime);

	class on_updateAnimation_player
	{
	public:
		static void install()
		{
			auto& trampoline = SKSE::GetTrampoline();
			REL::Relocation<std::uintptr_t> PlayerCharacterVtbl{ RE::VTABLE_PlayerCharacter[0] };
			_PlayerCharacter_UpdateAnimation = PlayerCharacterVtbl.write_vfunc(0x7D, PlayerCharacter_UpdateAnimation);
			logger::info("hook:on_updateAnimation_player");
		}

	private:
		static void PlayerCharacter_UpdateAnimation(RE::PlayerCharacter* a_this, float a_deltaTime)
		{
			AnimSpeedManager::update(a_this->GetHandle(), a_deltaTime);
			_PlayerCharacter_UpdateAnimation(a_this, a_deltaTime);
		}
		static inline REL::Relocation<decltype(PlayerCharacter_UpdateAnimation)> _PlayerCharacter_UpdateAnimation;
	};

	class on_updateAnimation_internal
	{
	public:
		static void install()
		{
			auto& trampoline = SKSE::GetTrampoline();
			REL::Relocation<uintptr_t> hook{ RELOCATION_ID(40436, 41453) };                                                                // 6E1990, 70A840, RunOneActorAnimationUpdateJob
			_UpdateAnimationInternal = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(0x74, 0x74), UpdateAnimationInternal);  // 6E1A04, 70A8B4;
			logger::info("hook:on_updateAnimation_internal");
		}

	private:
		static void UpdateAnimationInternal(RE::Actor* a_this, float a_deltaTime)
		{
			AnimSpeedManager::update(a_this->GetHandle(), a_deltaTime);
			_UpdateAnimationInternal(a_this, a_deltaTime);
		}
		static inline REL::Relocation<decltype(UpdateAnimationInternal)> _UpdateAnimationInternal;
	};
};

class hitStop
{
public:

	static hitStop* GetSingleton()
	{
		static hitStop singleton;
		return  std::addressof(singleton);
	}

	void update();


	enum STOPTYPE
	{
		objectStop = 0,
		bashStop = 1,
		blockedStop = 2,
		creatureStop = 3
	};

	void calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType);


	inline void stop(float stopTime, float stopSpeed, RE::Actor* a_actor);


private:
	
	float stopSGTM_time;
	bool stopSGTM_ongoing;
	void stopSGTM(float stopTime, float stopSpeed, RE::Actor* a_actor);

	void revertSGTM();

	inline float getStopSpeed(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
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

	inline float getStopTime(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float stopTime;
		using namespace settings::stopTime;
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

	inline float getShakeTime(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float shakeTime;
		using namespace settings::shakeTime;
		switch (stopType)
		{
		case STOPTYPE::bashStop: shakeTime = bashShakeTime; break;
		case STOPTYPE::blockedStop: shakeTime = blockedShakeTime; break;
		case STOPTYPE::creatureStop: shakeTime = creatureShakeTime; break;
		case STOPTYPE::objectStop: shakeTime = objectShakeTime; break;
		default: shakeTime = 0;
		}
		switch (wpnType)
		{
		case RE::WEAPON_TYPE::kHandToHandMelee: shakeTime *= ShakeTimeHandToHandMult; break;
		case RE::WEAPON_TYPE::kOneHandDagger: shakeTime *= ShakeTimeDaggerMult; break;
		case RE::WEAPON_TYPE::kOneHandSword: shakeTime *= ShakeTimeSwordMult; break;
		case RE::WEAPON_TYPE::kOneHandMace: shakeTime *= ShakeTimeMaceMult; break;
		case RE::WEAPON_TYPE::kOneHandAxe: shakeTime *= ShakeTimeAxeMult; break;
		case RE::WEAPON_TYPE::kTwoHandAxe: shakeTime *= ShakeTime2hwMult; break;
		case RE::WEAPON_TYPE::kTwoHandSword: shakeTime *= ShakeTimeGreatSwordMult; break;
		}
		if (isPower) {
			shakeTime *= ShakeTimePowerMult;
		}
		return shakeTime;
	}

	inline float getShakeMagnitude(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower) {
		float shakeMag;
		using namespace settings::shakeMagnitude;
		switch (stopType) {
		case STOPTYPE::bashStop: shakeMag = bashShakeMagnitude; break;
		case STOPTYPE::blockedStop: shakeMag = blockedShakeMagnitude; break;
		case STOPTYPE::creatureStop: shakeMag = creatureShakeMagnitude; break;
		case STOPTYPE::objectStop: shakeMag = objectShakeMagnitude; break;
		default: shakeMag = 0;
		}
		switch (wpnType) {
		case RE::WEAPON_TYPE::kHandToHandMelee: shakeMag *= ShakeMagnitudeHandToHandMult; break;
		case RE::WEAPON_TYPE::kOneHandDagger: shakeMag *= ShakeMagnitudeDaggerMult; break;
		case RE::WEAPON_TYPE::kOneHandSword: shakeMag *= ShakeMagnitudeSwordMult; break;
		case RE::WEAPON_TYPE::kOneHandMace: shakeMag *= ShakeMagnitudeMaceMult; break;
		case RE::WEAPON_TYPE::kOneHandAxe: shakeMag *= ShakeMagnitudeAxeMult; break;
		case RE::WEAPON_TYPE::kTwoHandAxe: shakeMag *= ShakeMagnitude2hwMult; break;
		case RE::WEAPON_TYPE::kTwoHandSword: shakeMag *= ShakeMagnitudeGreatSwordMult; break;
		}
		if (isPower) {
			shakeMag *= ShakeMagnitudePowerMult;
		}
		return shakeMag;
	}

};
	

class MainUpdateHook
{
public:
	static void InstallHook()
	{
		auto& trampoline = SKSE::GetTrampoline();
		REL::Relocation<uintptr_t> hook{ RELOCATION_ID(35565, 36564) };  // 5B2FF0, 5D9F50, main update

		_Nullsub = trampoline.write_call<5>(hook.address() + RELOCATION_OFFSET(0x748, 0xC26), Nullsub);  // 5B3738, 5DAB76
	}

private:
	static void Nullsub() {
		hitStop::GetSingleton()->update();
		_Nullsub();
	}
	static inline REL::Relocation<decltype(Nullsub)> _Nullsub;	

};
