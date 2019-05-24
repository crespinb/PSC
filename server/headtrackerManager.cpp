//  headtrackerManager.cpp
//  modified version of hedrotReceiverDemo_cmd.c
//
//  Created by Alexis Baskind on 16/10/16.
//  Copyright (c) 2016 Alexis Baskind. All rights reserved.

#include <stdio.h>
#include <string>
#include <iostream>
#include <Windows.h>

#include "headtrackerManager.h"

//=====================================================================================================
// definitions and includes for clocking
//=====================================================================================================

double getTime() {
	LARGE_INTEGER frequency;
    LARGE_INTEGER time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&time);
    return time.QuadPart / (double)frequency.QuadPart;
}

int headtracker_test(void){
    double currentTime1, currentTime2, previousTime;
    char messageNumber;
	int i;
    headtrackerData* trackingData;

    //printf("Hedrot command-line demo, based on hedrot version %s, compiled on "__DATE__"\r\n", HEDROT_VERSION);
    //printf("Required firmware version %d\r\n", HEDROT_FIRMWARE_VERSION);
    printf("HEDROT LOADED\n");
    
    trackingData = headtracker_new();
    
    // change baudrate (for some reason the command-line version does not accept higher baud rates than 57600)
    trackingData->serialcomm->baud = 57600;
    
    // set verbose to 1
    setVerbose(trackingData,1);
    
    // set autodiscover to 1
    setAutoDiscover(trackingData,1);
    
    // switch on the headtracker
    setHeadtrackerOn(trackingData,1);
    //headtracker_open(trackingData,1); // if autodiscover = 0
    
    previousTime = getTime();
    
    while(1) { //infinite loop
        currentTime1 = getTime();
        
        // next tick
        headtracker_tick(trackingData);
        
        // process messages to notify
        while( (messageNumber = pullNotificationMessage(trackingData)) ) {
            switch( messageNumber ) {
                case NOTIFICATION_MESSAGE_COMM_PORT_LIST_UPDATED:
                    printf("port list updated, %d ports found\r\n", trackingData->serialcomm->numberOfAvailablePorts);
					for(i = 0;i<trackingData->serialcomm->numberOfAvailablePorts; i++) {
						printf("\t port %d: %s \r\n", i, trackingData->serialcomm->availablePorts[i]);
					}
                    break;
                case NOTIFICATION_MESSAGE_PORT_OPENED:
                    printf("port %s opened\r\n", trackingData->serialcomm->availablePorts[trackingData->serialcomm->portNumber]);
                    break;
                case NOTIFICATION_MESSAGE_WRONG_FIRMWARE_VERSION:
                    printf("Wrong Headtracker Firmware Version - expected version: %i - actual version: %i\r\n",HEDROT_FIRMWARE_VERSION,trackingData->firmwareVersion);
                    break;
                case NOTIFICATION_MESSAGE_HEADTRACKER_STATUS_CHANGED:
                    printf("new reception status: %d\r\n",trackingData->infoReceptionStatus);
                    break;
                case NOTIFICATION_MESSAGE_SETTINGS_DATA_READY:
                    printf("headtracking settings received\r\n");
                    break;
                case NOTIFICATION_MESSAGE_CALIBRATION_NOT_VALID:
                    printf("calibration not valid\r\n");
                    break;
                case NOTIFICATION_MESSAGE_GYRO_CALIBRATION_STARTED:
                    printf("gyroscope calibration started\r\n");
                    break;
                case NOTIFICATION_MESSAGE_GYRO_CALIBRATION_FINISHED:
                    printf("gyroscope calibration finished\r\n");
                    break;
                case NOTIFICATION_MESSAGE_BOARD_OVERLOAD:
                    printf("board too slow, reduce samplerate\r\n");
                    break;
            }
        }
        currentTime2 = getTime();
        
        
        // print estimated quaternion and angles if transmitting
        if(trackingData->infoReceptionStatus == COMMUNICATION_STATE_HEADTRACKER_TRANSMITTING) {
            printf("estimated quaternion: %f %f %f %f\r\n", trackingData->qcent1, trackingData->qcent2, trackingData->qcent3, trackingData->qcent4);
            printf("estimated angles: yaw %f - pitch %f - roll %f\r\n", trackingData->yaw, trackingData->pitch, trackingData->roll);
            printf("Time elapsed since last tick = %f sec\r\n", currentTime2 - previousTime);
        }
        
        previousTime = currentTime2;
        
        // sleep so that the next tick starts TICK_PERIOD later
        Sleep((DWORD) (TICK_PERIOD*1000));
    }
    return 0;
}

HeadtrackerManager::HeadtrackerManager()
{
    trackingData = headtracker_new();
    trackingData->serialcomm->baud = 57600;
}

void HeadtrackerManager::startUp()
{
    //setVerbose(trackingData,1);
    setAutoDiscover(trackingData,1);
    setHeadtrackerOn(trackingData,1);
}

bool HeadtrackerManager::isConnected()
{
    headtracker_tick(trackingData);
    return (trackingData->infoReceptionStatus == COMMUNICATION_STATE_HEADTRACKER_TRANSMITTING);
}

YawPitchRoll HeadtrackerManager::getYawPitchRoll()
{
    headtracker_tick(trackingData);
    return {trackingData->yaw, trackingData->pitch, trackingData->roll};
}