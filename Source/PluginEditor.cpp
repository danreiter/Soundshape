/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Soundshape_pluginAudioProcessorEditor::Soundshape_pluginAudioProcessorEditor(Soundshape_pluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    setResizeLimits(300, 300, 1920, 1080);
    setSize(700, 600);
    addAndMakeVisible(&mainComponent);


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

    mainComponent.setSize(getWidth(), getHeight());
}
