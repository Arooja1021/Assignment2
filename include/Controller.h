#pragma once

#pragma once

#include <SMObject.h>
#include <smstructs.h>
#include <networked_module.h>



ref class Controller : public UGV_module {
public:
	error_state setupSharedMemory() override;

	error_state processSharedMemory() override;

	bool getShutdownFlag() override;

	error_state setHeartbeat(bool heartbeat) override;



	error_state getControlData();




protected:
	SMObject* ControllerData;
	int crashTimer;

};