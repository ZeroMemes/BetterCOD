#include "Form.h"

System::Void BetterMW3Form::Form_Closed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	this->Timer->Stop();
	this->WriteFov((float) this->TrackBarFov->Minimum);
	this->WriteFps(this->TrackBarFps->Minimum);
}

System::Void BetterMW3Form::TrackBarFov_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	this->LabelFovValue->Text = this->TrackBarFov->Value.ToString();
	this->WriteFov((float) this->TrackBarFov->Value);
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
	DWORD pid;
	bool ProcessOpen = Process::FindProcess(L"iw5mp.exe", pid);

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
		// iw5mp.exe + 0x70C738
		this->AddressFov = this->CodProcess->Read<DWORD_PTR>(0xB0C738) + 0xC;
	}

	// Get the FPS address
	if (!this->AddressFps)
	{
		// iw5mp.exe + 0x18F0B84
		this->AddressFps = this->CodProcess->Read<DWORD_PTR>(0x1CF0B84) + 0xC;
	}

	// Write the set FoV and Max FPS
	this->WriteFov((float) this->TrackBarFov->Value);
	this->WriteFps(this->TrackBarFps->Value);
}

void BetterMW3Form::WriteFov(float value)
{
	if (this->CodProcess && this->AddressFov)
	{
		this->CodProcess->Write<float>(this->AddressFov, value);
	}
}

void BetterMW3Form::WriteFps(int value)
{
	if (this->CodProcess && this->AddressFps)
	{
		this->CodProcess->Write<int>(this->AddressFps, value);
	}
}
