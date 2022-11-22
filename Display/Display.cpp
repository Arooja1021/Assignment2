#include <stdio.h>
#include <string>
#include <math.h>
#include <conio.h>
#include <iostream>
#include <engine.h> 
#include <Display.h>
//in E:\Software\MATLAB/extern/include
// run matlabroot in matlab to get the base directory

#define DATA_SIZE 10


int Display::matlab_plotting() {
    // Variable declarations
    mxArray* X = NULL, * Y = NULL;

    //Start local MATLAB engine
    if (!(ep = engOpen("\0")))
    {
        printf("\nMATLAB engine failure\n");
        return 1;
    }

    // Establish plot figure and set size
    engEvalString(ep, "figure(1);");
    engEvalString(ep, "myAx = axis([0 361 0 361]); hold on");



    double x[361], y[361];
    SM_Laser* LaserPtr = (SM_Laser*)LaserData;
    SM_GPS* GPSPtr = (SM_GPS*)GPSData;
    for (int i = 0; i < 361; i++) {
        

        x[i] = LaserPtr->x[i];
        y[i] = LaserPtr->y[i];


    }



    // Matlab tasks
    X = mxCreateDoubleMatrix(361, 1, mxREAL);
    Y = mxCreateDoubleMatrix(361, 1, mxREAL);

    // Fill in matlab space
    memcpy((void*)mxGetPr(X), (void*)&x, sizeof(x));
    memcpy((void*)mxGetPr(Y), (void*)&y, sizeof(y));


    // Transfer data to Matlab
    engPutVariable(ep, "X", X);
    engPutVariable(ep, "Y", Y);

    // Normal plot function

    if (!plotCreated) {
        engEvalString(ep, "myPlot = plot(X, Y, 'r')");
 
    }
    else {
        engEvalString(ep, "set(myPlot, 'xdata', X, 'ydata', Y)");
    }
    engEvalString(ep, "axis([0 8000 -8000 8000])");
    std::string a = "txt = {'Northing = " + std::to_string(GPSPtr->Northing) + "', 'Easting = " 
        + std::to_string(GPSPtr->Easting) + "', 'Height = " + std::to_string(GPSPtr->Height) + "'}";
 
   
    const char* asd = a.c_str();
    engEvalString(ep, "delete(b)");
    engEvalString(ep, asd);
    engEvalString(ep, "b = text(5500, 6500, txt)");

    mxDestroyArray(X);
    mxDestroyArray(Y);

    // Faster command to update plot
    //engEvalString(ep, "set(myPlot, 'xdata', X, 'ydata', Y)");

    plotCreated = 1;

    return 0;
}


void Display::shutdown() {


    engEvalString(ep, "close all");
    engClose(ep);


}


bool Display::getShutdownFlag() {
    SM_ProcessManagement* PMMPtr = (SM_ProcessManagement*)ProcessManagementData;


    return PMMPtr->Shutdown.Flags.Display;

    return false;
}

// Update heartbeat signal for process management
error_state Display::setHeartbeat(bool heartbeat) {
    return SUCCESS;
}

error_state Display::processSharedMemory() {

    return SUCCESS;
}


error_state Display::setupSharedMemory() {

    SMObject* PMObj = new SMObject(TEXT("PMMObj"), sizeof(SM_ProcessManagement));

    if (PMObj->SMAccess() == SM_ACCESS_ERROR) {
        Console::WriteLine("Access Error");
    }
    ProcessManagementData = (SMObject*)PMObj->pData;

    SMObject* GPSObj = new SMObject(TEXT("GPSObj"), sizeof(SM_GPS));
    if (GPSObj->SMAccess() == SM_ACCESS_ERROR) {
        Console::WriteLine("Access Error");
    }
    GPSData = (SMObject*)GPSObj->pData;

    SMObject* LaserObj = new SMObject(TEXT("LaserObj"), sizeof(SM_Laser));

    if (LaserObj->SMAccess() == SM_ACCESS_ERROR) {
        Console::WriteLine("Access Error");
    }
    LaserData = (SMObject*)LaserObj->pData;

    plotCreated = 0;

    return SUCCESS;
}