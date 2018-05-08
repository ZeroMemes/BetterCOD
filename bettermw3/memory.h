#pragma once

#include <Windows.h>

namespace Memory {

	bool Read(HANDLE process, DWORD_PTR address, LPVOID buffer, DWORD size);

	bool Write(HANDLE process, DWORD_PTR address, LPVOID buffer, DWORD size);

	template<typename T>
	T Read(HANDLE process, DWORD_PTR address) {
		T value;
		Read(process, address, &value, sizeof(T));
		return value;
	}

	template<typename T>
	bool Write(HANDLE process, DWORD_PTR address, T& value) {
		return Write(process, address, &value, sizeof(value));
	}
}