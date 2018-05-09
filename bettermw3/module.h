#pragma once

#include "datasource.h"

class Process;

class Module : public DataSource
{
public:
	Module(Process* process, wchar_t* name, DWORD_PTR pointer, DWORD size)
		: Process(process), Name(name), Pointer(pointer), Size(size) {}

	bool ReadMemory(DWORD_PTR address, LPVOID buffer, DWORD size) override;
	bool WriteMemory(DWORD_PTR address, LPVOID buffer, DWORD size) override;

	Process*  GetProcess();
	wchar_t*  GetName();
	DWORD_PTR GetPointer();
	DWORD     GetSize();

protected:
	Process*  Process;
	wchar_t*  Name;
	DWORD_PTR Pointer;
	DWORD     Size;
};
