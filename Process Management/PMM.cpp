

#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>

using namespace System;

using namespace System::Diagnostics;


error_state ProcessManagement::setupSharedMemory() {
	SMObject PMObj(TEXT("PMMObj"), sizeof(SM_ProcessManagement));
	PMObj.SMCreate();
	PMObj.SMAccess();
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
	SMObject PMObj(TEXT("PMObj"), sizeof(SM_ProcessManagement));
	SM_ProcessManagement *PMMPtr;
	PMObj.SMAccess();
	PMMPtr = (SM_ProcessManagement*)PMObj.pData;
	
	

	PMMPtr->Shutdown.Status = 0x00;

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
