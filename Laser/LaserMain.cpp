#include <Laser.h>

int main() {

	Laser^ laser = gcnew Laser();
	laser->connect();
	laser->recieveData();

	
	laser->getShutdownFlag();
	
	
	while (1) {
		
	}


	return 0;
}