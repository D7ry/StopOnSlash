#pragma once
#include "SimpleIni.h"
#include <iostream>
#include <cctype>


class dataHandler
{
public:
	static dataHandler* GetSingleton() {
		static dataHandler singleton;
		return std::addressof(singleton);
	}

	enum combatFrameWork
	{
		Vanilla = 0,
		ASF = 1,
		MCO = 2,
		STGM = 3
	};

	combatFrameWork currFrameWork;

	void readSettings();

	bool NPCstop = true;

	bool stopOnCreature = true;

	bool stopOnObject = true;

	bool stopOnDead = true;

	bool stopOnBash = true;

	bool stopOnBlocked = true;

private:
	void ReadBoolSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
	void ReadIntSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, int& a_setting);
};


