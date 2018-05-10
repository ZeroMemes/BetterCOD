#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

class DataSource
{
protected:
	virtual bool ReadMemory(DWORD_PTR address, LPVOID buffer, DWORD size) = 0;
	virtual bool WriteMemory(DWORD_PTR address, LPVOID buffer, DWORD size) = 0;

public:
	template<typename T>
	T Read(DWORD_PTR address)
	{
		T value;
		this->ReadMemory(address, &value, sizeof(T));
		return value;
	}

	template<typename T>
	bool Write(DWORD_PTR address, T& value)
	{
		return this->WriteMemory(address, &value, sizeof(value));
	}
};
