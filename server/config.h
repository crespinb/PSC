#ifndef INCLUDED_CONFIG
#define INCLUDED_CONFIG

#define CHANNEL_COUNT 8
#define SAMPLE_COUNT_PER_BUFFER 8192
#define SAMPLE_COUNT_PER_HRTF 8192
#define TIME_INTERVAL_COUNT 4

#define SERVER_PORT (unsigned short) 2019

class YawPitchRoll
{
    public:
    float yaw;
    float pitch;
    float roll;
    YawPitchRoll operator-(YawPitchRoll);
};

#endif