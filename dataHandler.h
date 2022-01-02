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
		SkySa = 1,
		ABR = 2,
		MCO = 3
	};

	combatFrameWork currFrameWork;

	void readSettings();

	bool stopOnCreature;

	bool stopOnObject;

	bool stopOnDead;

	bool stopOnBash;

	bool stopOnBlocked;

};


