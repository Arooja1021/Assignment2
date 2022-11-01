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

	// Creat TcpClient object and connect to it
	TcpClient^ Client;
	Client = gcnew TcpClient(ip, portNum);
	
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;
	Client->SendTimeout = 500;
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;
	
	// unsigned char arrays of 16 bytes each are created on managed heap
	auto SendData = gcnew array<unsigned char>(1024);
	auto ReadData = gcnew array<unsigned char>(1024);
	auto Message = gcnew String("");
	
	Message = zid + "\n";
	SendData = Encoding::ASCII->GetBytes(Message);

	//declaring stream
	Stream = Client->GetStream();
	Stream->Write(SendData, 0, SendData->Length);

	Threading::Thread::Sleep(50);

	// Read the incoming data
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

	auto ReadData = gcnew array<unsigned char>(1024); 
	Stream->Read(ReadData, 0, ReadData->Length);

	auto ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Console::WriteLine(ResponseData);

	return 0;
}