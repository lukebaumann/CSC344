/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

const float defaultHighPassFrequency = 440.0f;
const bool defaultHighPassFilterEnabledFlag = false;

//==============================================================================
HighPassFilterAudioProcessor::HighPassFilterAudioProcessor()
    : highPassBuffer(2, 4),
      inputBuffer(2, 4)
{
    // Set up some default values..
    highPassFrequency = defaultHighPassFrequency;
    highPassFilterEnabledFlag = defaultHighPassFilterEnabledFlag;
    
    for (int iter = 0; iter < NUMBER_OF_POLES; iter++) {
        chebyshevPoles[iter] = I * cos(1.0f / NUMBER_OF_POLES * acos(I / 0.5f) + iter * float_Pi / NUMBER_OF_POLES);
    }
    
    lastUIWidth = 400;
    lastUIHeight = 270;
    
    lastPosInfo.resetToDefault();
    highPassPosition = 0;
}

HighPassFilterAudioProcessor::~HighPassFilterAudioProcessor()
{
}

//==============================================================================
const String HighPassFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int HighPassFilterAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float HighPassFilterAudioProcessor::getParameter (int index)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case highPassFrequencyParam:     return highPassFrequency;
        case highPassFilterEnabledParam: return highPassFilterEnabledFlag ? 1.0f : 0.0f;

        default:                        return 0.0f;
    }
}

void HighPassFilterAudioProcessor::setParameter (int index, float newValue)
{
    // This method will be called by the host, probably on the audio thread, so
    // it's absolutely time-critical. Don't use critical sections or anything
    // UI-related, or anything at all that may block in any way!
    switch (index)
    {
        case highPassFrequencyParam:     highPassFrequency = newValue;    break;
        case highPassFilterEnabledParam: highPassFilterEnabledFlag = newValue > 0.5f; break;
        default:                        break;
    }
}

const String HighPassFilterAudioProcessor::getParameterName (int index)
{
    switch (index)
    {
        case highPassFrequencyParam:     return "highPassFrequency";
        case highPassFilterEnabledParam: return "highPassFilterEnabledFlag";
        default:                        break;
    }
    
    return String::empty;
}

const String HighPassFilterAudioProcessor::getParameterText (int index)
{
    return String (getParameter (index), 2);
}

const String HighPassFilterAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String HighPassFilterAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool HighPassFilterAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool HighPassFilterAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool HighPassFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HighPassFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HighPassFilterAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double HighPassFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HighPassFilterAudioProcessor::getNumPrograms()
{
    return 0;
}

int HighPassFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HighPassFilterAudioProcessor::setCurrentProgram (int index)
{
}

const String HighPassFilterAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void HighPassFilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HighPassFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    highPassBuffer.clear();
    inputBuffer.clear();
}

void HighPassFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void HighPassFilterAudioProcessor::reset()
{
    // Use this method as the place to clear any delay lines, buffers, etc, as it
    // means there's been a break in the audio's continuity.
    highPassBuffer.clear();
    inputBuffer.clear();
}

void HighPassFilterAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    int channel;
    // Played with the numbers. Solved: x*y^-1/12 = 22000, x*y^-80/12 = 10000
    float angleToFilter = float_Pi * 2.0 / getSampleRate() * 22270 * pow(1.127f, -highPassFrequency / 12);

    // Go through the incoming data, and apply our gain to it...
    for (channel = 0; channel < getNumInputChannels(); ++channel)
    
    // Apply high pass filter
    for (channel = 0; channel < getNumInputChannels(); ++channel)
    {
        if (highPassFilterEnabledFlag) {
            chebyshevFilter(angleToFilter, buffer, channel);
        }
    }

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

