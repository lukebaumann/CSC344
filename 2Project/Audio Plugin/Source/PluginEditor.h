/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class AudioPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                         public Slider::Listener,
                                         public Button::Listener,
                                         public Timer
{
public:
    AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor* ownerFilter);
    ~AudioPluginAudioProcessorEditor();

    //==============================================================================
    void timerCallback() override;
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider*) override;
    void buttonClicked (Button*) override;
    
private:
    MidiKeyboardComponent midiKeyboard;
    Label infoLabel, gainLabel, delayLabel;
    Slider gainSlider, delaySlider;//, waveTypeSlider;
    ToggleButton sineWaveButton, squareWaveButton, triangleWaveButton, sawToothWaveButton,
        FMWaveButton, AMWaveButton, FMixWaveButton;
    ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    
    AudioPlayHead::CurrentPositionInfo lastDisplayedPosition;
    
    AudioPluginAudioProcessor* getProcessor() const
    {
        return static_cast <AudioPluginAudioProcessor*> (getAudioProcessor());
    }
    
    void displayPositionInfo (const AudioPlayHead::CurrentPositionInfo& pos);
};


#endif  // PLUGINEDITOR_H_INCLUDED
