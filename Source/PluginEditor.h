/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MainComponent.h"

//==============================================================================
/**
*/
class Soundshape_pluginAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    Soundshape_pluginAudioProcessorEditor (Soundshape_pluginAudioProcessor&, AudioProcessorValueTreeState& _valueTreeState);
    ~Soundshape_pluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    MainComponent mainComp;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Soundshape_pluginAudioProcessor& processor;

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Soundshape_pluginAudioProcessorEditor)
};
