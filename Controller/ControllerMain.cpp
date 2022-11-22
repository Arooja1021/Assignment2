#include <Controller.h>
#include <chrono>
#include <thread>

int main() {

	Controller^ controller = gcnew Controller();
	controller->setupSharedMemory();


	while (!controller->getShutdownFlag()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		controller->getControlData();
		controller->processSharedMemory();
		controller->setHeartbeat(0);

	}
	return 0;

}