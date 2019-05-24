#ifndef INCLUDED_HEADTRACKER_MANAGER
#define INCLUDED_HEADTRACKER_MANAGER

#include "config.h"
#include "libhedrot.h"
#include <stdio.h>

#define TICK_PERIOD 30

double getTime();
int headtracker_test();

class HeadtrackerManager
{
    public:
    HeadtrackerManager();
    void startUp();
    bool isConnected();
    YawPitchRoll getYawPitchRoll();
    private:
    headtrackerData* trackingData;
};

#endif