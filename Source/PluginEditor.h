/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class OSC_Spect_RecieverAudioProcessorEditor  : public juce::AudioProcessorEditor
                                                
{
public:
    OSC_Spect_RecieverAudioProcessorEditor (OSC_Spect_RecieverAudioProcessor&);
    ~OSC_Spect_RecieverAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OSC_Spect_RecieverAudioProcessor& audioProcessor;
    
    juce::Label portLabel;
    juce::Label portInputLabel;
    
    juce::Label addressLabel;
    juce::Label addressInputLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC_Spect_RecieverAudioProcessorEditor)
};
