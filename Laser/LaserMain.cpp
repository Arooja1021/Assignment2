#include <Laser.h>
#include <chrono>
#include <thread>
#define ip "127.0.0.1"
#define portNum 23000

int main() {

	Laser^ laser = gcnew Laser();
	laser->connect(ip, portNum);
	laser->setupSharedMemory();

	
	
	
	
	
	while (!laser->getShutdownFlag()) {
		laser->communicate();
		laser->processSharedMemory();
		laser->setHeartbeat(0);
	}


	return 0;
}