/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#define M_PI 3.14159265358

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
                       ),
parameters(*this, nullptr, Identifier("OSC_Spect_Reciever"),
           {
    std::make_unique<AudioParameterInt> (ParameterID {"port", 1 },
                                           "Port",
                                           0,
                                           1 << 16,
                                           DEFAULT_PORT)
            }),
    noise(std::make_unique<NoiseGenerator>())
#endif
{
    dataListener = std::make_unique<MyOSCListener>([this] (const juce::OSCMessage& message) {
        oscMessageReceived(message);
    });
    
    juce::Value portValue = parameters.getParameterAsValue("port");
    int port = portValue.getValue().toString().getIntValue();
    dataListener->connectTo(port);
    
    amplitudes = std::vector<float>(fftSize / 2, 0.0f);
    fft = std::make_unique<juce::dsp::FFT>(fftOrder);
    fft_in = std::vector<std::complex<float>>(fftSize, 0.0);
    fft_out[0] = std::vector<std::complex<float>>(fftSize, 0.0);
    fft_out[1] = std::vector<std::complex<float>>(fftSize, 0.0);
    
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
    
    projectSampleRate = sampleRate;
    
    for(int i = 0; i < fftSize / 2; i++)
        fft_in[i] = std::polar(amplitudes[i], 0.0f);
    
    for(int i = 1; i < fftSize / 2; i++)
        fft_in[fftSize - i] = std::conj(fft_in[i]);
    
    fft->perform(&fft_in[0], &fft_out[0][0], true);
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
    auto* channel0Data = buffer.getWritePointer (0);

    float d = 1.0 / fftSize;
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
    
        channel0Data[sample]  = sin(M_PI * 0.5 * d * sampleCounter) * fft_out[outIndex][sampleCounter].real();
        channel0Data[sample] += cos(M_PI * 0.5 * d * sampleCounter) * fft_out[(outIndex + 1) % 2][sampleCounter].real();
          
        sampleCounter++;
        if(sampleCounter == fftSize)
        {
            if(amplitudesMutex.try_lock())
            {
                for(int i = 0; i < fftSize / 2; i++)
                    fft_in[i] = std::polar(abs(amplitudes[i]), (float)(2 * M_PI * noise->getNextValue()));
                
                for(int i = 1; i < fftSize / 2; i++)
                    fft_in[fftSize - i] = std::conj(fft_in[i]);
                
                fft->perform(&fft_in[0], &fft_out[outIndex][0], true);
                
                outIndex++;
                outIndex %= 2;
                amplitudesMutex.unlock();
            }
            
            if(holdCounter >= maxHold)
                for(int i = 0; i < fftSize / 2; i++)
                    amplitudes[i] = 0;
            else
                holdCounter++;
            
            sampleCounter = 0;
        }
    }
    
    for (int channel = 1; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for(int sample = 0; sample < buffer.getNumSamples(); sample++)
            channelData[sample] = channel0Data[sample];

    }
}

//==============================================================================
bool OSC_Spect_RecieverAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OSC_Spect_RecieverAudioProcessor::createEditor()
{
    return new OSC_Spect_RecieverAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void OSC_Spect_RecieverAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void OSC_Spect_RecieverAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));

    juce::Value portValue = parameters.getParameterAsValue("port");
    int port = portValue.getValue().toString().getIntValue();
    dataListener->connectTo(port);
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
    
    std::lock_guard<std::mutex> guard(amplitudesMutex);
    std::vector<float> rawAmplitudes(fftSize / 2, 0.0);
    for(int i = 0; i < juce::jmin(message.size(), fftSize / 2); i++)
    {
        if(message[i].isFloat32())
            rawAmplitudes[i] = message[i].getFloat32();
    }
    amplitudes[0] = rawAmplitudes[0];
    float centerFreq = 1000.0f;
    float freqInc = projectSampleRate / (fftSize / 2);
    float centerBin = centerFreq / freqInc;
    float centerIndex = centerBin / (fftSize / 2);
    float exponent = log(0.5) / log(centerIndex);
    float d = 2.0 / fftSize;
    for(int i = 1; i < fftSize / 2; i++)
    {
        float index = (fftSize / 2) * pow(i * d, exponent);
        amplitudes[i] = rawAmplitudes[(int)index];
    }
    holdCounter = 0;
}

void OSC_Spect_RecieverAudioProcessor::setOSCPort(int port)
{
    dataListener->connectTo(port);
    auto portParam = (AudioParameterInt*) parameters.getParameter("port");
    *portParam = port;
}

void OSC_Spect_RecieverAudioProcessor::setOSCAddress(const char* address)
{
    //dataListener->listenTo(address);
}
