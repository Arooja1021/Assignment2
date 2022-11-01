#include <Laser.h>

int main() {

	Laser^ laser = gcnew Laser();
	laser->connect();
	laser->recieveData();

	while (1) {}

	return 0;
}