/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

using namespace juce;

//==============================================================================
/**
*/
class OSC_Spect_RecieverAudioProcessorEditor  : public juce::AudioProcessorEditor
                                                
{
public:
    OSC_Spect_RecieverAudioProcessorEditor (OSC_Spect_RecieverAudioProcessor&, AudioProcessorValueTreeState&);
    ~OSC_Spect_RecieverAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OSC_Spect_RecieverAudioProcessor& audioProcessor;
    AudioProcessorValueTreeState& valueTreeState;
    
    juce::Label portLabel;
    juce::Label portInputLabel;
    
    juce::Label addressLabel;
    juce::Label addressInputLabel;
    
    struct LabeledSlider
    {
        std::string parameterName;
        std::string labelName;
        Label label{};
        Slider slider{};
        std::unique_ptr<SliderAttachment> attachment{};
        
        LabeledSlider(std::string pn, std::string ln) : parameterName(pn), labelName(ln) {}
        LabeledSlider() {}
        LabeledSlider& operator=(const LabeledSlider& other)
        {
            parameterName = other.parameterName;
            labelName = other.labelName;
            return *this;
        }
    };
    
    LabeledSlider minFreqSlider {"minFreq", "Min Frequency"};
    LabeledSlider maxFreqSlider {"maxFreq", "Max Frequency"};
    LabeledSlider skewSlider    {   "skew",          "Skew"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC_Spect_RecieverAudioProcessorEditor)
};
