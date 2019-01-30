/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Pscjuce0AudioProcessor::Pscjuce0AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Pscjuce0AudioProcessor::~Pscjuce0AudioProcessor()
{
}

//==============================================================================
const String Pscjuce0AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Pscjuce0AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Pscjuce0AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Pscjuce0AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Pscjuce0AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Pscjuce0AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Pscjuce0AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Pscjuce0AudioProcessor::setCurrentProgram (int index)
{
}

const String Pscjuce0AudioProcessor::getProgramName (int index)
{
    return {};
}

void Pscjuce0AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Pscjuce0AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Pscjuce0AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Pscjuce0AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Pscjuce0AudioProcessor::updateFileData(std::string &filename) {
    delete fileInputStream;
    delete file;
    file = new File(filename);
    fileInputStream = new FileInputStream(*file);
}

void Pscjuce0AudioProcessor::playTheFile() {
    const int maxBytesToRead = 64;
    /*while(true)
    {
    }*/
}

void Pscjuce0AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

/*    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    */
    
    simpleAussalAddition(buffer);
}

void Pscjuce0AudioProcessor::simpleAussalAddition(AudioBuffer<float>& buffer)
{
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    auto numberOfSamples = buffer.getNumSamples();
    float S = 0;
    float* writePointer;
    for(auto sample = 0; sample < numberOfSamples; ++sample) {
        S = 0;
        for (auto inputChannel = 0; inputChannel < totalNumInputChannels; ++inputChannel) {
            const float *readPointer = buffer.getReadPointer(inputChannel, sample);
            S += *readPointer;
        }
        for (auto outputChannel = 0; outputChannel < totalNumOutputChannels; ++outputChannel) {
            writePointer = buffer.getWritePointer(outputChannel, sample);
            *writePointer = S;
        }
    }
}

//==============================================================================
bool Pscjuce0AudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Pscjuce0AudioProcessor::createEditor()
{
    return new Pscjuce0AudioProcessorEditor (*this);
}

//==============================================================================
void Pscjuce0AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Pscjuce0AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pscjuce0AudioProcessor();
}
