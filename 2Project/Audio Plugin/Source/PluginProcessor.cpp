/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
///** A sound wrapper class.. */
//class WaveSound : public SynthesiserSound
//{
//public:
//    WaveSound() {}
//    
//    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
//    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
//    
//    virtual int getSoundIndex();
//};

//==============================================================================

/** A demo synth sound that's just a sine wave.. */
class SineWaveSound : public SynthesiserSound
{
public:
    SineWaveSound() {}
    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

//==============================================================================
/** A demo synth sound that's just a square wave.. */
class SquareWaveSound : public SynthesiserSound
{
public:
    SquareWaveSound() {}
    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

//==============================================================================
/** A demo synth sound that's just a triangle wave.. */
class TriangleWaveSound : public SynthesiserSound
{
public:
    TriangleWaveSound() {}
    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

//==============================================================================
/** A demo synth sound that's just a saw tooth wave.. */
class SawToothWaveSound : public SynthesiserSound
{
public:
    SawToothWaveSound() {}
    bool appliesToNote (const int /*midiNoteNumber*/)           { return true; }
    bool appliesToChannel (const int /*midiChannel*/)           { return true; }
};

//==============================================================================
/** A simple demo synth voice that just plays a sine wave.. */
class WaveVoice  : public SynthesiserVoice
{
public:
    WaveVoice()
    : angleDelta (0.0),
    tailOff (0.0),
    soundIndex(0)
    {}
    
    bool canPlaySound (SynthesiserSound* sound)
    {
        return dynamic_cast <SineWaveSound*> (sound) != 0 ||
        dynamic_cast <SquareWaveSound*> (sound) != 0 ||
        dynamic_cast <TriangleWaveSound*> (sound) != 0 ||
        dynamic_cast <SawToothWaveSound*> (sound) != 0;
    }
    
    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* sound, int /*currentPitchWheelPosition*/)
    {
        if (dynamic_cast <const SineWaveSound*> (sound))
        {
            soundIndex = 0;
        }
        else if (dynamic_cast <const SquareWaveSound*> (sound))
        {
            soundIndex = 1;
        }
        else if (dynamic_cast <const TriangleWaveSound*> (sound))
        {
            soundIndex = 2;
        }
        else if (dynamic_cast <const SawToothWaveSound*> (sound))
        {
            soundIndex = 3;
        }
        else {
            soundIndex = 4;
        }
        
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;
        
        double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        double cyclesPerSample = cyclesPerSecond / getSampleRate();
        
        angleDelta = cyclesPerSample * 2.0 * double_Pi;
    }
    
    void stopNote (bool allowTailOff)
    {
        if (allowTailOff)
        {
            // start a tail-off by setting this flag. The render callback will pick up on
            // this and do a fade out, calling clearCurrentNote() when it's finished.
            
            if (tailOff == 0.0) // we only need to begin a tail-off if it's not already doing so - the
                // stopNote method could be called more than once.
                tailOff = 1.0;
        }
        else
        {
            // we're being told to stop playing immediately, so reset everything..
            
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }
    
    void pitchWheelMoved (int /*newValue*/)
    {
        // can't be bothered implementing this for the demo!
    }
    
    void controllerMoved (int /*controllerNumber*/, int /*newValue*/)
    {
        // not interested in controllers in this case.
    }
    
    void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        if (soundIndex == 0) {
            sineRenderNextBlock(outputBuffer, startSample, numSamples);
        }
        else if (soundIndex == 1) {
            squareRenderNextBlock(outputBuffer, startSample, numSamples);
        }
        else if (soundIndex == 2) {
            triangleRenderNextBlock(outputBuffer, startSample, numSamples);
        }
        else if (soundIndex == 3) {
            sawToothRenderNextBlock(outputBuffer, startSample, numSamples);
        }
        else {
            assert(false);
        }
    }
    
