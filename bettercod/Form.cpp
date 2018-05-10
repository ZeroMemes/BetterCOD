#include "Form.h"

System::Void BetterCODForm::Form_Closed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	this->Timer->Stop();
	this->WriteFov((float) this->TrackBarFov->Minimum);
	this->WriteFps(this->TrackBarFps->Minimum);
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

System::Void BetterCODForm::Timer_Tick(System::Object^ sender, System::EventArgs^ e)
{
	DWORD pid;
	bool ProcessOpen = Process::FindProcess(this->CodAdapter->ProcessName.c_str(), pid);

	this->ButtonLaunch->Enabled = !ProcessOpen;

	if (ProcessOpen && !this->CodProcess)
	{
		HANDLE handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pid);
		this->CodProcess = new Process(handle);
	}
	else if (!ProcessOpen)
	{
		this->AddressFps = 0;
		this->AddressFps = 0;
		this->CodProcess = nullptr;
		return;
	}

	// Get the FOV address
	if (!this->AddressFov)
	{
		this->AddressFov = this->CodProcess->Read<DWORD_PTR>(this->CodAdapter->PointerFOV) + 0xC;
	}

	// Get the FPS address
	if (!this->AddressFps)
	{
		this->AddressFps = this->CodProcess->Read<DWORD_PTR>(this->CodAdapter->PointerFPS) + 0xC;
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
