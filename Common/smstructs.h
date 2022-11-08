#pragma once

#ifndef SMSTRUCTS_H
#define SMSTRUCTS_H

#using <System.dll>
#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

// Use these for defining the name of SMstructs when they are created and accessed
// This can be used in the SMObject constructor and may need to be cast, for example (TCHAR *)PMArray
#define PMArray _TEXT("ProcessManagement")
#define LaserArray _TEXT("Laser")
#define GPSArray _TEXT("GPS")
#define VehicleControlArray _TEXT("VehicleControl")

// Defines which processes are critical and not critical
// Ordering of processes in the mask should be based on the order in the UnitFlags type below.
// You should change this based on what processes are and are not critical
#define NONCRITICALMASK 0xff
#define CRITICALMASK 0x00

struct UnitFlags
{
	unsigned char	ProcessManagement : 1,
					Laser : 1,
					VehicleControl : 1,
					GPS : 1,
					Controller : 1,
					Display : 1,
					Garbage : 2;
};

union ExecFlags
{
	UnitFlags Flags;
	unsigned short Status;
};

struct SM_ProcessManagement
{
	ExecFlags Heartbeat;
	ExecFlags Shutdown;
	ExecFlags Ready;
	long int LifeCounter;
};

// This is the size of the data recieved from the laser
#define STANDARD_LASER_LENGTH 361

struct SM_Laser
{
	double x[STANDARD_LASER_LENGTH];
	double y[STANDARD_LASER_LENGTH];
};

struct SM_GPS
{
	double Northing;
	double Easting;
	double Height;
};

struct SM_VehicleControl
{
	double Speed;
	double Steering;
};

#endif
