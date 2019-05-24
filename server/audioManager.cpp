#include "audioManager.h"
#include <stdio.h>

//define absolute standard positions for channels
YawPitchRoll ABSOLUTE_SOURCE_POSITIONS[CHANNEL_COUNT] =
{
    YawPitchRoll{-30, 0, 0},
    YawPitchRoll{30, 0, 0},
    YawPitchRoll{0, 0, 0},
    YawPitchRoll{0, 0, 0},
    YawPitchRoll{-110, 0, 0},
    YawPitchRoll{110, 0, 0},
    YawPitchRoll{-150, 0, 0},
    YawPitchRoll{150, 0, 0}
};

std::string INPUT_FILE = "test.wav";
std::string OUTPUT_FILE = "test-output.wav";

ConvolutionFilter::ConvolutionFilter(vec& inCoeffs)
{
    fftwTool = new sp::FFTW(SAMPLE_COUNT_PER_BUFFER*2, FFTW_ESTIMATE);
    coeffs = vec(SAMPLE_COUNT_PER_BUFFER*2, arma::fill::zeros);
    fftCoeffs = cx_vec(SAMPLE_COUNT_PER_BUFFER*2);
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)//change end condition?
    {
        coeffs[i] = inCoeffs[i];
    }
    fftCoeffs = fftwTool->fft(coeffs);
}

void ConvolutionFilter::setCoeffs(vec& inCoeffs)
{
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)//change end condition?
    {
        coeffs[i] = inCoeffs[i];
    }
    fftCoeffs = fftwTool->fft(coeffs);
}

vec ConvolutionFilter::filter(vec& input)
{
    vec input0 = vec(SAMPLE_COUNT_PER_BUFFER*2, arma::fill::zeros);
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)
    {
        input0[i] = input[i];
    }
    cx_vec fftInput = fftwTool->fft(input0);
    cx_vec fftOutput(SAMPLE_COUNT_PER_BUFFER*2);
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER*2; ++i)
    {
        fftOutput[i] = fftInput[i]*fftCoeffs[i];
    }
    return fftwTool->ifft(fftOutput);
}

AudioManager::AudioManager()
{
    //std::cout << "audio manager started" << std::endl;
    //Sleep(2000);
    /*fvec newBuffer[2];
    newBuffer[0] = fvec(SAMPLE_COUNT_PER_BUFFER, arma::fill::zeros);
    newBuffer[1] = fvec(SAMPLE_COUNT_PER_BUFFER, arma::fill::zeros);
    for(int i = 0; i < TIME_INTERVAL_COUNT; ++i)
    {
        currentOutputBuffer.push_back(newBuffer);//change
    }*/
    for(int i = 0; i < CHANNEL_COUNT; ++i)
    {
        currentInputBuffer[i] = vec(SAMPLE_COUNT_PER_BUFFER*2, arma::fill::zeros);
    }
    currentOutputBuffer[0] = vec(SAMPLE_COUNT_PER_BUFFER*2, arma::fill::zeros);
    currentOutputBuffer[1] = vec(SAMPLE_COUNT_PER_BUFFER*2, arma::fill::zeros);
    currentCarriedBuffer[0] = vec(SAMPLE_COUNT_PER_BUFFER, arma::fill::zeros);
    currentCarriedBuffer[1] = vec(SAMPLE_COUNT_PER_BUFFER, arma::fill::zeros);
    std::cout << "Loading HRTF data in RAM..." << std::endl;
    /*for(int T = 0; T < 24; ++T)
    {
        for(int P = 0; P < 7; ++P)
        {
            //allHrtf[T][P][0] = new sp::FIR_filt<double, double, double>();
            //allHrtf[T][P][1] = new sp::FIR_filt<double, double, double>();
            allHrtf[T][P][0] = new ConvolutionFilter();
        }
    }*/
    loadAllHrtf();
    std::cout << "HRTF data loaded!" << std::endl;
    std::cout << "Waiting for headtracker..." << std::endl;
    headtrackerMgr = new HeadtrackerManager();
    headtrackerMgr->startUp();
    while(!headtrackerMgr->isConnected())
    {
        std::cout << "No headtracker found, retrying..." << std::endl;
        Sleep(1000);
    }
    std::cout << "Headtracker connected!" << std::endl;
    setInitYPR();
    updateYPR();
    //updateHrtf();
    std::cout << "Loading input .wav..." << std::endl;
    inStream = new SoundFileRead(INPUT_FILE.c_str());
    SoundHeader hd = *inStream;
    hd.setChannels(2);
    outStream = new SoundFileWrite(OUTPUT_FILE.c_str(), hd);
    std::cout << "Loaded input .wav!" << std::endl;
    std::cout << "Server starting..." << std::endl;
    serverMgr = new ServerManager();
    if(serverMgr->startUp())
    {
        std::cout << "Server started!" << std::endl;
        std::cout << "Waiting for client, port is " + std::to_string(serverMgr->getPort()) + "..." << std::endl;
        serverMgr->waitForClient();
        std::cout << "Client connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to start server, exiting..." << std::endl;
        exit(1);
    }
    mainLoop();
}

