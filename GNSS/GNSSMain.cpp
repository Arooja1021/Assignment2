#include <GNSS.h>
#include <chrono>
#include <thread>
#define ip "127.0.0.1"
#define portNum 24000

int main() {

	GNSS^ gnss = gcnew GNSS();
	gnss->setupSharedMemory();
	gnss->connect(ip, portNum);

	while (!gnss->getShutdownFlag()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		gnss->communicate();
		gnss->processSharedMemory();
		gnss->setHeartbeat(0);
	
	}
	return 0;

}