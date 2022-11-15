#pragma once

#include <GNSS.h>

#include <SMObject.h>
#include <smstructs.h>
#include <networked_module.h>

#define ip "127.0.0.1"
#define portNum 24000
#define zid 5309962


int GNSS::connect() {
	Client = gcnew TcpClient(ip, portNum);


	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;
	Client->SendTimeout = 500;
	Client->ReceiveBufferSize = 256;
	ReadData = gcnew array<unsigned char>(1024);

	Stream = Client->GetStream();

	Stream->Read(ReadData, 0, ReadData->Length);


	unsigned int Header = 0;
	int i = 0;
	int Start;
	do
	{

		Header = ((Header << 8) | ReadData[i++]);
	} while (Header != 0xaa44121c);
	Start = i - 4;

	GPS data;
	unsigned char* BytePtr = nullptr;
	BytePtr = (unsigned char*)&data;

	for (int i = Start; i < Start + sizeof(GPS); i++) {

		*(BytePtr++) = ReadData[i];

	}

	Console::WriteLine(data.Northing);

	return 0;


}


error_state GNSS::setupSharedMemory() {




	return SUCCESS;
}

error_state GNSS::processSharedMemory() {
	return SUCCESS;
}

bool GNSS::getShutdownFlag() {

	

	return 0;
}

error_state GNSS::setHeartbeat(bool heartbeat) {
	return SUCCESS;
}
error_state GNSS::connect(String^ hostName, int portNumber) {
	return SUCCESS;
}
error_state GNSS::communicate() {
	return SUCCESS;
}