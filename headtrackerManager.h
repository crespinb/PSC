#ifndef INCLUDED_HEADTRACKER_MANAGER
#define INCLUDED_HEADTRACKER_MANAGER

#include "libhedrot.h"
#include <stdio.h>

#define TICK_PERIOD 30

double getTime();
int headtracker_test();

struct YawPitchRoll
{
    float yaw;
    float pitch;
    float roll;
};

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