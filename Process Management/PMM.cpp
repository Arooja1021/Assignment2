#pragma once

#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>
#include <conio.h>
#include <chrono>
#include <thread>

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
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	PMMPtr->Ready.Flags.ProcessManagement = 1;

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
	int LaserCrash = 0;
	int GNSSCrash = 0;
	int ControllerCrash = 0;
	int VCCrash = 0;
	int DisplayCrash = 0;




	return SUCCESS;
}


error_state ProcessManagement::startupProcesses() {

	ProcessList = gcnew array<WeederProcessess>
	{
		{"Laser", true, 0, 10, gcnew Process},
		{ "GNSS", true, 0, 10, gcnew Process },
		{ "Controller", true, 0, 10, gcnew Process },
		{ "Vehicle Control", true, 0, 10, gcnew Process },
		{ "Display", true, 0, 10, gcnew Process },

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
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	if (PMMPtr->Heartbeat.Flags.Laser) {
		LaserCrash++;
	}
	else {
		LaserCrash = 0;
		PMMPtr->Heartbeat.Flags.Laser = 1;
	}
	if (PMMPtr->Heartbeat.Flags.GPS) {
		GNSSCrash++;
	}
	else {
		GNSSCrash = 0;
		PMMPtr->Heartbeat.Flags.GPS = 1;
	}
	if (PMMPtr->Heartbeat.Flags.Controller) {
		ControllerCrash++;
	}
	else {
		ControllerCrash = 0;
		PMMPtr->Heartbeat.Flags.Controller = 1;
	}
	if (PMMPtr->Heartbeat.Flags.VehicleControl) {
		VCCrash++;
	}
	else {
		VCCrash = 0;
		PMMPtr->Heartbeat.Flags.VehicleControl = 1;
	}
	if (PMMPtr->Heartbeat.Flags.Display) {
		DisplayCrash++;
	}
	else {
		DisplayCrash = 0;
		PMMPtr->Heartbeat.Flags.Display = 1;
	}

	if (LaserCrash > 100) {
		shutdownModules();
	}
	if (GNSSCrash > 100) {
		ProcessList[1].ProcessName->Start();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	if (ControllerCrash > 100) {
		shutdownModules();
	}
	if (VCCrash > 100) {
		shutdownModules();
	}
	if (DisplayCrash > 100) {
		shutdownModules();
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	return SUCCESS;
}

// Signal shutdown of all processes on normal termination
void ProcessManagement::shutdownModules() {

	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;

	
	PMMPtr->Shutdown.Flags.Laser = 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	PMMPtr->Shutdown.Flags.GPS = 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	PMMPtr->Shutdown.Flags.Controller = 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	PMMPtr->Shutdown.Flags.VehicleControl = 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	PMMPtr->Shutdown.Flags.Display = 1;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	PMMPtr->Shutdown.Flags.ProcessManagement = 1;


}

// Get Shutdown signal for module, from Process Management SM
bool ProcessManagement::getShutdownFlag() {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;


	
	return PMMPtr->Shutdown.Flags.ProcessManagement;
}

// Update heartbeat signal for process management
error_state ProcessManagement::setHeartbeat(bool heartbeat) {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	PMMPtr->Heartbeat.Flags.ProcessManagement = heartbeat;
	return SUCCESS;
}
