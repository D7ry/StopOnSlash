#pragma once
#include "onHitEventHandler.h"
#include "dataHandler.h"

class loadGame
{
public:

	static void EventCallBACK(SKSE::MessagingInterface::Message* msg)
	{
		switch (msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			INFO("data loaded, read settings...");
			onHitEventHandler::Register();
			dataHandler::readSettings();
			dataHandler::initGlobals();
			INFO("...done.");
			break;
		case SKSE::MessagingInterface::kPostLoadGame:
			INFO("Post load; update globals...");
			dataHandler::updateGlobals();
			INFO("...done.");
		}

	}

};

