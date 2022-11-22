#pragma once

#include <SMObject.h>
#include <smstructs.h>
#include <networked_module.h>



ref class Laser : public networked_module{
public:
	error_state setupSharedMemory() override;

	error_state processSharedMemory() override;

	bool getShutdownFlag() override;

	error_state setHeartbeat(bool heartbeat) override;


	error_state connect(String^ hostName, int portNumber) override;
	error_state communicate() override;


protected:
	SMObject* LaserData;
	int crashTimer;
};
