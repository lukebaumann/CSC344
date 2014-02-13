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
class HighPassFilterAudioProcessorEditor  : public AudioProcessorEditor,
                                    public Slider::Listener,
                                    public Button::Listener,
                                    public Timer
{
public:
    HighPassFilterAudioProcessorEditor (HighPassFilterAudioProcessor* ownerFilter);
    ~HighPassFilterAudioProcessorEditor();

    
    //==============================================================================
    void timerCallback() override;
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider*) override;
    void buttonClicked (Button*) override;
    
private:
    Label infoLabel, highPassFrequencyLabel;
    Slider highPassFrequencySlider;
    ToggleButton highPassFilterEnabledButton;

 ScopedPointer<ResizableCornerComponent> resizer;
    ComponentBoundsConstrainer resizeLimits;
    
    AudioPlayHead::CurrentPositionInfo lastDisplayedPosition;
    
    HighPassFilterAudioProcessor* getProcessor() const
    {
        return static_cast <HighPassFilterAudioProcessor*> (getAudioProcessor());
    }
    
    void displayPositionInfo (const AudioPlayHead::CurrentPositionInfo& pos);
};


#endif  // PLUGINEDITOR_H_INCLUDED