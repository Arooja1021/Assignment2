#pragma once

#include <UGV_module.h>
#include <smstructs.h>
#include <SMObject.h>

ref class ProcessManagement : public UGV_module {
public:
    // Create and access shared memory objects
    error_state setupSharedMemory() override;

    // Add logic for starting individual processes - should use some form of CLR process list as discussed in lectures
    error_state startupProcesses();

    // Recieve/update data from process management shared memory structure about the state of all modules
    error_state processSharedMemory() override;

    // Signal shutdown of all processes on normal termination
    void shutdownModules();

    // Get Shutdown signal for module, from Process Management SM
    bool getShutdownFlag() override;

    // Update heartbeat signal for process management
    error_state setHeartbeat(bool heartbeat) override;

private:
    // Add any class member variables you require
    // Remember that some member variables already exist in the parent classes that may be useful
    // ensure you do not have any duplicates due to this
    SMObject* ControllerData;
    SMObject* DisplayData;
    SMObject* GNSSData;
    SMObject* LaserData;
    SMObject* VCData;

};