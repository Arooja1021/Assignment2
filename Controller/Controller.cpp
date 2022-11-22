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
	return SUCCESS;
}

bool Controller::getShutdownFlag() {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	return PMMPtr->Shutdown.Flags.Controller;
}

error_state Controller::setHeartbeat(bool heartbeat) {
	return SUCCESS;
}

error_state Controller::getControlData() {

	ControllerInterface* contInt = new ControllerInterface(1, 1);
	controllerState contState = contInt->GetState();
	double speed = contState.rightTrigger - contState.leftTrigger;
	double steer = -contState.rightThumbX * 40;
	SM_VehicleControl* VCPtr = (SM_VehicleControl*)ControllerData;
	VCPtr->Speed = speed;
	VCPtr->Steering = steer;

	Console::WriteLine("speed: {0}\tsteer : {1}", speed, steer);


	return SUCCESS;
}