#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>
#include <conio.h>

int main(void) {
	ProcessManagement myPM;


	myPM.setupSharedMemory();
	myPM.shutdownModules();

	while (!_kbhit()) {

	}


	return 0;
}
