/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OSC_Spect_RecieverAudioProcessorEditor::OSC_Spect_RecieverAudioProcessorEditor (OSC_Spect_RecieverAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    addAndMakeVisible(portLabel);
    portLabel.setText("PORT: ", juce::dontSendNotification);
    portLabel.attachToComponent(&portInputLabel, true);
    portLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(portInputLabel);
    portInputLabel.setEditable(true);
    juce::Value portValue = valueTreeState.getParameterAsValue("port");
    juce::String initalPort = portValue.toString().dropLastCharacters(2);
    portInputLabel.setText(initalPort, juce::dontSendNotification);
    portInputLabel.onTextChange = [this] {
        audioProcessor.setOSCPort(portInputLabel.getText().getIntValue());
    };
    
    addAndMakeVisible(addressLabel);
    addressLabel.setText("ADDRESS: ", juce::dontSendNotification);
    addressLabel.attachToComponent(&addressInputLabel, true);
    addressLabel.setJustificationType(juce::Justification::right);
    addAndMakeVisible(addressInputLabel);
    addressInputLabel.setEditable(true);
    addressInputLabel.onTextChange = [this] {
        audioProcessor.setOSCAddress(addressInputLabel.getText().toRawUTF8());
    };
    
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
}

void OSC_Spect_RecieverAudioProcessorEditor::resized()
{
    portInputLabel.setBounds(100, 20, getWidth() - 20, 20);
    addressInputLabel.setBounds(100, 60, getWidth() - 20, 20);
}
