#include <Laser.h>

int main() {

	Laser laser;
	laser.connect();
	laser.recieveData();

	while (1) {}

	return 0;
}