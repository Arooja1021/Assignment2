#pragma once

#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>
#include <conio.h>

using namespace System;

using namespace System::Diagnostics;


error_state ProcessManagement::setupSharedMemory() {
	SMObject* PMObj = new SMObject(TEXT("PMMObj"), sizeof(SM_ProcessManagement));


	if (PMObj->SMCreate() == SM_CREATE_ERROR) {
		Console::WriteLine("Create Error");
	}

	if (PMObj->SMAccess() == SM_ACCESS_ERROR) {
		Console::WriteLine("Access Error");
	}
	ProcessManagementData = (SMObject*)PMObj->pData;


	SMObject* LaserObj = new SMObject(TEXT("LaserObj"), sizeof(SM_Laser));


	if (LaserObj->SMCreate() == SM_CREATE_ERROR) {
		Console::WriteLine("Create Error");
	}


	SMObject* GPSObj = new SMObject(TEXT("GPSObj"), sizeof(SM_GPS));


	if (GPSObj->SMCreate() == SM_CREATE_ERROR) {
		Console::WriteLine("Create Error");
	}

	SMObject* VCObj = new SMObject(TEXT("VCObj"), sizeof(SM_VehicleControl));


	if (VCObj->SMCreate() == SM_CREATE_ERROR) {
		Console::WriteLine("Create Error");
	}





	return SUCCESS;
}


error_state ProcessManagement::startupProcesses() {

	array<WeederProcessess>^ ProcessList = gcnew array<WeederProcessess>
	{
		{"Laser", true, 0, 10, gcnew Process},
		{ "GNSS", true, 0, 10, gcnew Process },
		{ "Controller", true, 0, 10, gcnew Process },
		{ "Display", true, 0, 10, gcnew Process },
		{ "Vehicle Control", true, 0, 10, gcnew Process },

	};

	for (int i = 0; i < ProcessList->Length; i++)
	{
		if (Process::GetProcessesByName(ProcessList[i].ModuleName)->Length == 0)
		{

			//ProcessList[i].ProcessName->StartInfo->WorkingDirectory = "C:\\Users\\z5309962\\source\\repos\\Arooja1021\\Assignment2\\executables";
			ProcessList[i].ProcessName->StartInfo->WorkingDirectory = "C:\\Users\\Haoru\\source\\repos\\Arooja1021\\Assignment2\\executables";

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

	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;

	
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
