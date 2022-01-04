
#include "Papyrus.h"

#include "dataHandler.h"

namespace Papyrus
{
	void StopOnSlash_MCM::OnConfigClose(RE::TESQuest*)
	{
		dataHandler::GetSingleton()->readSettings();
	}

	bool StopOnSlash_MCM::Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("OnConfigClose", "StopOnSlash_MCM", OnConfigClose);

		INFO("Registered SOS MCM class");
		return true;
	}

	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();
		papyrus->Register(StopOnSlash_MCM::Register);
		INFO("Registered papyrus functions");
	}
}