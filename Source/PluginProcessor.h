/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MyOSCListener.h"

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

private:
    //=============================================================================
    const int DEFAULT_PORT = 6666;
    const char* dataAddress = "/chan1";
    const char* metaDataAddress = "";
    
    std::unique_ptr<MyOSCListener> dataListener;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC_Spect_RecieverAudioProcessor)
};
