#pragma once

#include <SMObject.h>
#include <smstructs.h>


ref class Laser {
public:
	int connect();
	int recieveData();

protected:
	NetworkStream^ Stream;

};
