#pragma once

#include "datasource.h"

class Module;

class Process : public DataSource
{
public:
	Process(HANDLE handle) : Handle(handle)
	{
		FindModules();
	}

	bool ReadMemory(DWORD_PTR address, LPVOID buffer, DWORD size) override;
	bool WriteMemory(DWORD_PTR address, LPVOID buffer, DWORD size) override;

	HANDLE   GetHandle();
	Module*  GetModule(wchar_t* name);
	Module*  GetBaseModule();

	static bool Process::FindProcess(wchar_t* name, DWORD& pidOut);
protected:
	HANDLE    Handle;
	std::vector<Module*> Modules;

private:
	void    FindModules();
	Module* FindModule(wchar_t* name);
};
