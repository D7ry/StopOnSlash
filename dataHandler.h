#pragma once
#include "SimpleIni.h"
#include <iostream>
#include <cctype>


class dataHandler
{
public:

	enum combatFrameWork
	{
		Vanilla = 0,
		SkySa = 1,
		ABR = 2,
		MCO = 3
	};

	static combatFrameWork currFrameWork;

	static void readSettings();

	static bool stopOnCreature;

	static bool stopOnObject;

	static bool stopOnDead;

	static bool stopOnBash;

	static bool stopOnBlocked;

};


