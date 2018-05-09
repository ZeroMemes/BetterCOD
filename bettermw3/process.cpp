#include "process.h"
#include "module.h"

bool Process::ReadMemory(DWORD_PTR address, LPVOID buffer, DWORD size)
{
	return ReadProcessMemory(this->Handle, (LPCVOID) address, buffer, size, NULL) == TRUE;
}

bool Process::WriteMemory(DWORD_PTR address, LPVOID buffer, DWORD size)
{
	return WriteProcessMemory(this->Handle, (LPVOID) address, buffer, size, NULL) == TRUE;
}

HANDLE Process::GetHandle()
{
	return this->Handle;
}

Module* Process::GetModule(wchar_t* name)
{
	if (this->Modules.empty())
		return nullptr;

	Module* module = FindModule(name);
	
	// If we were unable to find the module, refresh the list and try to find it again
	if (!module)
	{
		FindModules();
		module = FindModule(name);
	}

	return module;
}

Module* Process::FindModule(wchar_t* name)
{
	for (Module* m : this->Modules)
	{
		if (!wcscmp(name, m->GetName()))
		{
			return m;
		}
	}
	return nullptr;
}

void Process::FindModules()
{
	// Delete all existing Modules
	for (Module* m : this->Modules)
	{
		delete m;
	}

	// Clear all modules
	this->Modules.clear();

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, (DWORD) this->Handle);

	MODULEENTRY32 entry;
	entry.dwSize = sizeof(entry);

	if (Module32First(snapshot, &entry))
	{
		do
		{
			Module* m = new Module(this, entry.szModule, (DWORD_PTR) entry.hModule, entry.modBaseSize);
			this->Modules.emplace_back(m);
		}
		while (Module32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);
}

bool Process::FindProcess(wchar_t* name, DWORD& pidOut)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	if (Process32First(snapshot, &entry))
	{
		do
		{
			if (!wcscmp(entry.szExeFile, name))
			{
				pidOut = entry.th32ProcessID;
				CloseHandle(snapshot);
				return true;
			}
		}
		while (Process32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return false;
}
