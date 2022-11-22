#include <VC.h>
#include <smstructs.h>
#include <SMObject.h>


#define zid 5309962

using namespace System;
using namespace System::Diagnostics;
using namespace Net;
using namespace Sockets;
using namespace Text;

error_state VC::connect(String^ hostName, int portNumber) {


	Client = gcnew TcpClient(hostName, portNumber);


	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;
	Client->SendTimeout = 500;
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;


	auto SendData = gcnew array<unsigned char>(1024);
	ReadData = gcnew array<unsigned char>(1024);
	auto Message = gcnew String("");

	Message = zid + "\n";

	SendData = Encoding::ASCII->GetBytes(Message);


	Stream = Client->GetStream();
	Stream->Write(SendData, 0, SendData->Length);
	Threading::Thread::Sleep(50);

	return SUCCESS;
}

error_state VC::communicate() {
	SM_VehicleControl* VCPtr = (SM_VehicleControl*)ControllerData;
	flag = (int)!flag;
	auto SendData = gcnew array<unsigned char>(1024);
	auto Message = gcnew String("");
	Message = "# " + VCPtr->Steering + " " + VCPtr->Speed + " " + (int)flag + " #";
	SendData = Encoding::ASCII->GetBytes(Message);
	Stream->Write(SendData, 0, SendData->Length);
	Console::WriteLine(Message);
	System::Threading::Thread::Sleep(100);
	return SUCCESS;
}

error_state VC::setupSharedMemory() {
	SMObject* PMObj = new SMObject(TEXT("PMMObj"), sizeof(SM_ProcessManagement));

	if (PMObj->SMAccess() == SM_ACCESS_ERROR) {
		Console::WriteLine("Access Error");
	}
	ProcessManagementData = (SMObject*)PMObj->pData;


	SMObject* VCObj = new SMObject(TEXT("VCObj"), sizeof(SM_VehicleControl));

	if (VCObj->SMAccess() == SM_ACCESS_ERROR) {
		Console::WriteLine("Access Error");
	}
	ControllerData = (SMObject*)VCObj->pData;



	return SUCCESS;
}

error_state VC::processSharedMemory() {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	bool ready = PMMPtr->Ready.Flags.ProcessManagement;
	bool heartbeat = PMMPtr->Heartbeat.Flags.ProcessManagement;
	if (!ready || heartbeat) {
		PMMPtr->Heartbeat.Flags.ProcessManagement = 0;
		crashTimer = 0;
	}
	else {
		crashTimer++;
		Threading::Thread::Sleep(50);
	}


	if (crashTimer > 30) {
		PMMPtr->Shutdown.Status = 0xFF;
	}

	return SUCCESS;
}

bool VC::getShutdownFlag() {

	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;


	return PMMPtr->Shutdown.Flags.VehicleControl;


}

error_state VC::setHeartbeat(bool heartbeat) {

	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	PMMPtr->Heartbeat.Flags.VehicleControl = heartbeat;
	return SUCCESS;
}


