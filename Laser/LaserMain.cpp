#include <Laser.h>

int main() {

	Laser^ laser = gcnew Laser();
	laser->connect();
	laser->recieveData();

	
		
	SMObject PMObj(TEXT("PMObj"), sizeof(SM_ProcessManagement));
	SM_ProcessManagement* PMMPtr;
		
	PMObj.SMAccess();
	PMMPtr = (SM_ProcessManagement*)PMObj.pData;
	
	while (!PMMPtr->Shutdown.Status) {
		
	}


	return 0;
}