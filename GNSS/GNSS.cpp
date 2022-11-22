#pragma once


#include <GNSS.h>

#include <SMObject.h>
#include <smstructs.h>
#include <networked_module.h>


#define zid 5309962


error_state GNSS::connect(String^ hostName, int portNumber) {
	Client = gcnew TcpClient(hostName, portNumber);


	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;
	Client->SendTimeout = 500;
	Client->ReceiveBufferSize = 512;
	ReadData = gcnew array<unsigned char>(4096);

	Stream = Client->GetStream();

	



	return SUCCESS;


}

error_state GNSS::communicate() {
	if (Stream->DataAvailable) {
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
		SM_GPS* GPSPtr = (SM_GPS*)GPSData;
		GPSPtr->Northing = data.Northing;
		GPSPtr->Easting = data.Easting;
		GPSPtr->Height = data.Height;
	}
	return SUCCESS;
	
}





error_state GNSS::setupSharedMemory() {

	SMObject* GPSObj = new SMObject(TEXT("GPSObj"), sizeof(SM_GPS));
	if (GPSObj->SMAccess() == SM_ACCESS_ERROR) {
		Console::WriteLine("Access Error");
	}
	GPSData = (SMObject*)GPSObj->pData;

	SMObject* PMObj = new SMObject(TEXT("PMMObj"), sizeof(SM_ProcessManagement));

	if (PMObj->SMAccess() == SM_ACCESS_ERROR) {
		Console::WriteLine("Access Error");
	}
	ProcessManagementData = (SMObject*)PMObj->pData;


	return SUCCESS;
}

error_state GNSS::processSharedMemory() {


	return SUCCESS;
}

bool GNSS::getShutdownFlag() {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	

	return PMMPtr->Shutdown.Flags.GPS;
}

error_state GNSS::setHeartbeat(bool heartbeat) {
	return SUCCESS;
}




