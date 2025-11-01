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
private juce::OSCReceiver::Listener<juce::OSCReceiver::RealtimeCallback>
{
public:
    MyOSCListener(std::function<void(const juce::OSCMessage&)> callback)
    {
        this->callback = callback;
        addListener(this);
    }
    ~MyOSCListener()
    {
        removeListener(this);
        disconnect();
    }
    
    void connectTo(int port)
    {
        disconnect();
        if(!connect(port))
        {
            std::cout << "OSC connection failed on port " << port << std::endl;
        }
        else
        {
            std::cout << "OSC connection succeeded on port " << port << std::endl;
        }
    }
    
    void oscMessageReceived (const juce::OSCMessage& message) override
    {
        callback(message);
    }
    
    void oscBundleReceived (const juce::OSCBundle& bundle) override
    {
        for(auto& el : bundle)
            callback(el.getMessage());
    }
    
private:
    std::function<void(const juce::OSCMessage&)> callback;
};
