#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>
#include <conio.h>

int main(void) {
	ProcessManagement myPM;


	myPM.setupSharedMemory();

	
	myPM.startupProcesses();


	while (!_kbhit()) {

	}
	myPM.shutdownModules();


	return 0;
}
