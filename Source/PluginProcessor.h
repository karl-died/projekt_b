/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyOSCListener.h"
#include "NoiseGenerator.h"

using namespace juce;

//==============================================================================
/**
*/
class OSC_Spect_RecieverAudioProcessor  : public juce::AudioProcessor
                                            
{
public:
    //==============================================================================
    OSC_Spect_RecieverAudioProcessor();
    ~OSC_Spect_RecieverAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void oscMessageReceived (const juce::OSCMessage& message);
    
    void setOSCPort(int);
    void setOSCAddress(const char*);

private:
    //=============================================================================
    const int DEFAULT_PORT = 7001;
    const char* dataAddress = "/chan1";
    const char* metaDataAddress = "";
    
    AudioProcessorValueTreeState parameters;
    
    double projectSampleRate = 48000;
    
    std::unique_ptr<MyOSCListener> dataListener;
    
    int fftOrder = 9;
    int fftSize = 1 << fftOrder;
    
    std::mutex amplitudesMutex;
    std::vector<float> amplitudes;
    std::unique_ptr<juce::dsp::FFT> fft;
    std::vector<std::complex<float>> fft_in;
    std::array<std::vector<std::complex<float>>, 2> fft_out;
    
    int outIndex = 0;
    
    std::unique_ptr<NoiseGenerator> noise;
    
    int sampleCounter = 0;
    const std::atomic<int> maxHold = 20;
    std::atomic<int> holdCounter = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC_Spect_RecieverAudioProcessor)
};
