
#include "Papyrus.h"

#include "dataHandler.h"

namespace Papyrus
{
	void StopOnSlash_MCM::OnConfigClose(RE::TESQuest*)
	{
		dataHandler::readSettings();
	}

	bool StopOnSlash_MCM::Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("OnConfigClose", "StopOnSlash_MCM", OnConfigClose);
		return true;
	}

	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();
		papyrus->Register(StopOnSlash_MCM::Register);
	}
}
