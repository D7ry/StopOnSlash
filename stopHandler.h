#pragma once
class stopHandler
{
public:
	enum STOPTYPE
	{
		objectStop = 0,
		bashStop = 1,
		blockedStop = 2,
		creatureStop = 3
	};

	static void calculateStop(bool isPowerAtk, RE::Actor* hitter, RE::TESObjectWEAP* weapon, STOPTYPE stopType);
	
private:
	static float getStopSpeed(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower);
	static float getStopTime(RE::WEAPON_TYPE wpnType, STOPTYPE stopType, bool isPower);
};