void HighPassFilterAudioProcessor::chebyshevFilter(float angleToFilter, AudioSampleBuffer &buffer, int channel) {
    float pastOutputTemp;
    float pastInputTemp;
    std::complex<float> zPoles[NUMBER_OF_POLES];
    std::complex<float> zZeros[NUMBER_OF_POLES];
    float bottomCoefficients[NUMBER_OF_POLES + 1];
    float topCoefficients[NUMBER_OF_POLES + 1];
    int hPP = highPassPosition, numSamples = buffer.getNumSamples();

    calculateZZeros(zZeros);
    calculateZPoles(angleToFilter, zPoles);
    calculateTopCoefficients(zZeros, topCoefficients);
    calculateBottomCoefficients(zPoles, bottomCoefficients);
    
    float gain = calculateHFGain(zPoles, zZeros);
    
    buffer.applyGain (channel, 0, buffer.getNumSamples(), 1.0 / gain);

    float* channelData = buffer.getSampleData (channel);
    float* highPassData = highPassBuffer.getSampleData(jmin(channel, highPassBuffer.getNumChannels()));
    float* pastInputData = inputBuffer.getSampleData(jmin(channel, inputBuffer.getNumChannels()));
    
    
    for (int i = 0; i < numSamples; ++i) {
        pastInputData[hPP] = channelData[i];
        
        if (hPP >= 4) {
            pastInputTemp = topCoefficients[0] * pastInputData[hPP] + topCoefficients[1] * pastInputData[hPP - 1] + topCoefficients[2] * pastInputData[hPP - 2] + topCoefficients[3] * pastInputData[hPP - 3] + topCoefficients[4] * pastInputData[hPP - 4];
            
            pastOutputTemp = bottomCoefficients[1] * highPassData[hPP - 1] + bottomCoefficients[2] * highPassData[hPP - 2] + bottomCoefficients[3] * highPassData[hPP - 3] + bottomCoefficients[4] * highPassData[hPP - 4];
        }
        // Simple edge case
        else if (hPP == 3) {
            pastInputTemp = topCoefficients[0] * pastInputData[hPP] + topCoefficients[1] * pastInputData[hPP - 1] + topCoefficients[2] * pastInputData[hPP - 2] + topCoefficients[3] * pastInputData[hPP - 3] + topCoefficients[4] * pastInputData[highPassBuffer.getNumSamples() - 1];
            
            pastOutputTemp = bottomCoefficients[1] * highPassData[hPP - 1] + bottomCoefficients[2] * highPassData[hPP - 2] + bottomCoefficients[3] * highPassData[hPP - 3] + bottomCoefficients[4] * highPassData[highPassBuffer.getNumSamples() - 1];
        }
        else if (hPP == 2) {
            pastInputTemp = topCoefficients[0] * pastInputData[hPP] + topCoefficients[1] * pastInputData[hPP - 1] + topCoefficients[2] * pastInputData[hPP - 2] + topCoefficients[3] * pastInputData[highPassBuffer.getNumSamples() - 1] + topCoefficients[4] * pastInputData[highPassBuffer.getNumSamples() - 2];
            
            pastOutputTemp = bottomCoefficients[1] * highPassData[hPP - 1] + bottomCoefficients[2] * highPassData[hPP - 2] + bottomCoefficients[3] * highPassData[highPassBuffer.getNumSamples() - 1] + bottomCoefficients[4] * highPassData[highPassBuffer.getNumSamples() - 2];
        }
        else if (hPP == 1) {
            pastInputTemp = topCoefficients[0] * pastInputData[hPP] + topCoefficients[1] * pastInputData[hPP - 1] + topCoefficients[2] * pastInputData[highPassBuffer.getNumSamples() - 1] + topCoefficients[3] * pastInputData[highPassBuffer.getNumSamples() - 2] + topCoefficients[4] * pastInputData[highPassBuffer.getNumSamples() - 3];
            
            pastOutputTemp = bottomCoefficients[1] * highPassData[hPP - 1] + bottomCoefficients[2] * highPassData[highPassBuffer.getNumSamples() - 1] + bottomCoefficients[3] * highPassData[highPassBuffer.getNumSamples() - 2] + bottomCoefficients[4] * highPassData[highPassBuffer.getNumSamples() - 3];
        }
        else if (hPP == 0) {
            pastInputTemp = topCoefficients[0] * pastInputData[hPP] + topCoefficients[1] * pastInputData[highPassBuffer.getNumSamples() - 1] + topCoefficients[2] * pastInputData[highPassBuffer.getNumSamples() - 2] + topCoefficients[3] * pastInputData[highPassBuffer.getNumSamples() - 3] + topCoefficients[4] * pastInputData[highPassBuffer.getNumSamples() - 4];
            
            pastOutputTemp = bottomCoefficients[1] * highPassData[highPassBuffer.getNumSamples() - 1] + bottomCoefficients[2] * highPassData[highPassBuffer.getNumSamples() - 2] + bottomCoefficients[3] * highPassData[highPassBuffer.getNumSamples() - 3] + bottomCoefficients[4] * highPassData[highPassBuffer.getNumSamples() - 4];
        }
        else {
            assert(false);
        }

        channelData[i] = highPassData[hPP] = pastInputTemp - pastOutputTemp;
        
        if (++hPP >= highPassBuffer.getNumSamples()) {
            hPP = 0;
        }
    }
    
    highPassPosition = hPP;
}

void HighPassFilterAudioProcessor::calculateZPoles(float angleToFilter, std::complex<float> zPoles[]) {
    for (int i = 0; i < NUMBER_OF_POLES; i++) {
        std::complex<float> cTemp = angleToFilter * chebyshevPoles[i];
        zPoles[i] = (1.0f + cTemp / 2.0f) / (1.0f - cTemp / 2.0f);
        zPoles[i] = -zPoles[i];
    }
}

