#pragma once

#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>
#include <conio.h>

using namespace System;

using namespace System::Diagnostics;


error_state ProcessManagement::setupSharedMemory() {
	SMObject* PMObj = new SMObject(TEXT("PMMObj"), sizeof(SM_ProcessManagement));
	PMObj->SMCreate();
	//PMObj->SMAccess();
	//SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)PMObj->pData;
	if (PMObj->SMCreate() == SM_CREATE_ERROR) {
		Console::WriteLine("Error");
	}
	//PMMPtr->Shutdown.Status = 0x00;
	//Console::WriteLine(PMMPtr->Shutdown.Status);

	return SUCCESS;
}


error_state ProcessManagement::startupProcesses() {

	array<WeederProcessess>^ ProcessList = gcnew array<WeederProcessess>
	{
		{"Laser", true, 0, 10, gcnew Process},

	};

	for (int i = 0; i < ProcessList->Length; i++)
	{
		if (Process::GetProcessesByName(ProcessList[i].ModuleName)->Length == 0)
		{

			ProcessList[i].ProcessName->StartInfo->WorkingDirectory = "C:\\Users\\z5309962\\source\\repos\\Arooja1021\\Assignment2\\executables";

			ProcessList[i].ProcessName->StartInfo->FileName = ProcessList[i].ModuleName + "_{z5309962}";

			ProcessList[i].ProcessName->Start();

			Console::WriteLine("The process" + ProcessList[i].ModuleName + ".exe started");
		}
	}


	return SUCCESS;
}

// Recieve/update data from process management shared memory structure about the state of all modules
error_state ProcessManagement::processSharedMemory() {

	return SUCCESS;
}

// Signal shutdown of all processes on normal termination
void ProcessManagement::shutdownModules() {
	//SMObject PMObj(TEXT("PMObj"), sizeof(SM_ProcessManagement));
	SMObject* PMObj = new SMObject(TEXT("PMMObj"), sizeof(SM_ProcessManagement));
	
	//sSM_ProcessManagement *PMMPtr = nullptr;
	PMObj->SMAccess();
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)PMObj->pData;

	
	

	PMMPtr->Shutdown.Status = 0xFF;

	Console::WriteLine(PMMPtr->Shutdown.Status);
}

// Get Shutdown signal for module, from Process Management SM
bool ProcessManagement::getShutdownFlag() {
	return false;
}

// Update heartbeat signal for process management
error_state ProcessManagement::setHeartbeat(bool heartbeat) {
	return SUCCESS;
}
