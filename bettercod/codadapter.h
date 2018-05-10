#pragma once

#include <Windows.h>
#include <string>
#include <sstream>

namespace CODAdapter
{
	struct Adapter
	{
		std::string  GameName;
		std::wstring ProcessName;
		std::string  SteamAppID;
		DWORD_PTR    PointerFOV;
		DWORD_PTR    PointerFPS;
	};

	extern Adapter* MW3;
}
