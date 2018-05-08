#include "Form.h"

System::Void BetterMW3Form::Form_Closed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	this->Timer->Stop();
	this->WriteFov(this->TrackBarFov->Minimum);
	this->WriteFps(this->TrackBarFps->Minimum);
	CloseHandle(this->pHandle);
}

System::Void BetterMW3Form::TrackBarFov_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	this->LabelFovValue->Text = this->TrackBarFov->Value.ToString();
	this->WriteFov(this->TrackBarFov->Value);
}

System::Void BetterMW3Form::TrackBarFps_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	this->LabelFpsValue->Text = this->TrackBarFps->Value.ToString();
	this->WriteFps(this->TrackBarFps->Value);
}

System::Void BetterMW3Form::ButtonLaunch_Click(System::Object^ sender, System::EventArgs^ e)
{
	system("start steam://rungameid/42690");
}

System::Void BetterMW3Form::Timer_Tick(System::Object^ sender, System::EventArgs^ e)
{
	bool ProcessOpen = FindProcess();

	this->ButtonLaunch->Enabled = !ProcessOpen;

	if (!ProcessOpen && this->pHandle)
	{
		CloseHandle(this->pHandle);
		this->addressFov = 0;
		this->addressFps = 0;
		this->pHandle = 0;
		return;
	}

	DWORD address;

	// Get the FOV address
	if (!this->addressFov && ReadProcessMemory(pHandle, (LPCVOID) 0xB0C738, &address, 4, NULL) && address > 0)
	{
		this->addressFov = address + 0xC;
	}

	// Get the FPS address
	if (!this->addressFps && ReadProcessMemory(pHandle, (LPCVOID) 0x1CF0B84, &address, 4, NULL) && address > 0)
	{
		this->addressFps = address + 0xC;
	}

	// Write the set FoV and Max FPS
	this->WriteFov(this->TrackBarFov->Value);
	this->WriteFps(this->TrackBarFps->Value);
}

bool BetterMW3Form::FindProcess()
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(snapshot, &entry))
	{
		while (Process32Next(snapshot, &entry))
		{
			if (!wcscmp(entry.szExeFile, L"iw5mp.exe"))
			{
				if (!this->pHandle)
				{
					this->pHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, entry.th32ProcessID);
				}
				return true;
			}
		}
	}

	CloseHandle(snapshot);
	return false;
}

void BetterMW3Form::WriteFov(int value)
{
	if (this->pHandle && this->addressFov)
	{
		WriteProcessMemory(pHandle, (LPVOID) this->addressFov, &value, 4, NULL);
	}
}

void BetterMW3Form::WriteFps(int value)
{
	if (this->pHandle && this->addressFps)
	{
		WriteProcessMemory(this->pHandle, (LPVOID) this->addressFps, &value, 4, NULL);
	}
}