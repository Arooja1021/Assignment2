#pragma once


#include <controllerInterface.h>
#include <Controller.h>
#include <SMObject.h>
#include <smstructs.h>
#include <networked_module.h>


using namespace System;
using namespace System::Diagnostics;


error_state Controller::setupSharedMemory() {

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

error_state Controller::processSharedMemory() {
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

bool Controller::getShutdownFlag() {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	return PMMPtr->Shutdown.Flags.Controller;
}

error_state Controller::setHeartbeat(bool heartbeat) {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	PMMPtr->Heartbeat.Flags.Controller = heartbeat;
	return SUCCESS;
}

error_state Controller::getControlData() {

	SM_VehicleControl* VCPtr = (SM_VehicleControl*)ControllerData;
	ControllerInterface* Controller = new ControllerInterface(1, 1);
	if (!Controller->IsConnected()) {
		VCPtr->Speed = 0;
		VCPtr->Steering = 0;
		return SUCCESS;
	}
	controllerState contState = Controller->GetState();
	if (contState.buttonB) {
		SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
		PMMPtr->Shutdown.Flags.ProcessManagement = 1;
		return SUCCESS;
	}
	double speed = contState.rightTrigger - contState.leftTrigger;
	double steer = -contState.rightThumbX * 40;
	
	VCPtr->Speed = speed;
	VCPtr->Steering = steer;

	Console::WriteLine("speed: {0}\tsteer : {1}", speed, steer);


	return SUCCESS;
}