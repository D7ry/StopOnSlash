#pragma once
#include "SimpleIni.h"
#include <iostream>
#include <cctype>


class dataHandler
{
public:

	enum combatFrameWork
	{
		SkySa = 0,
		ABR = 1,
		MCO = 2
	};

	static combatFrameWork currFrameWork;

	static void readSettings();

	static bool stopOnCreature;

	static bool stopOnObject;

	static bool stopOnDead;

	static bool stopOnBash;

	static bool stopOnBlocked;

};


