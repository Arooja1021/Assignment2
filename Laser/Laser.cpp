#include <Laser.h>
#include <smstructs.h>
#include <SMObject.h>

#define ip "127.0.0.1"
#define portNum 23000
#define zid 5257944

using namespace System;
using namespace System::Diagnostics;
using namespace Net;
using namespace Sockets;
using namespace Text;

int Laser::connect() {


	Client = gcnew TcpClient(ip, portNum);
	

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

	Stream->Read(ReadData, 0, ReadData->Length);
	auto ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Console::WriteLine(ResponseData);
	
	return 0;
}

int Laser::recieveData() {
	Stream->WriteByte(0x02);
	
	auto Message = gcnew String("");
	Message = "sRN LMDscandata";

	auto SendData = System::Text::Encoding::ASCII->GetBytes(Message);
	Stream->Write(SendData, 0, SendData->Length);

	Stream->WriteByte(0x03);

	ReadData = gcnew array<unsigned char>(1024); 
	Stream->Read(ReadData, 0, ReadData->Length);

	auto ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Console::WriteLine(ResponseData);

	return 0;
}

error_state Laser::setupSharedMemory() {




	return SUCCESS;
}

error_state Laser::processSharedMemory() {
	return SUCCESS;
}

bool Laser::getShutdownFlag() {

	SMObject PMMObj(TEXT("PMMObj"), sizeof(SM_ProcessManagement));
	SM_ProcessManagement* PMMPtr;
	PMMObj.SMAccess();
	PMMPtr = (SM_ProcessManagement*)PMMObj.pData;

	return PMMPtr->Shutdown.Flags.Laser;
}

error_state Laser::setHeartbeat(bool heartbeat) {
	return SUCCESS;
}
error_state Laser::connect(String^ hostName, int portNumber) {
	return SUCCESS;
}
error_state Laser::communicate() {
	return SUCCESS;
}