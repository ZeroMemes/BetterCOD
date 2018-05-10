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
}

System::Void BetterCODForm::Form_Closed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	this->Timer->Stop();
	this->RestoreGameState();
}

System::Void BetterCODForm::TrackBarFov_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	this->LabelFovValue->Text = this->TrackBarFov->Value.ToString();
	this->WriteFov((float) this->TrackBarFov->Value);
}

System::Void BetterCODForm::TrackBarFps_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	this->LabelFpsValue->Text = this->TrackBarFps->Value.ToString();
	this->WriteFps(this->TrackBarFps->Value);
}

System::Void BetterCODForm::ButtonLaunch_Click(System::Object^ sender, System::EventArgs^ e)
{
	std::stringstream str;
	str << "start steam://rungameid/";
	str << this->CodAdapter->SteamAppID;
	system(str.str().c_str());
}

System::Void BetterCODForm::ComboBoxGame_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	this->RestoreGameState();
	this->InvalidateCurrentGame();
	this->CodAdapter = CODAdapter::Adapters->at(this->ComboBoxGame->SelectedIndex);
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
