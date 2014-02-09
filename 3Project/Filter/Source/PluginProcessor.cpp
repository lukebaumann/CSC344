/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const float defaultGain = 1.0f;
const float defaultDelay = 0.0f;
const float defaultLowPassFrequency = 220.0f;
const bool defaultDelayEnabledFlag = false;
const bool defaultDelayFeedBackEnabledFlag = false;
const bool defaultLowPassFilterEnabledFlag = false;

//==============================================================================
FilterAudioProcessor::FilterAudioProcessor()
    : delayBuffer(2, 12000),
      lowPassBuffer(2, 12000)
{
    // Set up some default values..
    gain = defaultGain;
    delay = defaultDelay;
    lowPassFrequency = defaultLowPassFrequency;
    delayEnabledFlag = defaultDelayEnabledFlag;
    delayFeedBackEnabledFlag = defaultDelayFeedBackEnabledFlag;
    lowPassFilterEnabledFlag = defaultLowPassFilterEnabledFlag;
    
    lastUIWidth = 400;
    lastUIHeight = 270;
    
    lastPosInfo.resetToDefault();
    delayPosition = 0;
    lowPassPosition = 0;
}

FilterAudioProcessor::~FilterAudioProcessor()
{
}

//==============================================================================
const String FilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int FilterAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float FilterAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:                 return gain;
        case delayParam:                return delay;
        case lowPassFrequencyParam:     return lowPassFrequency;
        case delayEnabledParam:         return delayEnabledFlag ? 1.0f : 0.0f;
        case delayFeedBackEnabledParam: return delayFeedBackEnabledFlag ? 1.0f : 0.0f;
        case lowPassFilterEnabledParam: return lowPassFilterEnabledFlag ? 1.0f : 0.0f;

        default:                        return 0.0f;
    }
}

void FilterAudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:                 gain = newValue;  break;
        case delayParam:                delay = newValue;  break;
        case lowPassFrequencyParam:     lowPassFrequency = newValue;  break;
        case delayEnabledParam:         delayEnabledFlag = newValue > 0.5f; break;
        case delayFeedBackEnabledParam: delayFeedBackEnabledFlag = newValue > 0.5f; break;
        case lowPassFilterEnabledParam: lowPassFilterEnabledFlag = newValue > 0.5f; break;
        default:                        break;
    }
}

const String FilterAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:                 return "gain";
        case delayParam:                return "delay";
        case lowPassFrequencyParam:     return "lowPassFrequency";
        case delayEnabledParam:         return "delayEnabledFlag";
        case delayFeedBackEnabledParam: return "delayFeedBackEnabledFlag";
        case lowPassFilterEnabledParam: return "lowPassFilterEnabledFlag";
        default:                        break;
    }
    
    return String::empty;
}

const String FilterAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String FilterAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String FilterAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool FilterAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool FilterAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool FilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double FilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterAudioProcessor::getNumPrograms()
{
    return 0;
}

int FilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterAudioProcessor::setCurrentProgram (int index)
{
}

const String FilterAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void FilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    delayBuffer.clear();
    lowPassBuffer.clear();
}

void FilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void FilterAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    delayBuffer.clear();
    lowPassBuffer.clear();
}

void FilterAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int numSamples = buffer.getNumSamples();
    int channel, dp = 0, lPP = 0, angleToFilter = 0;
    
    // Go through the incoming data, and apply our gain to it...
    for (channel = 0; channel < getNumInputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gain);
    
    // Apply low pass filter
    for (channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getSampleData (channel);
        float* delayData = delayBuffer.getSampleData (jmin (channel, delayBuffer.getNumChannels() - 1));
        float* lowPassData = lowPassBuffer.getSampleData(jmin(channel, lowPassBuffer.getNumChannels()));
        
        dp = delayPosition;
        lPP = lowPassPosition;
        angleToFilter = double_Pi * 2 / getSampleRate() * lowPassFrequency;
        
        for (int i = 0; i < numSamples; ++i)
        {
            const float in = channelData[i];

            if (lowPassFilterEnabledFlag) {
                lowPassData[lPP] = in;
                if (lPP >= 2 && lPP < lowPassBuffer.getNumSamples()) {
                    channelData[i] = lowPassData[lPP] - 2 * cos(angleToFilter) * lowPassData[lPP - 1] + lowPassData[lPP - 2];
                }
                // Simple edge case
                else if (lPP == 1) {
                    channelData[i] = lowPassData[lPP] - 2 * cos(angleToFilter) * lowPassData[lPP - 1] + lowPassData[lowPassBuffer.getNumSamples() - 1];
                }
                else if (lPP == 0) {
                    channelData[i] = lowPassData[lPP] - 2 * cos(angleToFilter) * lowPassData[lowPassBuffer.getNumSamples() - 1] + lowPassData[lowPassBuffer.getNumSamples() - 2];
                }
                else {
                    assert(false);
                }
                
                if (++lPP >= lowPassBuffer.getNumSamples()) {
                    lPP = 0;
                }
            }
            
            if (delayEnabledFlag) {
                float temp = channelData[i];
                channelData[i] += delayData[dp];
                if (delayFeedBackEnabledFlag) {
                    delayData[dp] = (delayData[dp] + temp) * delay;
                }
                else {
                    delayData[dp] = (temp) * delay;
                }
                
                if (++dp >= delayBuffer.getNumSamples())
                    dp = 0;
            }
        }
        
    }
    
    lowPassPosition = lPP;
    delayPosition = dp;

    
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // ask the host for the current time so we can display it...
    AudioPlayHead::CurrentPositionInfo newTime;
    
    if (getPlayHead() != nullptr && getPlayHead()->getCurrentPosition (newTime))
    {
        // Successfully got the current time from the host..
        lastPosInfo = newTime;
    }
    else
    {
        // If the host fails to fill-in the current time, we'll just clear it to a default..
        lastPosInfo.resetToDefault();
    }
}

//==============================================================================
bool FilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
    return new FilterAudioProcessorEditor (this);
}

//==============================================================================
void FilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Create an outer XML element..
    XmlElement xml ("FILTERSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("gain", gain);
    xml.setAttribute ("delay", delay);
    xml.setAttribute ("lowPassFrequency", lowPassFrequency);
    xml.setAttribute ("delayEnabledFlag", delayEnabledFlag);
    xml.setAttribute ("delayFeedBackEnabledFlag", delayFeedBackEnabledFlag);
    xml.setAttribute ("lowPassFilterEnabledFlag", lowPassFilterEnabledFlag);
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void FilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    // This getXmlFromBinary() helper function retrieves our XML from the binary blob..
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr)
    {
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("MYPLUGINSETTINGS"))
        {
            // ok, now pull out our parameters..
            lastUIWidth  = xmlState->getIntAttribute ("uiWidth", lastUIWidth);
            lastUIHeight = xmlState->getIntAttribute ("uiHeight", lastUIHeight);
            
            gain  = (float) xmlState->getDoubleAttribute ("gain", gain);
            delay = (float) xmlState->getDoubleAttribute ("delay", delay);
            lowPassFrequency = (float) xmlState->getDoubleAttribute ("lowPassFrequency", lowPassFrequency);
            delayEnabledFlag = (bool) xmlState->getBoolAttribute("delayEnabledFlag", delayEnabledFlag);
            delayFeedBackEnabledFlag = (bool) xmlState->getBoolAttribute("delayFeedBackEnabledFlag", delayFeedBackEnabledFlag);
            lowPassFilterEnabledFlag = (bool) xmlState->getBoolAttribute("lowPassFilterEnabledFlag", lowPassFilterEnabledFlag);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterAudioProcessor();
}