/*AudioManager::~AudioManager()
{
    delete 
}*/

void AudioManager::loadAllHrtf()
{
    std::string hrtfFilename;
    SoundFileRead* hrtfStream;
    vec rawHrtf0(SAMPLE_COUNT_PER_HRTF);
    vec rawHrtf1(SAMPLE_COUNT_PER_HRTF);
    for(int T = 0; T < 24; ++T)
    {
        for(int P = 0; P < 7; ++P)
        {
            hrtfFilename = "IRC_1002_C/IRC_1002_C_R0195_T"+to3LengthString(15*T)+"_P"+elevationList[P]+".wav";
            hrtfStream = new SoundFileRead(hrtfFilename.c_str());
            for(int sample = 0; sample < SAMPLE_COUNT_PER_HRTF; ++sample)
            {
                rawHrtf0[sample] = hrtfStream->getCurrentSampleDouble(0);
                rawHrtf1[sample] = hrtfStream->getCurrentSampleDouble(1);
                hrtfStream->incrementSample();
            }
            /*delete allHrtf[T][P][0];
            delete allHrtf[T][P][0];
            allHrtf[T][P][0] = new sp::FIR_filt<double, double, double>();
            allHrtf[T][P][1] = new sp::FIR_filt<double, double, double>();*/
            //allHrtf[T][P][0]->set_coeffs(rawHrtf0);
            //allHrtf[T][P][1]->set_coeffs(rawHrtf1);
            allHrtf[T][P][0] = new ConvolutionFilter(rawHrtf0);
            allHrtf[T][P][1] = new ConvolutionFilter(rawHrtf1);
            delete hrtfStream;
        }
    }
}

/*fvec* AudioManager::process(fvec& input)
{
    sp::FIR_filt<float, float, float>* filter = new sp::FIR_filt<float, float, float>();
    fvec newBuffer[2];
    newBuffer[0] = fvec(SAMPLE_COUNT_PER_BUFFER, arma::fill::zeros);
    newBuffer[1] = fvec(SAMPLE_COUNT_PER_BUFFER, arma::fill::zeros);
    currentOutputBuffer.push_back(newBuffer);
    for(int channel = 0; channel < CHANNEL_COUNT; ++channel)
    {
        for(int ear = 0; ear < 2; ++ear)
        {
            for(int delay = 0; delay < TIME_INTERVAL_COUNT; ++delay)
            {
                //create filter for hrtf[channel][ear][time slice]
                //filter input[channel] by that filter
                //filter->set_coeffs(); //set arg
                currentOutputBuffer.at(delay)[ear] += filter->filter(input);//slice it, equalize it
                //add output[0, SAMPLE_COUNT_PER_BUFFER] to output[time][ear], then output [numS+1:] to output[time+1][ear]
            }
        }
    }
    //return first item of deque
    fvec* output = currentOutputBuffer.front();
    currentOutputBuffer.pop_front();
    return output;
}*/

void AudioManager::mainLoop()
{
    for(int i = 0; i < 5000; ++i)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        //updateYPR();
        updateYPR(i);
        auto t2 = std::chrono::high_resolution_clock::now();
        updateHrtf();
        auto t3 = std::chrono::high_resolution_clock::now();
        processNextBuffer();
        auto t4 = std::chrono::high_resolution_clock::now();
        //writeOutputInFile();
        auto t6 = std::chrono::high_resolution_clock::now();
        writeOutputToClient();
        updateCarriedBuffer();
        auto t5 = std::chrono::high_resolution_clock::now();
        //std::chrono::duration<float> d6 = t5-t6;
        //std::cout << "Buffer sent in " + std::to_string(d6.count()) << std::endl;
        /*std::chrono::duration<float> d1 = t2-t1;
        std::chrono::duration<float> d2 = t3-t2;
        std::chrono::duration<float> d3 = t4-t3;
        std::chrono::duration<float> d4 = t5-t4;
        std::cout << "YPR update in " + std::to_string(d1.count()) << std::endl;
        std::cout << "HRTF update in " + std::to_string(d2.count()) << std::endl;
        std::cout << "Filtering in " + std::to_string(d3.count()) << std::endl;*/
        //std::cout << "Buffer sent to client in " + std::to_string(d6.count()) << std::endl;
        //std::cout << "Output .wav in " + std::to_string(d4.count()) << std::endl;
        /*std::chrono::duration<float> d0 = t5-t1;
        std::cout << "Buffer processed in " + std::to_string(d0.count()) << std::endl;*/
        /*if(i % 100 == 0)
        {
            std::cout << "Processed " + std::to_string(i) + " buffers." << std::endl;
            std::cout << "Yaw:" << std::to_string(ypr.yaw) << std::endl;
            std::cout << "Pitch:" << std::to_string(ypr.pitch) << std::endl;
        }*/
    }
}

