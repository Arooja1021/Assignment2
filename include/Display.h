#pragma once

#include <UGV_module.h>
#include <smstructs.h>
#include <SMObject.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <conio.h>
#include <iostream>
#include <engine.h> 
#include <Display.h>

ref class Display : public UGV_module {
public:
    // Create and access shared memory objects
    error_state setupSharedMemory() override;



    // Recieve/update data from process management shared memory structure about the state of all modules
    error_state processSharedMemory() override;

    int matlab_plotting();

    // Get Shutdown signal for module, from Process Management SM
    bool getShutdownFlag() override;

    // Update heartbeat signal for process management
    error_state setHeartbeat(bool heartbeat) override;

    void shutdown();

private:
    // Add any class member variables you require
    // Remember that some member variables already exist in the parent classes that may be useful
    // ensure you do not have any duplicates due to this

    SMObject* DisplayData;
    SMObject* GPSData;
    SMObject* LaserData;
    int plotCreated;
    engine* ep;
    int crashTimer;

};