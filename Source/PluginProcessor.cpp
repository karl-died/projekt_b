/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OSC_Spect_RecieverAudioProcessor::OSC_Spect_RecieverAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    dataListener = std::make_unique<MyOSCListener>([this] (const juce::OSCMessage& message) {
        oscMessageReceived(message);
    });
    dataListener->connectTo(DEFAULT_PORT);
    //dataListener->listenTo(dataAddress);
    
}

OSC_Spect_RecieverAudioProcessor::~OSC_Spect_RecieverAudioProcessor()
{
}

//==============================================================================
const juce::String OSC_Spect_RecieverAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OSC_Spect_RecieverAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OSC_Spect_RecieverAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OSC_Spect_RecieverAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OSC_Spect_RecieverAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OSC_Spect_RecieverAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OSC_Spect_RecieverAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OSC_Spect_RecieverAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OSC_Spect_RecieverAudioProcessor::getProgramName (int index)
{
    return {};
}

void OSC_Spect_RecieverAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OSC_Spect_RecieverAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OSC_Spect_RecieverAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OSC_Spect_RecieverAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void OSC_Spect_RecieverAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
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
}

//==============================================================================
bool OSC_Spect_RecieverAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OSC_Spect_RecieverAudioProcessor::createEditor()
{
    return new OSC_Spect_RecieverAudioProcessorEditor (*this);
}

//==============================================================================
void OSC_Spect_RecieverAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OSC_Spect_RecieverAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OSC_Spect_RecieverAudioProcessor();
}

void OSC_Spect_RecieverAudioProcessor::oscMessageReceived(const juce::OSCMessage& message)
{
    juce::String pattern = message.getAddressPattern().toString();
    std::cout << pattern;
    for(auto& arg : message)
    {
        if(arg.isFloat32())
            std::cout << ": " << arg.getFloat32();
    }
    std::cout << std::endl;
}

void OSC_Spect_RecieverAudioProcessor::setOSCPort(int port)
{
    dataListener->connectTo(port);
}

void OSC_Spect_RecieverAudioProcessor::setOSCAddress(const char* address)
{
    //dataListener->listenTo(address);
}
