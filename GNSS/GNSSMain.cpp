#include <GNSS.h>

int main() {

	GNSS^ gnss = gcnew GNSS();
	gnss->connect();
	while (1) {}
	return 0;

}