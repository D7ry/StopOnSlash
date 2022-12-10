#pragma once
#include "hitStop.h"
void AnimSpeedManager::setAnimSpeed(RE::ActorHandle a_actorHandle, float a_speedMult, float a_dur)
{
	auto it = _animSpeeds.find(a_actorHandle);
	if (it != _animSpeeds.end()) {
		it->second.speedMult = a_speedMult;
		it->second.dur = a_dur;
	} else {
		_animSpeeds.emplace(a_actorHandle, AnimSpeedData{ a_speedMult, a_dur });
	}
}

void AnimSpeedManager::revertAnimSpeed(RE::ActorHandle a_actorHandle)
{
	_animSpeedsLock.lock();
	auto it = _animSpeeds.find(a_actorHandle);
	if (it != _animSpeeds.end()) {
		it->second.speedMult = 1.0f;
		it->second.dur = 0.0f;
	}
	_animSpeedsLock.unlock();
}

void AnimSpeedManager::revertAllAnimSpeed()
{
	_animSpeedsLock.lock();
	_animSpeeds.clear();
	_animSpeedsLock.unlock();
}

void AnimSpeedManager::update(RE::ActorHandle a_actorHandle, float& a_deltaTime)
{
	if (a_deltaTime > 0.f) {
		_animSpeedsLock.lock();
		auto it = _animSpeeds.find(a_actorHandle);
		if (it != _animSpeeds.end()) {
			float newHitstopLength = it->second.dur - a_deltaTime;
			it->second.dur = newHitstopLength;

			float mult = 1.f;
			if (newHitstopLength <= 0.f) {
				mult = (a_deltaTime + newHitstopLength) / a_deltaTime;
				_animSpeeds.erase(it);
			}
			a_deltaTime *= it->second.speedMult + ((1.f - it->second.speedMult) * (1.f - mult));
		}
		_animSpeedsLock.unlock();
	}
}


inline void SGTM(float a_in) {
	static float* g_SGTM = (float*)RELOCATION_ID(511883, 388443).address();
	*g_SGTM = a_in;
	using func_t = decltype(SGTM);
	REL::Relocation<func_t> func{ RELOCATION_ID(66989, 68246) };
	return;
}

typedef void(_fastcall* _shakeCamera)(float strength, RE::NiPoint3 source, float duration);
static REL::Relocation<_shakeCamera> shakeCamera{ RELOCATION_ID(32275, 33012) };



void hitStop::stopSGTM(float stopTime, float stopSpeed, RE::Actor* a_actor) {
	if (a_actor->AsMagicTarget()->HasEffectWithArchetype(RE::MagicTarget::Archetype::kSlowTime)) {
		return;
	}
	stopSGTM_time = stopTime;
	stopSGTM_ongoing = true;
	SGTM(stopSpeed);
}

void hitStop::revertSGTM() {
	SGTM(1);
}

void hitStop::update()
{
	if (settings::currFramework != dataHandler::stopMethod::SGTM) {
		return;
	}
	if (stopSGTM_ongoing) {
		if (stopSGTM_time <= 0) {
			revertSGTM();
			stopSGTM_ongoing = false;
			return;
		}
		stopSGTM_time -= *Utils::g_deltaTime;
	}
}

void hitStop::calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType)
{
	RE::WEAPON_TYPE wpnType = weapon->GetWeaponType();
	stop(getStopTime(wpnType, stopType, isPowerAtk), getStopSpeed(wpnType, stopType, isPowerAtk), hitter);
	//FIXME: fix this quick workaround
	if (hitter->IsPlayerRef() && settings::pcShake) {
		if (!isPowerAtk) {
			if (!settings::shakeOnLight) {
				return;
			}
		}
		switch (stopType) {
		case STOPTYPE::objectStop:
			if (settings::shakeOnObject) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		case STOPTYPE::bashStop:
			if (settings::shakeOnBash) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		case STOPTYPE::blockedStop:
			if (settings::shakeOnBlocked) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		case STOPTYPE::creatureStop:
			if (settings::shakeOnCreature) {
				shakeCamera(getShakeMagnitude(wpnType, stopType, isPowerAtk), hitter->GetPosition(), getShakeTime(wpnType, stopType, isPowerAtk)); break;
			}
		}
	}


}


/*call this to start a new hitstop. Also checks if a_actor is in the process of hitstop. 
@param a_actor: the actor who will experience hitstop effect.*/
void hitStop::stop(float stopTime, float stopSpeed, RE::Actor* a_actor) {
	using method = dataHandler::stopMethod;
	switch (settings::currFramework) {
	case method::AnimSpeed:
		AnimSpeedManager::setAnimSpeed(a_actor->GetHandle(), stopSpeed, stopTime); break;
	case method::SGTM: 
		if (a_actor->IsPlayerRef()) {
			stopSGTM(stopTime, stopSpeed, a_actor);
		}
		break; //SGTM Stop only happens to player. A check is placed in onHitEventHandler to make sure it doesn't happen to NPC.
	}
}

