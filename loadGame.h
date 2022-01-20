#pragma once
#include "onHitEventHandler.h"
#include "dataHandler.h"

class loadGame
{
public:

	static void EventCallBACK(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
			INFO("data loaded, initializing...");
			onHitEventHandler::Register();
			dataHandler::readSettings();
			INFO("initialization complete!");
		}
	}

};

