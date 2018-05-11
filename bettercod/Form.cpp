#include "Form.h"

void BetterCODForm::InitializeBetterCOD()
{
	cli::array<System::String^>^ Adapters = gcnew cli::array<System::String^>(CODAdapter::Adapters->size());
	for (unsigned int i = 0; i < CODAdapter::Adapters->size(); i++)
	{
		Adapters[i] = gcnew System::String(CODAdapter::Adapters->at(i)->GameName);
	}
	this->ComboBoxGame->Items->AddRange(Adapters);
	this->ComboBoxGame->SelectedIndex = 0;
	this->ComboBoxGame->DropDownStyle = ComboBoxStyle::DropDownList;
	this->LoadSettings();

	// Register selected index changed listener here to prevent calls before it should be called
	this->ComboBoxGame->SelectedIndexChanged += gcnew System::EventHandler(this, &BetterCODForm::ComboBoxGame_SelectedIndexChanged);
}

void BetterCODForm::LoadSettings()
{
	// Retrieve the AppData/Roaming directory
	wchar_t* ppszPath;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &ppszPath);

	// Append the settings path to a new stream
	std::wstringstream ss;
	ss << ppszPath << L"\\BetterCOD\\Settings.ini";

	std::wstring ini = ss.str();

	if (!PathFileExists(ini.c_str()))
	{
		this->SaveSettings();
		return;
	}

	int selected = GetPrivateProfileInt(L"General", L"CurrentGame", 0, ini.c_str());
	if (selected >= 0 && selected <= (int) CODAdapter::Adapters->size())
	{
		this->ComboBoxGame->SelectedIndex = selected;
	}

	this->TrackBarFov->Value = GetPrivateProfileInt(this->CodAdapter->GameName, L"Fov", 90, ini.c_str());
	this->TrackBarFps->Value = GetPrivateProfileInt(this->CodAdapter->GameName, L"Fps", 85, ini.c_str());
	this->UpdateTrackBarLabels();
}

void BetterCODForm::SaveSettings()
{
	// Retrieve the AppData/Roaming directory
	wchar_t* ppszPath;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &ppszPath);

	// Append the path to the settings directory to a new stream
	std::wstringstream ss;
	ss << ppszPath << L"\\BetterCOD";

	// Create the directory
	CreateDirectory(ss.str().c_str(), NULL);

	// Append the file path to the stream
	ss << L"\\Settings.ini";

	std::wstring ini = ss.str();

	WritePrivateProfileString(L"General", L"CurrentGame", std::to_wstring(this->ComboBoxGame->SelectedIndex).c_str(), ini.c_str());
	WritePrivateProfileString(this->CodAdapter->GameName, L"Fov", std::to_wstring(this->TrackBarFov->Value).c_str(), ini.c_str());
	WritePrivateProfileString(this->CodAdapter->GameName, L"Fps", std::to_wstring(this->TrackBarFps->Value).c_str(), ini.c_str());
}

System::Void BetterCODForm::Form_Closed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	this->Timer->Stop();
	this->RestoreGameState();
	this->SaveSettings();
}

System::Void BetterCODForm::TrackBarFov_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	this->WriteFov((float) this->TrackBarFov->Value);
	this->UpdateTrackBarLabels();
}

System::Void BetterCODForm::TrackBarFps_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	this->WriteFps(this->TrackBarFps->Value);
	this->UpdateTrackBarLabels();
}

System::Void BetterCODForm::ButtonLaunch_Click(System::Object^ sender, System::EventArgs^ e)
{
	std::wstringstream str;
	str << "steam://rungameid/";
	str << this->CodAdapter->SteamAppID;
	ShellExecute(NULL, NULL, str.str().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

System::Void BetterCODForm::ComboBoxGame_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	this->SaveSettings();
	this->RestoreGameState();
	this->InvalidateCurrentGame();
	this->CodAdapter = CODAdapter::Adapters->at(this->ComboBoxGame->SelectedIndex);
	this->LoadSettings();
}

System::Void BetterCODForm::Timer_Tick(System::Object^ sender, System::EventArgs^ e)
{
	DWORD pid;
	bool ProcessOpen = Process::FindProcess(this->CodAdapter->ProcessName, pid);

	this->ButtonLaunch->Enabled = !ProcessOpen;

	if (ProcessOpen && !this->CodProcess)
	{
		HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pid);
		this->CodProcess = new Process(handle);
	}
	else if (!ProcessOpen)
	{
		this->InvalidateCurrentGame();
		return;
	}

	// Get the FOV address
	if (!this->AddressFov)
	{
		DWORD_PTR address = this->CodProcess->Read<DWORD_PTR>(this->CodAdapter->PointerFOV) + 0xC;
		// Make sure the address points to the right location
		if (this->CodProcess->Read<float>(address) == (float) this->TrackBarFov->Minimum)
		{
			this->AddressFov = address;
		}
	}

	// Get the FPS address
	if (!this->AddressFps)
	{
		DWORD_PTR address = this->CodProcess->Read<DWORD_PTR>(this->CodAdapter->PointerFPS) + 0xC;
		// Make sure the address points to the right location
		if (this->CodProcess->Read<int>(address) == this->TrackBarFps->Minimum)
		{
			this->AddressFps = address;
		}
	}

	// Write the set FoV and Max FPS
	this->WriteFov((float) this->TrackBarFov->Value);
	this->WriteFps(this->TrackBarFps->Value);
}

void BetterCODForm::UpdateTrackBarLabels()
{
	this->LabelFovValue->Text = this->TrackBarFov->Value.ToString();
	this->LabelFpsValue->Text = this->TrackBarFps->Value.ToString();
}

void BetterCODForm::WriteFov(float value)
{
	if (this->CodProcess && this->AddressFov)
	{
		this->CodProcess->Write<float>(this->AddressFov, value);
	}
}

void BetterCODForm::WriteFps(int value)
{
	if (this->CodProcess && this->AddressFps)
	{
		this->CodProcess->Write<int>(this->AddressFps, value);
	}
}

void BetterCODForm::RestoreGameState()
{
	this->WriteFov((float) this->TrackBarFov->Minimum);
	this->WriteFps(this->TrackBarFps->Minimum);
}

void BetterCODForm::InvalidateCurrentGame()
{
	this->AddressFps = 0;
	this->AddressFps = 0;
	this->CodProcess = nullptr;
}
