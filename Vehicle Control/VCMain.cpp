#include <VC.h>
#define ip "127.0.0.1"
#define portNum 25000

int main() {

	VC^ vc = gcnew VC();
	vc->setupSharedMemory();
	vc->connect(ip, portNum);
	

	while (!vc->getShutdownFlag()) {
		vc->communicate();
		vc->processSharedMemory();
		vc->setHeartbeat(0);
	}


	return 0;
}