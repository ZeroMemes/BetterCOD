#pragma once

#include <Windows.h>

namespace CODAdapter
{
	struct Adapter
	{
		char*     GameName;
		wchar_t*  ProcessName;
		char*     SteamAppID;
		DWORD_PTR PointerFOV;
		DWORD_PTR PointerFPS;
	};

	extern Adapter* MW3;
}
