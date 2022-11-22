#pragma once
#define CRC32_POLYNOMIAL	0xEDB88320L

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


unsigned long CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for (j = 8; j > 0; j--)
	{
		if (ulCRC & 1)
			ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}

unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char* ucBuffer) 
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
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
		if (data.Checksum == CalculateBlockCRC32(108, (unsigned char*)&data)) {
			GPSPtr->Northing = data.Northing;
			GPSPtr->Easting = data.Easting;
			GPSPtr->Height = data.Height;
			Console::WriteLine("Northing: {0:F3}\tEasting: {1:F3}\tHeight: {1:F3}", GPSPtr->Northing, GPSPtr->Easting, GPSPtr->Height = data.Height);
		}
		

			
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

bool GNSS::getShutdownFlag() {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	

	return PMMPtr->Shutdown.Flags.GPS;
}

error_state GNSS::setHeartbeat(bool heartbeat) {
	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;
	PMMPtr->Heartbeat.Flags.GPS = heartbeat;
	return SUCCESS;
}




