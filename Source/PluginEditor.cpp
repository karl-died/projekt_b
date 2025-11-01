/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OSC_Spect_RecieverAudioProcessorEditor::OSC_Spect_RecieverAudioProcessorEditor (OSC_Spect_RecieverAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
   
    setSize (400, 300);
}

OSC_Spect_RecieverAudioProcessorEditor::~OSC_Spect_RecieverAudioProcessorEditor()
{
}

//==============================================================================
void OSC_Spect_RecieverAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void OSC_Spect_RecieverAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
