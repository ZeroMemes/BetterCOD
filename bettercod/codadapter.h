#pragma once

#include <Windows.h>
#include <vector>

namespace CODAdapter
{
	struct Adapter
	{
		wchar_t*  GameName;
		wchar_t*  ProcessName;
		char*     SteamAppID;
		DWORD_PTR PointerFOV;
		DWORD_PTR PointerFPS;
	};

	extern Adapter* MW3;
	extern Adapter* MW2;
	extern std::vector<Adapter*>* Adapters;
}
