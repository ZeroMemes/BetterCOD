#include "module.h"
#include "process.h"

bool Module::ReadMemory(DWORD_PTR address, LPVOID buffer, DWORD size)
{
	return this->Process->ReadMemory(this->Pointer + address, buffer, size);
}

bool Module::WriteMemory(DWORD_PTR address, LPVOID buffer, DWORD size)
{
	return this->Process->WriteMemory(this->Pointer + address, buffer, size);
}

Process* Module::GetProcess()
{
	return this->Process;
}

wchar_t* Module::GetName()
{
	return this->Name;
}

DWORD_PTR Module::GetPointer()
{
	return this->Pointer;
}

DWORD Module::GetSize()
{
	return this->Size;
}
