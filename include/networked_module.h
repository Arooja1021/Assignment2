/*****************
networked_module.h
ALEXANDER CUNIO 2022
*****************/

/*
This class provides additional functionality for networked modules (Laser, GPS, Vehicle control) beyond the
base UGV module class. These modules should therefore be derived from this rather than the base class.
For clarification of inheritance requirements see diagram in the assignment specification.
*/

#ifndef MTRN_3500_networked_module
#define MTRN_3500_networked_module

#include <UGV_module.h>

// You will need to select which address to use depending on if you are working with the simulator (127.0.0.1)
// or the physical robot in the lab (192.168.1.200).
#define WEEDER_ADDRESS "127.0.0.1"
//#define WEEDER_ADDRESS "192.168.1.200"

ref class networked_module abstract : public UGV_module
{
	public:
		virtual error_state connect(String^ hostName, int portNumber) = 0;	// Establish TCP connection
		virtual error_state communicate() = 0;								// Communicate over TCP connection (includes any error checking required on data)

	protected:
		TcpClient^ Client;					// Handle for TCP connection
		NetworkStream^ Stream;				// Handle for TCP data stream
		array<unsigned char>^ ReadData;		// Array to store sensor Data
};

#endif 