void HighPassFilterAudioProcessor::calculateZZeros(std::complex<float> zZeros[]) {
    for (int i = 1; i < NUMBER_OF_POLES; i++) {
        zZeros[i] = std::complex<float>(1.0, 0.0);
    }
}

void HighPassFilterAudioProcessor::calculateTopCoefficients(std::complex<float> zZeros[], float coefficients[]) {
    coefficients[0] = 1;
    coefficients[1] = ((-zZeros[0]) +
                       (-zZeros[1]) +
                       (-zZeros[2]) +
                       (-zZeros[3])).real();
    coefficients[2] = ((-zZeros[0]) * (-zZeros[1]) +
                       (-zZeros[0]) * (-zZeros[2]) +
                       (-zZeros[0]) * (-zZeros[3]) +
                       (-zZeros[1]) * (-zZeros[2]) +
                       (-zZeros[1]) * (-zZeros[3]) +
                       (-zZeros[2]) * (-zZeros[3])).real();
    coefficients[3] = ((-zZeros[0]) * (-zZeros[1]) * (-zZeros[2]) +
                       (-zZeros[0]) * (-zZeros[1]) * (-zZeros[3]) +
                       (-zZeros[0]) * (-zZeros[2]) * (-zZeros[3]) +
                       (-zZeros[1]) * (-zZeros[2]) * (-zZeros[3])).real();
    coefficients[4] = ((-zZeros[0]) * (-zZeros[1]) * (-zZeros[2]) * (-zZeros[3])).real();
}

void HighPassFilterAudioProcessor::calculateBottomCoefficients(std::complex<float> zPoles[], float coefficients[]) {
    coefficients[0] = 0;
    coefficients[1] = ((-zPoles[0]) +
                       (-zPoles[1]) +
                       (-zPoles[2]) +
                       (-zPoles[3])).real();
    coefficients[2] = ((-zPoles[0]) * (-zPoles[1]) +
                       (-zPoles[0]) * (-zPoles[2]) +
                       (-zPoles[0]) * (-zPoles[3]) +
                       (-zPoles[1]) * (-zPoles[2]) +
                       (-zPoles[1]) * (-zPoles[3]) +
                       (-zPoles[2]) * (-zPoles[3])).real();
    coefficients[3] = ((-zPoles[0]) * (-zPoles[1]) * (-zPoles[2]) +
                       (-zPoles[0]) * (-zPoles[1]) * (-zPoles[3]) +
                       (-zPoles[0]) * (-zPoles[2]) * (-zPoles[3]) +
                       (-zPoles[1]) * (-zPoles[2]) * (-zPoles[3])).real();
    coefficients[4] = ((-zPoles[0]) * (-zPoles[1]) * (-zPoles[2]) * (-zPoles[3])).real();
}

float HighPassFilterAudioProcessor::calculateGain(std::complex<float> frequency, std::complex<float> zPoles[], std::complex<float> zZeros[]) {
    std::complex<float> tempTop = 1.0f;
    std::complex<float> tempBottom = 1.0f;
    
    for (int i = 0; i < NUMBER_OF_POLES; i++) {
        tempTop *= frequency - zZeros[i];
        tempBottom *= frequency - zPoles[i];
    }

    return abs(tempTop / tempBottom);
}

float HighPassFilterAudioProcessor::calculateDCGain(std::complex<float> zPoles[], std::complex<float> zZeros[]) {
    return calculateGain(std::complex<float>(1.0, 0.0), zPoles, zZeros);
}

float HighPassFilterAudioProcessor::calculateHFGain(std::complex<float> zPoles[], std::complex<float> zZeros[]) {
    return calculateGain(std::complex<float>(-1.0, 0.0), zPoles, zZeros);
}

//==============================================================================
bool HighPassFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HighPassFilterAudioProcessor::createEditor()
{
    return new HighPassFilterAudioProcessorEditor (this);
}

//==============================================================================
void HighPassFilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    // Create an outer XML element..
    XmlElement xml ("FILTERSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("uiWidth", lastUIWidth);
    xml.setAttribute ("uiHeight", lastUIHeight);
    xml.setAttribute ("highPassFrequency", highPassFrequency);
    xml.setAttribute ("highPassFilterEnabledFlag", highPassFilterEnabledFlag);
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void HighPassFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
            
            highPassFrequency = (float) xmlState->getDoubleAttribute ("highPassFrequency", highPassFrequency);
            highPassFilterEnabledFlag = (bool) xmlState->getBoolAttribute("highPassFilterEnabledFlag", highPassFilterEnabledFlag);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HighPassFilterAudioProcessor();
}
