#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include "process.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Data;
using namespace System::Drawing;

public ref class BetterMW3Form : public Form
{

public:
	BetterMW3Form(void)
	{
		InitializeComponent();
	}

protected:
	~BetterMW3Form()
	{
		if (components)
		{
			delete components;
		}
	}
private:
	// UI Components
	System::Windows::Forms::TrackBar^   TrackBarFov;
	System::Windows::Forms::TrackBar^   TrackBarFps;
	System::Windows::Forms::Label^      LabelFov;
	System::Windows::Forms::Label^      LabelFovValue;
	System::Windows::Forms::Label^      LabelFps;
	System::Windows::Forms::Label^      LabelFpsValue;
	System::Windows::Forms::Label^      LabelCredit;
	System::Windows::Forms::Button^     ButtonLaunch;
	System::Windows::Forms::Timer^      Timer;
	System::ComponentModel::IContainer^ components;

	// Memory Stuff
	Process*  CodProcess = nullptr;
	DWORD_PTR AddressFov;
	DWORD_PTR AddressFps;

private:
	void InitializeComponent(void)
	{
		this->components = (gcnew System::ComponentModel::Container());
		this->TrackBarFov = (gcnew System::Windows::Forms::TrackBar());
		this->LabelFps = (gcnew System::Windows::Forms::Label());
		this->TrackBarFps = (gcnew System::Windows::Forms::TrackBar());
		this->LabelFov = (gcnew System::Windows::Forms::Label());
		this->LabelFovValue = (gcnew System::Windows::Forms::Label());
		this->LabelFpsValue = (gcnew System::Windows::Forms::Label());
		this->LabelCredit = (gcnew System::Windows::Forms::Label());
		this->ButtonLaunch = (gcnew System::Windows::Forms::Button());
		this->Timer = (gcnew System::Windows::Forms::Timer(this->components));
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackBarFov))->BeginInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackBarFps))->BeginInit();
		this->SuspendLayout();
		// 
		// TrackBarFov
		// 
		this->TrackBarFov->Location = System::Drawing::Point(46, 12);
		this->TrackBarFov->Maximum = 100;
		this->TrackBarFov->Minimum = 65;
		this->TrackBarFov->Name = L"TrackBarFov";
		this->TrackBarFov->Size = System::Drawing::Size(253, 45);
		this->TrackBarFov->TabIndex = 0;
		this->TrackBarFov->TickFrequency = 5;
		this->TrackBarFov->Value = 65;
		this->TrackBarFov->Scroll += gcnew System::EventHandler(this, &BetterMW3Form::TrackBarFov_Scroll);
		// 
		// LabelFps
		// 
		this->LabelFps->AutoSize = true;
		this->LabelFps->Location = System::Drawing::Point(12, 16);
		this->LabelFps->Name = L"LabelFps";
		this->LabelFps->Size = System::Drawing::Size(28, 13);
		this->LabelFps->TabIndex = 1;
		this->LabelFps->Text = L"FOV";
		// 
		// TrackBarFps
		// 
		this->TrackBarFps->Location = System::Drawing::Point(46, 63);
		this->TrackBarFps->Maximum = 235;
		this->TrackBarFps->Minimum = 85;
		this->TrackBarFps->Name = L"TrackBarFps";
		this->TrackBarFps->Size = System::Drawing::Size(253, 45);
		this->TrackBarFps->TabIndex = 2;
		this->TrackBarFps->TickFrequency = 30;
		this->TrackBarFps->Value = 85;
		this->TrackBarFps->Scroll += gcnew System::EventHandler(this, &BetterMW3Form::TrackBarFps_Scroll);
		// 
		// LabelFov
		// 
		this->LabelFov->AutoSize = true;
		this->LabelFov->Location = System::Drawing::Point(12, 67);
		this->LabelFov->Name = L"LabelFov";
		this->LabelFov->Size = System::Drawing::Size(27, 13);
		this->LabelFov->TabIndex = 3;
		this->LabelFov->Text = L"FPS";
		// 
		// LabelFovValue
		// 
		this->LabelFovValue->AutoSize = true;
		this->LabelFovValue->Location = System::Drawing::Point(305, 16);
		this->LabelFovValue->Name = L"LabelFovValue";
		this->LabelFovValue->Size = System::Drawing::Size(19, 13);
		this->LabelFovValue->TabIndex = 4;
		this->LabelFovValue->Text = L"65";
		// 
		// LabelFpsValue
		// 
		this->LabelFpsValue->AutoSize = true;
		this->LabelFpsValue->Location = System::Drawing::Point(305, 67);
		this->LabelFpsValue->Name = L"LabelFpsValue";
		this->LabelFpsValue->Size = System::Drawing::Size(19, 13);
		this->LabelFpsValue->TabIndex = 5;
		this->LabelFpsValue->Text = L"85";
		// 
		// LabelCredit
		// 
		this->LabelCredit->AutoSize = true;
		this->LabelCredit->Location = System::Drawing::Point(12, 107);
		this->LabelCredit->Name = L"LabelCredit";
		this->LabelCredit->Size = System::Drawing::Size(234, 13);
		this->LabelCredit->TabIndex = 6;
		this->LabelCredit->Text = L"MW3 FOV Changer and FPS Unlocker by Brady";
		// 
		// ButtonLaunch
		// 
		this->ButtonLaunch->Location = System::Drawing::Point(252, 100);
		this->ButtonLaunch->Name = L"ButtonLaunch";
		this->ButtonLaunch->Size = System::Drawing::Size(75, 29);
		this->ButtonLaunch->TabIndex = 7;
		this->ButtonLaunch->Text = L"Launch";
		this->ButtonLaunch->UseVisualStyleBackColor = true;
		this->ButtonLaunch->Click += gcnew System::EventHandler(this, &BetterMW3Form::ButtonLaunch_Click);
		// 
		// Timer
		// 
		this->Timer->Enabled = true;
		this->Timer->Interval = 50;
		this->Timer->Tick += gcnew System::EventHandler(this, &BetterMW3Form::Timer_Tick);
		// 
		// BetterMW3Form
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(338, 136);
		this->Controls->Add(this->ButtonLaunch);
		this->Controls->Add(this->LabelCredit);
		this->Controls->Add(this->LabelFpsValue);
		this->Controls->Add(this->LabelFovValue);
		this->Controls->Add(this->LabelFov);
		this->Controls->Add(this->TrackBarFps);
		this->Controls->Add(this->LabelFps);
		this->Controls->Add(this->TrackBarFov);
		this->MaximumSize = System::Drawing::Size(354, 175);
		this->MinimumSize = System::Drawing::Size(354, 175);
		this->Name = L"BetterMW3Form";
		this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
		this->Text = L"Better MW3";
		this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &BetterMW3Form::Form_Closed);
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackBarFov))->EndInit();
		(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackBarFps))->EndInit();
		this->ResumeLayout(false);
		this->PerformLayout();

	}

	System::Void Form_Closed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
	System::Void TrackBarFov_Scroll(System::Object^ sender, System::EventArgs^ e);
	System::Void TrackBarFps_Scroll(System::Object^ sender, System::EventArgs^ e);
	System::Void ButtonLaunch_Click(System::Object^ sender, System::EventArgs^ e);
	System::Void Timer_Tick(System::Object^ sender, System::EventArgs^ e);

	void WriteFov(float value);
	void WriteFps(int value);
};