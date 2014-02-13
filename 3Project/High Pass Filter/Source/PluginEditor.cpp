/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HighPassFilterAudioProcessorEditor::HighPassFilterAudioProcessorEditor (HighPassFilterAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter),
      infoLabel (String::empty),
      highPassFrequencyLabel("", "Low Pass Frequency:"),
      highPassFrequencySlider("highPassFrequency"),
      highPassFilterEnabledButton("highPassFilterEnabled")
{
    // add some sliders..
    addAndMakeVisible (highPassFrequencySlider);
    highPassFrequencySlider.setSliderStyle (Slider::Rotary);
    highPassFrequencySlider.addListener (this);
    highPassFrequencySlider.setEnabled(true);
    highPassFrequencySlider.setRange (1, 79, 1);
    
    highPassFrequencyLabel.attachToComponent (&highPassFrequencySlider, false);
    highPassFrequencyLabel.setFont (Font (11.0f));
    
    // add some toggle buttons for the filters..
    addAndMakeVisible (highPassFilterEnabledButton);
    highPassFilterEnabledButton.setToggleState(false, dontSendNotification);
    highPassFilterEnabledButton.setClickingTogglesState(true);
    highPassFilterEnabledButton.addListener (this);
    
    // add a label that will display the current timecode and status..
    addAndMakeVisible (infoLabel);
    infoLabel.setColour (Label::textColourId, Colours::blue);
    
    // add the triangular resizer component for the bottom-right of the UI
    addAndMakeVisible (resizer = new ResizableCornerComponent (this, &resizeLimits));
    resizeLimits.setSizeLimits (150, 150, 800, 300);
    
    // set our component's initial size to be the last one that was stored in the filter's settings
    setSize (ownerFilter->lastUIWidth,
             ownerFilter->lastUIHeight);
    
    startTimer (50);
}

HighPassFilterAudioProcessorEditor::~HighPassFilterAudioProcessorEditor()
{
}

//==============================================================================
void HighPassFilterAudioProcessorEditor::paint (Graphics& g)
{
    g.setGradientFill (ColourGradient (Colours::white, 0, 0,
                                       Colours::grey, 0, (float) getHeight(), false));
    g.fillAll();
}

void HighPassFilterAudioProcessorEditor::resized()
{
    infoLabel.setBounds (10, 4, 400, 25);
    highPassFilterEnabledButton.setBounds(20, 60, 100, 20);
    highPassFrequencySlider.setBounds(200, 60, 150, 40);

    resizer->setBounds (getWidth() - 16, getHeight() - 16, 16, 16);
    
    getProcessor()->lastUIWidth = getWidth();
    getProcessor()->lastUIHeight = getHeight();
}


//==============================================================================
// This timer periodically checks whether any of the filter's parameters have changed...
void HighPassFilterAudioProcessorEditor::timerCallback()
{
    HighPassFilterAudioProcessor* ourProcessor = getProcessor();
    
    AudioPlayHead::CurrentPositionInfo newPos (ourProcessor->lastPosInfo);
    
    if (lastDisplayedPosition != newPos)
        displayPositionInfo (newPos);

    highPassFrequencySlider.setValue (ourProcessor->getParameter(HighPassFilterAudioProcessor::highPassFrequencyParam), dontSendNotification);
}


// This is our Slider::Listener callback, when the user drags a slider.
void HighPassFilterAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &highPassFrequencySlider)
    {
        getProcessor()->setParameterNotifyingHost (HighPassFilterAudioProcessor::highPassFrequencyParam,
                                                   (float) highPassFrequencySlider.getValue());
    }
}



// This is our Button::Listener callback, when the user drags a slider.
void HighPassFilterAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &highPassFilterEnabledButton)
    {
        // It's vital to use setParameterNotifyingHost to change any parameters that are automatable
        // by the host, rather than just modifying them directly, otherwise the host won't know
        // that they've changed.
        getProcessor()->setParameterNotifyingHost (HighPassFilterAudioProcessor::highPassFilterEnabledParam, highPassFilterEnabledButton.getToggleState() ? 1.0f : 0.0f);
    }
    else
    {
        jassertfalse;
    }
}

//==============================================================================
// quick-and-dirty function to format a timecode string
static const String timeToTimecodeString (const double seconds)
{
    const double absSecs = fabs (seconds);
    
    const int hours =  (int) (absSecs / (60.0 * 60.0));
    const int mins  = ((int) (absSecs / 60.0)) % 60;
    const int secs  = ((int) absSecs) % 60;
    
    String s (seconds < 0 ? "-" : "");
    
    s << String (hours).paddedLeft ('0', 2) << ":"
    << String (mins) .paddedLeft ('0', 2) << ":"
    << String (secs) .paddedLeft ('0', 2) << ":"
    << String (roundToInt (absSecs * 1000) % 1000).paddedLeft ('0', 3);
    
    return s;
}

// quick-and-dirty function to format a bars/beats string
static const String ppqToBarsBeatsString (double ppq, double /*lastBarPPQ*/, int numerator, int denominator)
{
    if (numerator == 0 || denominator == 0)
        return "1|1|0";
    
    const int ppqPerBar = (numerator * 4 / denominator);
    const double beats  = (fmod (ppq, ppqPerBar) / ppqPerBar) * numerator;
    
    const int bar    = ((int) ppq) / ppqPerBar + 1;
    const int beat   = ((int) beats) + 1;
    const int ticks  = ((int) (fmod (beats, 1.0) * 960.0 + 0.5));
    
    String s;
    s << bar << '|' << beat << '|' << ticks;
    return s;
}

// Updates the text in our position label.
void HighPassFilterAudioProcessorEditor::displayPositionInfo (const AudioPlayHead::CurrentPositionInfo& pos)
{
    lastDisplayedPosition = pos;
    String displayText;
    displayText.preallocateBytes (128);
    
    displayText << String (pos.bpm, 2) << " bpm, "
    << pos.timeSigNumerator << '/' << pos.timeSigDenominator
    << "  -  " << timeToTimecodeString (pos.timeInSeconds)
    << "  -  " << ppqToBarsBeatsString (pos.ppqPosition, pos.ppqPositionOfLastBarStart,
                                        pos.timeSigNumerator, pos.timeSigDenominator);
    
    if (pos.isRecording)
        displayText << "  (recording)";
    else if (pos.isPlaying)
        displayText << "  (playing)";
    
    infoLabel.setText (displayText, dontSendNotification);
}