#ifndef INCLUDED_AUDIO_MANAGER
#define INCLUDED_AUDIO_MANAGER

#include "config.h"
#include "headtrackerManager.h"
#include "serverManager.h"
#include "sigpack.h"
#include "soundfile.h"
#include <deque>

using namespace arma;

class ConvolutionFilter
{
    public:
    ConvolutionFilter(vec& inCoeffs);
    void setCoeffs(vec& inCoeffs);
    vec filter(vec& input);
    private:
    vec coeffs;
    cx_vec fftCoeffs;
    sp::FFTW* fftwTool;
};

class AudioManager
{
    public:
    AudioManager();
    void loadAllHrtf();
    //fvec* process(fvec& input);
    void mainLoop();
    void processNextBuffer();
    void setInitYPR();
    void updateYPR();
    void updateYPR(int);
    //HRTF functions
    void updateHrtf();
    static int getHrtfElevation(float elevation);
    //random functions
    static std::string to3LengthString(int n);
    void writeOutputInFile();
    void writeOutputToClient();
    void updateCarriedBuffer();
    private:
    const std::string elevationList[7] = {"000", "015", "030", "045", "315", "330", "345"};
    HeadtrackerManager* headtrackerMgr;
    ServerManager* serverMgr;
    SoundFileRead* inStream;
    SoundFileWrite* outStream;
    YawPitchRoll ypr;
    YawPitchRoll initYpr;
    //sp::FIR_filt<double, double, double>* allHrtf[24][7][2];
    //sp::FIR_filt<double, double, double>* currentHrtf[CHANNEL_COUNT][2];
    ConvolutionFilter* allHrtf[24][7][2];
    ConvolutionFilter* currentHrtf[CHANNEL_COUNT][2];
    vec currentInputBuffer[CHANNEL_COUNT];
    vec currentOutputBuffer[2];
    vec currentCarriedBuffer[2];
    //Sound
    //fvec** currentHrtfData; //usage: currentHrtfData[channel][ear]
    //std::deque <fvec*> currentOutputBuffer;
};

#endif

//arma::fvec = arma::Col<float>