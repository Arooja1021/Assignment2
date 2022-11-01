/*****************
UGV_module.h
ALEXANDER CUNIO 2022 (adapted from JAMES STEVENS 2021)
*****************/

/*
This file defines the base class for ALL UGV modules (PMM, Laser, GPS, Vehicle Control, Display, Controller).
Basic classes (PMM, Display, controller) will be derived directly from this class, whereas those that require networking
to external devices (GPS, Laser, VC) will derive from the networked module class (which itself inherits from this class).
For clarification of inheritance requirements see diagram in the assignment specification.
*/

#ifndef MTRN_3500_UGV_module
#define MTRN_3500_UGV_module

#include <iostream>
#include <SMObject.h>

#using <System.dll>

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;
using namespace System::Diagnostics;

// ERROR HANDLING. Use this as the return value in your functions
enum error_state {
	SUCCESS,
	ERR_STARTUP,
	ERR_NO_DATA,
	ERR_INVALID_DATA,
	ERR_SM,
	ERR_CONNECTION
	// Define your own additional error types as needed
};

ref class UGV_module abstract
{
	public:
		/**
		 * Create and access required shared memory objects
		 * 
		 * @returns the success of this function at completion
		*/
		virtual error_state setupSharedMemory() = 0;

		/**
		 * Send/Recieve data from shared memory structures
		 *
		 * @returns the success of this function at completion
		*/
		virtual error_state processSharedMemory() = 0;

		/**
		 * Get Shutdown signal for the current, from Process Management SM
		 *
		 * @returns the current state of the shutdown flag
		*/
		virtual bool getShutdownFlag() = 0;

		/**
		 * Update heartbeat signal for module
		 *
		 * @param heartbeat the state that the heartbeat should be set to
		 * @returns the success of this function at completion
		*/
		virtual error_state setHeartbeat(bool heartbeat) = 0;

		/**
		 * A helper function for printing a helpful error message based on the error type
		 * returned from a function
		 *
		 * @param error the error that should be printed out
		*/
		static void printError(error_state error)
		{
			switch (error)
			{
				case SUCCESS:
					std::cout << "Success." << std::endl;
					break;
				case ERR_NO_DATA:
					std::cout << "ERROR: No Data Available." << std::endl;
					break;
				case ERR_INVALID_DATA:
					std::cout << "ERROR: Invalid Data Received." << std::endl;
					break;
				// ADD PRINTOUTS FOR OTHER ERROR TYPES
			}
		}

	protected:
		SMObject* ProcessManagementData;	// Filled in setupSharedMemory(). For accessing PM shared Memory
		SMObject* SensorData;				// Filled in setupSharedMemory(). For storing sensor data to shared Memory (can be ignored for PM module)
};

#endif 
