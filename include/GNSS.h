#pragma once

#include <SMObject.h>
#include <smstructs.h>
#include <networked_module.h>


struct GPS
{
	unsigned int Header;
	unsigned char Discards1[40];
	double Northing;
	double Easting;
	double Height;
	unsigned char Discards2[40];
	unsigned int Checksum;

};





ref class GNSS : public networked_module {
public:
	error_state setupSharedMemory() override;

	error_state processSharedMemory() override;

	bool getShutdownFlag() override;

	error_state setHeartbeat(bool heartbeat) override;


	error_state connect(String^ hostName, int portNumber) override;
	error_state communicate() override;

	int connect() override;



protected:

};
