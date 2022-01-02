
#include "Papyrus.h"

#include "dataHandler.h"

namespace Papyrus
{
	void hitStopRedux_MCM::OnConfigClose(RE::TESQuest*)
	{
		dataHandler::GetSingleton()->readSettings();
	}

	bool hitStopRedux_MCM::Register(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("OnConfigClose", "hitStopRedux_MCM", OnConfigClose);

		INFO("Registered hitStopRedux MCM class");
		return true;
	}

	void Register()
	{
		auto papyrus = SKSE::GetPapyrusInterface();
		papyrus->Register(hitStopRedux_MCM::Register);
		INFO("Registered papyrus functions");
	}
}