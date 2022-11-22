#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>
#include <conio.h>

int main(void) {
	ProcessManagement myPM;


	myPM.setupSharedMemory();

	
	myPM.startupProcesses();


	while (!myPM.getShutdownFlag()) {
		if (_kbhit()) {
			break;
		}
		myPM.setHeartbeat(1);
		myPM.processSharedMemory();

	}
	myPM.shutdownModules();


	return 0;
}
