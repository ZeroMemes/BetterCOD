#include "memory.h"

namespace Memory {

	bool Read(HANDLE process, DWORD_PTR address, LPVOID buffer, DWORD size) {
		return ReadProcessMemory(process, (LPCVOID) address, buffer, size, NULL) == TRUE;
	}

	bool Write(HANDLE process, DWORD_PTR address, LPVOID buffer, DWORD size) {
		return WriteProcessMemory(process, (LPVOID) address, buffer, size, NULL) == TRUE;
	}
}