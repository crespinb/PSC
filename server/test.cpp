#include "serverManager.h"
#include "headtrackerManager.h"
#include "audioManager.cpp" //linker error fix
#include <iostream>
#include <Windows.h>
#include <time.h>

//#include "JuceHeader.h"

int testHrtf();

int main()
{
    //return 0;
    //std::cout << "started" << std::endl;
    /*ServerManager *mgr = new ServerManager();
    if(mgr->startUp())
    {
        std::cout << "started up on port " << mgr->getPort() << std::endl;
    }
    else
    {
        std::cout << "failed to start up" << std::endl;
    }
    if(mgr->waitForClient())
    {
        std::cout << "got client!" << std::endl;
    }
    else
    {
        std::cout << "failed to get client" << std::endl;
    }*/
    //delete mgr;
    //Sleep(5000);
    /*HeadtrackerManager *hMgr = new HeadtrackerManager();
    hMgr->startUp();
    YawPitchRoll ypr;*/
    /*for(int i = 0; i < 50; ++i)
    {
        if(hMgr->isConnected())
        {
            std::cout << "connexion au headtracker etablie" << std::endl;
            ypr = hMgr->getYawPitchRoll();
            std::cout << "yaw " << ypr.yaw << std::endl;
            std::cout << "pitch " << ypr.pitch << std::endl;
            std::cout << "roll " << ypr.roll << std::endl;
        }
        else
        {
            std::cout << "headtracker non trouvé, nouvel essai dans 5 secondes" << std::endl;
        }
        Sleep(1000);
    }*/
    //juce::AudioBuffer<float>* buffer = new juce::AudioBuffer<float>();
    //delete buffer;
    AudioManager* audioMgr = new AudioManager();
    /*vec x = vec("1 2 0 0 0 4 2 1");
    vec f = vec("1 0 0 0 1 0 0 0");
    sp::FIR_filt<double, double, double> *filt = new sp::FIR_filt<double, double, double>();
    filt->set_coeffs(x);
    vec y = filt->filter(f);*/
    //std::cout << y << std::endl;
    //vec a = vec("1 0 0 0 0 0 0 0");
    //std::cout << y+a << std::endl;
    //y += a;
    //std::cout << y << std::endl;
    //while(1);
    /*std::string filename = "D:\\builds\\server\\IRC_1002_C_R0195_T000_P000.wav";
    SoundFileRead hrtfFile(filename.c_str());*/
    //while(1);
    //std::cout << hrtfFile.getChannels() << std::endl;
    //std::cout << hrtfFile.getSamples() << std::endl;
    /*std::cout << hrtfFile.getBitsPerSample() << std::endl;
    for(int i = 0; i < 128; ++i)
    {
        std::cout << "0  " << hrtfFile.getCurrentSample24Bit(0) << std::endl;//channel 0
        std::cout << "1  " << hrtfFile.getCurrentSample24Bit(1) << std::endl;
        hrtfFile.incrementSample();
    }*/
    //Sleep(3000);
    //testHrtf();
    return 0;
}

int testHrtf()
{
    std::cout << "loading hrtf" << std::endl;
    std::string hrtfFilename = "IRC_1002_C_R0195_T090_P000.wav";
    SoundFileRead hrtfFile(hrtfFilename.c_str());
    vec vec0(SAMPLE_COUNT_PER_HRTF);
    vec vec1(SAMPLE_COUNT_PER_HRTF);
    for(int i = 0; i < SAMPLE_COUNT_PER_HRTF; ++i)
    {
        vec0[i] = hrtfFile.getCurrentSampleDouble(0);//channel 0
        vec1[i] = hrtfFile.getCurrentSampleDouble(1);//channel 1
        hrtfFile.incrementSample();
    }
    sp::FIR_filt<double, double, double> *filt0 = new sp::FIR_filt<double, double, double>();
    sp::FIR_filt<double, double, double> *filt1 = new sp::FIR_filt<double, double, double>();
    //vec0[0] = (double) 1;
    //for(int i = 1; i < SAMPLE_COUNT_PER_HRTF; ++i) vec0[i] = (double) 0;
    //vec1[0] = (double) 1;
    //for(int i = 1; i < SAMPLE_COUNT_PER_HRTF; ++i) vec1[i] = (double) 0;
    filt0->set_coeffs(vec0);
    filt1->set_coeffs(vec1);
    std::string inputFilename = "OOT_YoungLink_DinsFire.wav";
    SoundFileRead inputFile(inputFilename.c_str());
    SoundHeader header = inputFile;
    vec input(inputFile.getSamples());
    std::cout << inputFile.getBitsPerSample() << " x " << inputFile.getSamples() << std::endl;
    std::string outputFilename = "OOT_YoungLink_DinsFire-out.wav";
    header.setChannels(2);
    SoundFileWrite outputFile(outputFilename.c_str(), header);
    std::cout << "loading input" << std::endl;
    vec out0;
    vec out1;
    int db;
    for(int interval = 0; interval < 1; ++interval)
    {
        for(int i = 0; i < inputFile.getSamples(); ++i)
        {
            input[i] = inputFile.getCurrentSampleDouble(0);
            inputFile.incrementSample();
            //outputFile.writeSample16Bit(db);
            //std::cout << "wrote " << db << " at pos " << inputFile.getCurrentSampleIndex() << std::endl;
            //std::cout << input[i] << std::endl;
        }
        std::cout << "filtering" << std::endl;
        time_t tm = time(NULL);
        out0 = filt0->filter(input);
        out1 = filt1->filter(input);
        std::cout << "filtering done in " << std::to_string(difftime(time(NULL), tm)) << std::endl;
        std::cout << "writing output" << std::endl;
        for(int i = 0; i < inputFile.getSamples(); ++i)
        {
            outputFile.writeSampleDouble(out0[i]);
            //std::cout << out0[i] << std::endl;
            outputFile.writeSampleDouble(out1[i]);
        }
        //std::cout << "done " << interval << " out of " << inputFile.getSamples()/SAMPLE_COUNT_PER_BUFFER << std::endl;
    }
    delete filt0;
    delete filt1;
    outputFile.close();
    return 0;
}