#include <Laser.h>
#include <smstructs.h>
#include <SMObject.h>


#define zid 5309962

using namespace System;
using namespace System::Diagnostics;
using namespace Net;
using namespace Sockets;
using namespace Text;

error_state Laser::connect(String^ hostName, int portNumber) {


	Client = gcnew TcpClient(hostName, portNumber);
	

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
	
	return SUCCESS;
}

error_state Laser::communicate() {
	Stream->WriteByte(0x02);
	
	auto Message = gcnew String("");
	Message = "sRN LMDscandata";

	auto SendData = System::Text::Encoding::ASCII->GetBytes(Message);
	Stream->Write(SendData, 0, SendData->Length);

	Stream->WriteByte(0x03);

	ReadData = gcnew array<unsigned char>(2048); 
	Stream->Read(ReadData, 0, ReadData->Length);

	auto ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);

	array<wchar_t>^ Space = { ' ' };
	array<String^>^ StringArray = ResponseData->Split(Space);
	double StartAngle = System::Convert::ToInt32(StringArray[23], 16);
	double Resolution = System::Convert::ToInt32(StringArray[24], 16) / 10000.0;
	int NumRanges = System::Convert::ToInt32(StringArray[25], 16);

	array<double>^ Range = gcnew array<double>(NumRanges);
	array<double>^ RangeX = gcnew array<double>(NumRanges);
	array<double>^ RangeY = gcnew array<double>(NumRanges);

	// save information into shared memory and print onto console
	SM_Laser* LaserPtr = (SM_Laser*)LaserData;
	for (int i = 0; i < NumRanges; i++) {
		Range[i] = Convert::ToInt32(StringArray[26 + i], 16);
		RangeX[i] = Range[i] * sin(i * Resolution * Math::PI / 180);
		RangeY[i] = -Range[i] * cos(i * Resolution * Math::PI / 180);

		LaserPtr->x[i] = RangeX[i];
		LaserPtr->y[i] = RangeY[i];

		Console::WriteLine("X: {0:F3}\tY: {1:F3}", RangeX[i], RangeY[i]);
	}

	System::Threading::Thread::Sleep(100);

	return SUCCESS;
}

error_state Laser::setupSharedMemory() {
	SMObject* PMObj = new SMObject(TEXT("PMMObj"), sizeof(SM_ProcessManagement));

	if (PMObj->SMAccess() == SM_ACCESS_ERROR) {
		Console::WriteLine("Access Error");
	}
	ProcessManagementData = (SMObject*)PMObj->pData;


	SMObject* LaserObj = new SMObject(TEXT("LaserObj"), sizeof(SM_Laser));

	if (LaserObj->SMAccess() == SM_ACCESS_ERROR) {
		Console::WriteLine("Access Error");
	}
	LaserData = (SMObject*)LaserObj->pData;



	return SUCCESS;
}

error_state Laser::processSharedMemory() {
	return SUCCESS;
}

bool Laser::getShutdownFlag() {

	SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;


	return PMMPtr->Shutdown.Flags.Laser;


}

error_state Laser::setHeartbeat(bool heartbeat) {
	return SUCCESS;
}
