/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

// As of now the code only supports 4 poles
#define NUMBER_OF_POLES 4

#include "../JuceLibraryCode/JuceHeader.h"
#include <complex.h>
#include<iostream>

//==============================================================================
/**
*/
class LowPassFilterAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    LowPassFilterAudioProcessor();
    ~LowPassFilterAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    
    void zPoleFilter(float angleToFilter, float *lowPassData, float* channelData, float* pastInputData, int numSamples);
    void chebyshevFilter(float angleToFilter, AudioSampleBuffer &buffer, int channel);
    void calculateZPoles(float angleToFilter, std::complex<float> zPoles[]);
    void calculateZZeros(std::complex<float> zZeros[]);
    void calculateTopCoefficients(std::complex<float> zZeros[], float coefficients[]);
    void calculateBottomCoefficients(float angleToFilter, std::complex<float> zPoles[], float coefficients[]);
    float calculateGain(std::complex<float> frequency, std::complex<float> zPoles[], std::complex<float> zZeros[]);
    float calculateDCGain(std::complex<float> zPoles[], std::complex<float> zZeros[]);

    void reset() override;
    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);

    const String getParameterName (int index);
    const String getParameterText (int index);

    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    
    //==============================================================================
    // These properties are public so that our editor component can access them
    // A bit of a hacky way to do it, but it's only a demo! Obviously in your own
    // code you'll do this much more neatly..
    
    // this keeps a copy of the last set of time info that was acquired during an audio
    // callback - the UI component will read this and display it.
    AudioPlayHead::CurrentPositionInfo lastPosInfo;
    
    // these are used to persist the UI's size - the values are stored along with the
    // filter's other parameters, and the UI component will update them when it gets
    // resized.
    int lastUIWidth, lastUIHeight;
    
    //==============================================================================
    enum Parameters
    {
        lowPassFrequencyParam = 0,
        lowPassFilterEnabledParam,
        
        totalNumParams
    };
    
    // For some reason, I cannot go lower than 183 Hz. Overshooting the minimum so I am never close
    float lowPassFrequency;
    bool lowPassFilterEnabledFlag;
    
private:
    //==============================================================================
    AudioSampleBuffer lowPassBuffer, inputBuffer;
    int lowPassPosition;
    std::complex<float> chebyshevPoles[NUMBER_OF_POLES];
    const std::complex<float> I = std::complex<float>(0.0, 1.0);
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowPassFilterAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