void AudioManager::writeOutputInFile()
{
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)
    {
        outStream->writeSampleDouble(currentOutputBuffer[0][i]+currentCarriedBuffer[0][i]);
        outStream->writeSampleDouble(currentOutputBuffer[1][i]+currentCarriedBuffer[1][i]);
    }
}

void AudioManager::writeOutputToClient()
{
    float buffer[2*SAMPLE_COUNT_PER_BUFFER];
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)
    {
        buffer[2*i] = (float) (currentOutputBuffer[0][i]+currentCarriedBuffer[0][i]);
        //std::cout << std::to_string(buffer[2*i]) << std::endl;
        buffer[2*i+1] = (float) (currentOutputBuffer[1][i]+currentCarriedBuffer[1][i]);
    }
    //std::cout << std::to_string(buffer[0]) << std::endl;
    serverMgr->sendBuffer(buffer);
}

void AudioManager::updateCarriedBuffer()
{
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)
    {
        currentCarriedBuffer[0][i] = currentOutputBuffer[0][i+SAMPLE_COUNT_PER_BUFFER];
        currentCarriedBuffer[1][i] = currentOutputBuffer[1][i+SAMPLE_COUNT_PER_BUFFER];
    }
}

void AudioManager::processNextBuffer()
{
    //std::cout << inputFile.getBitsPerSample() << " x " << inputFile.getSamples() << std::endl;
    //std::cout << "loading input buffer" << std::endl;
    for(int i = 0; i < SAMPLE_COUNT_PER_BUFFER; ++i)//bad code; need to load all channels
    {
        for(int channel = 0; channel < CHANNEL_COUNT; ++channel)
        {
            currentInputBuffer[channel][i] = inStream->getCurrentSampleDouble(channel);
        }
        inStream->incrementSample();
        //filling input vectors(double)
        //std::cout << input[i] << std::endl;
    }//ONLY MONO SUPPORT!!!!!
    //0 pad somewhere in here -- does sigpack do it automatically?
    currentOutputBuffer[0].fill(arma::fill::zeros);
    currentOutputBuffer[1].fill(arma::fill::zeros);
    //std::cout << "filtering" << std::endl;
    time_t tm = time(NULL);
    for(int i = 0; i < CHANNEL_COUNT; ++i)
    {
        currentOutputBuffer[0] += currentHrtf[i][0]->filter(currentInputBuffer[i]);
        currentOutputBuffer[1] += currentHrtf[i][1]->filter(currentInputBuffer[i]);
    }
    //std::cout << "filtering done in " << std::to_string(difftime(time(NULL), tm)) << std::endl;
}

void AudioManager::setInitYPR()
{
    initYpr = headtrackerMgr->getYawPitchRoll();
}

void AudioManager::updateYPR()
{
    ypr = headtrackerMgr->getYawPitchRoll();
    ypr.yaw = ypr.yaw - initYpr.yaw;
    ypr.pitch = ypr.pitch - initYpr.pitch;
    ypr.roll = ypr.roll - initYpr.roll;
}

void AudioManager::updateYPR(int i)
{
    ypr.yaw = (float)i *360/50;
    ypr.pitch = 0;
    ypr.roll = 0;
}

void AudioManager::updateHrtf()
{
    int T, P;
    for(int i = 0; i < CHANNEL_COUNT; ++i)
    {
        T = ((int) (-(ABSOLUTE_SOURCE_POSITIONS[i].yaw - ypr.yaw)/15))%24;
        //minus sign because HRTF are dumb
        if(T < 0) T += 24;
        P = getHrtfElevation(-(ABSOLUTE_SOURCE_POSITIONS[i].pitch - ypr.pitch));//?
        currentHrtf[i][0] = allHrtf[T][P][0];
        currentHrtf[i][1] = allHrtf[T][P][1];
    }
    //std::cout << P << std::endl;
}//fuck it
//approx formula : T = normal-yaw, P=normal-pitch

float findMod(float a, int b) 
{ 
    if (a < 0) 
        a = -a; 
    double mod = a; 
    while (mod >= b) 
        mod = mod - b;
    if (a < 0)
        return -mod + b; 
    return mod;
} 

int AudioManager::getHrtfElevation(float P)
{
    float P2 = P;//findMod(P, 360);
    /*if(P2 < 0) P2 += 360;
    if(P2 > 180) P2 = P2-360;*/
    if(P2 >= 45) return 3;
    else if(P2 <= 45 && P2 >= 30) return 2;
    else if(P2 <= 30 && P2 >= 15) return 1;
    else if(P2 <= -15 && P2 >= -30) return 6;
    else if(P2 <= -30 && P2 >= -45) return 5;
    else if(P2 <= -45) return 4;
    else return 0;
    //return 0;
}//change that

std::string AudioManager::to3LengthString(int n)
{
    std::string s = std::to_string(n);
    return std::string(3 - s.length(), '0') + s;
}