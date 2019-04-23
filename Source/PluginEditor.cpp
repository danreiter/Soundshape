/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Soundshape_pluginAudioProcessorEditor::Soundshape_pluginAudioProcessorEditor(Soundshape_pluginAudioProcessor& p,
    AudioProcessorValueTreeState& _valueTreeState)
    : AudioProcessorEditor(&p), processor(p), mainComp(p, _valueTreeState)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    setResizeLimits(300, 300, 1920, 1080);
    setSize(900, 600);
    addAndMakeVisible(&mainComp);

}

Soundshape_pluginAudioProcessorEditor::~Soundshape_pluginAudioProcessorEditor()
{
}

//==============================================================================
void Soundshape_pluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void Soundshape_pluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    mainComp.setSize(getWidth(), getHeight());
}
