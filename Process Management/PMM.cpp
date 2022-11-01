

#include <PMM.h>
#include <SMObject.h>
#include <smstructs.h>

using namespace std;

error_state ProcessManagement::setupSharedMemory() {
	SMObject PMMObj(TEXT("PMM"), sizeof(SMObject));
	if (PMMObj.SMCreate() || PMMObj.SMAccess()) {
		return ERR_SM;
	}
	ProcessManagementData = (SMObject*)PMMObj.pData;

	SMObject LaserObj(TEXT("SM_Laser"), sizeof(SMObject));
	if (LaserObj.SMCreate() || LaserObj.SMAccess()) {
		return ERR_SM;
	}
	LaserData = (SMObject*)LaserObj.pData;

	return SUCCESS;
}


error_state ProcessManagement::startupProcesses() {
	return SUCCESS;
}

// Recieve/update data from process management shared memory structure about the state of all modules
error_state ProcessManagement::processSharedMemory() {
	return SUCCESS;
}

// Signal shutdown of all processes on normal termination
void ProcessManagement::shutdownModules() {
	cout << ProcessManagementData;
}

// Get Shutdown signal for module, from Process Management SM
bool ProcessManagement::getShutdownFlag() {
	return false;
}

// Update heartbeat signal for process management
error_state ProcessManagement::setHeartbeat(bool heartbeat) {
	return SUCCESS;
}
