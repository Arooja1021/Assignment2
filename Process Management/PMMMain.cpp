#include <PMM.h>
#include <conio.h>

int main(void) {
	ProcessManagement myPM;

	SMObject PMObj(TEXT("PMObj"), sizeof(SM_ProcessManagement));
	SM_ProcessManagement* PMMPtr;
	PMObj.SMCreate();
	PMObj.SMAccess();
	PMMPtr = (SM_ProcessManagement*)PMObj.pData;


	PMMPtr->Shutdown.Status = 0x00;
	myPM.startupProcesses();

	while (!_kbhit()) {

	}

	PMMPtr->Shutdown.Status = 0xFF;
	return 0;
}
