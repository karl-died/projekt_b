/*
  ==============================================================================

    MyOSCListener.h
    Created: 1 Nov 2025 12:08:53pm
    Author:  Karl Diedrichsen

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MyOSCListener : public juce::Component,
                      private juce::OSCReceiver,
                      private juce::OSCReceiver::ListenerWithOSCAddress<juce::OSCReceiver::MessageLoopCallback>
{
public:
    MyOSCListener(int portNumber, const char* address, std::function<void(const juce::OSCMessage&)> callback)
    {
        this->portNumber = portNumber;
        this->address = address;
        this->callback = callback;
        connect(portNumber);
        addListener(this, address);
    }
    ~MyOSCListener()
    {
        disconnect();
    }
    
    void oscMessageReceived (const juce::OSCMessage& message) override
    {
        callback(message);
    }
    
private:
    int portNumber = 7000;
    const char* address;
    std::function<void(const juce::OSCMessage&)> callback;
};
