#include <Display.h>
#include <SMObject.h>
#include <smstructs.h>
#include <conio.h>
#include <chrono>
#include <thread>

int main(void) {
	Display myDisplay;
	
	myDisplay.setupSharedMemory();
	

	while (!myDisplay.getShutdownFlag()) {
		myDisplay.matlab_plotting();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	myDisplay.shutdown();

	return 0;
}