    void sineRenderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = sin (currentAngle);
                    const float currentSample = (float) (currentSampleTemp * level * tailOff);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                    
                    tailOff *= 0.99;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = sin (currentAngle);
                    const float currentSample = (float) (currentSampleTemp * level);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                }
            }
        }
    }
    
    void squareRenderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = currentAngle < double_Pi ? 1.0 : -1.0;
                    const float currentSample = (float) (currentSampleTemp * level * tailOff);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                    
                    tailOff *= 0.99;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = currentAngle < double_Pi ? 1.0 : -1.0;
                    const float currentSample = (float) (currentSampleTemp * level);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                }
            }
        }
    }
    
    void triangleRenderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = currentAngle > double_Pi ? 1 - (2 * currentAngle) / double_Pi : (2 * currentAngle - 2 * double_Pi) / double_Pi - 1;
                    const float currentSample = (float) (currentSampleTemp * level * tailOff);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                    
                    tailOff *= 0.99;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = currentAngle > double_Pi ? 1 - (2 * currentAngle) / double_Pi : (2 * currentAngle - 2 * double_Pi) / double_Pi - 1;
                    const float currentSample = (float) (currentSampleTemp * level);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                }
            }
        }
    }
    
    void sawToothRenderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
    {
        if (angleDelta != 0.0)
        {
            if (tailOff > 0)
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = currentAngle / double_Pi / 2;
                    const float currentSample = (float) (currentSampleTemp * level * tailOff);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                    
                    tailOff *= 0.99;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    double currentSampleTemp = currentAngle / double_Pi / 2;
                    const float currentSample = (float) (currentSampleTemp * level);
                    
                    for (int i = outputBuffer.getNumChannels(); --i >= 0;)
                        *outputBuffer.getSampleData (i, startSample) += currentSample;
                    
                    currentAngle = fmod(currentAngle + angleDelta, 2 * double_Pi);
                    ++startSample;
                }
            }
        }
    }
    
private:
    double currentAngle, angleDelta, level, tailOff;
    int soundIndex;
};

const float defaultGain = 1.0f;
const float defaultDelay = 0.5f;
const float defaultWaveType = 0.0f;

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : delayBuffer (2, 12000)
{
    // Set up some default values..
    gain = defaultGain;
    delay = defaultDelay;
    waveType = defaultWaveType;
    
    lastUIWidth = 400;
    lastUIHeight = 270;
    
    lastPosInfo.resetToDefault();
    delayPosition = 0;
    
    // Initialise the synth...
    synth.addSound (getSound());
    
    for (int i = 4; --i >= 0;)
        synth.addVoice (new WaveVoice());   // These voices will play our custom sine-wave sounds..
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{}

//==============================================================================

SynthesiserSound* AudioPluginAudioProcessor::getSound() {
    if (waveType > -0.5 && waveType < 0.5) {
        return new SineWaveSound();
    }
    else if (waveType > 0.5 && waveType < 1.5) {
        return new SquareWaveSound();
    }
    else if (waveType > 1.5 && waveType < 2.5) {
        return new TriangleWaveSound();
    }
    else if (waveType > 2.5 && waveType < 3.5) {
        return new SawToothWaveSound();
    }
    else {
        return new SineWaveSound();
    }
}

const String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int AudioPluginAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float AudioPluginAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     return gain;
        case delayParam:    return delay;
        case waveTypeParam: return waveType;
        default:            return 0.0f;
    }
}

void AudioPluginAudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case gainParam:     gain = newValue;  break;
        case delayParam:    delay = newValue;  break;
        case waveTypeParam:
            waveType = newValue;
            synth.clearSounds();
            synth.addSound(getSound());
            break;
        default:            break;
    }
}

float AudioPluginAudioProcessor::getParameterDefaultValue (int index)
{
    switch (index)
    {
        case gainParam:     return defaultGain;
        case delayParam:    return defaultDelay;
        case waveTypeParam: return defaultWaveType;
        default:            break;
    }
    
    return 0.0f;
}

const String AudioPluginAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case gainParam:     return "gain";
        case delayParam:    return "delay";
        case waveTypeParam: return "waveType";
        default:            break;
    }
    
    return String::empty;
}

const String AudioPluginAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String AudioPluginAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String AudioPluginAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool AudioPluginAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool AudioPluginAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 0;
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioPluginAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void AudioPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate (sampleRate);
    keyboardState.reset();
    delayBuffer.clear();
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

void AudioPluginAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    delayBuffer.clear();
}

void AudioPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    synth.clearSounds();
    synth.addSound(getSound());
    
    const int numSamples = buffer.getNumSamples();
    int channel, dp = 0;
    
    // Go through the incoming data, and apply our gain to it...
    for (channel = 0; channel < getNumInputChannels(); ++channel)
        buffer.applyGain (channel, 0, buffer.getNumSamples(), gain);
    
    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);
    
    // and now get the synth to process these midi events and generate its output.
    synth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    
    // Apply our delay effect to the new output..
    for (channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getSampleData (channel);
        float* delayData = delayBuffer.getSampleData (jmin (channel, delayBuffer.getNumChannels() - 1));
        dp = delayPosition;
        
        for (int i = 0; i < numSamples; ++i)
        {
            const float in = channelData[i];
            channelData[i] += delayData[dp];
            delayData[dp] = (delayData[dp] + in) * delay;
            if (++dp >= delayBuffer.getNumSamples())
                dp = 0;
        }
    }
    
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
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("gain", gain);
    xml.setAttribute ("delay", delay);
    xml.setAttribute("waveType", waveType);
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
            waveType = (float) xmlState->getDoubleAttribute("waveType", waveType);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}
