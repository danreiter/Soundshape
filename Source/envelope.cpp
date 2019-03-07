/*
  ==============================================================================

    envelope.cpp
    Created: 24 Dec 2018 1:00:17pm
    Author:  danre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "envelope.h"

//==============================================================================
envelope::envelope()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	Attack = new Slider();
	Decay = new Slider();
	Sustain = new Slider();
	Release = new Slider();

	addAndMakeVisible(Attack);
	addAndMakeVisible(Decay);
	addAndMakeVisible(Sustain);
	addAndMakeVisible(Release);

	Attack->setRange(0, 100, 1.0);
	Attack->setSliderStyle(Slider::LinearVertical);
	Attack->setColour(Slider::trackColourId, Colours::orange);
	Attack->setColour(Slider::thumbColourId, Colours::orange);
	Attack->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	Attack->setTextValueSuffix("Attack");
	Attack->setTooltip("Controls how fast the sound reaches its max volume");

	Decay->setRange(0, 100, 1.0);
	Decay->setSliderStyle(Slider::LinearVertical);
	Decay->setColour(Slider::trackColourId, Colours::orange);
	Decay->setColour(Slider::thumbColourId, Colours::orange);
	Decay->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	Decay->setTextValueSuffix("Decay");
	Decay->setTooltip("Controls how fast the sound goes from max volume to sustain volume");

	Sustain->setRange(0, 100, 1.0);
	Sustain->setSliderStyle(Slider::LinearVertical);
	Sustain->setColour(Slider::trackColourId, Colours::orange);
	Sustain->setColour(Slider::thumbColourId, Colours::orange);
	Sustain->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	Sustain->setTextValueSuffix("Sustain");
	Sustain->setTooltip("Controls the volume of the sound while a note is held");

	Release->setRange(0, 100, 1.0);
	Release->setSliderStyle(Slider::LinearVertical);
	Release->setColour(Slider::trackColourId, Colours::orange);
	Release->setColour(Slider::thumbColourId, Colours::orange);
	Release->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	Release->setTextValueSuffix("Release-");
	Release->setTooltip("Controls how fast the sound fades out when a note is released");



}

envelope::~envelope()
{
}

void envelope::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
	g.fillAll(Colours::darkgrey);

    g.setColour (Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (getHeight() * .15f);
    g.drawText ("ADSR Envelope", getLocalBounds(),
                Justification::centredTop, true);   // draw some placeholder text
	auto area = getLocalBounds().removeFromLeft(getWidth()/5);
	float margin = area.getHeight() * .15f;
	Line<float> l1(area.getWidth() / 2.0f, margin, area.getWidth() / 2.0f, area.getHeight() - margin);
	Line<float> l2(area.getWidth() * .2f, margin, area.getWidth() * .8f, margin);
	Line<float> l3(area.getWidth() * .2f, area.getHeight() - margin, area.getWidth() * .8f, area.getHeight() - margin);

	area.reduce(0.0f, area.getHeight() * .08f);
	g.setFont(getHeight() *.08f);
	g.drawText("Max", area, Justification::centredTop, true);
	g.drawText("Min", area, Justification::centredBottom, true);
	g.drawLine(l1);
	g.drawLine(l2);
	g.drawLine(l3);
	g.setFont(getHeight() *.05f);
	Rectangle<float> scaleArea(area.getWidth() * .3f, margin, area.getWidth() * .4f, l1.getLength());
	area.reduce(area.getWidth() * .2f, area.getHeight() * .03f);
	area.removeFromBottom(margin);
	float tick = l1.getLength() / 10.0f;
	float w = area.getWidth() * .3f;
	for (int i = 0; i < 100; i += 10)
	{
		String temp = std::to_string(i);
		g.drawText(temp, scaleArea, Justification::bottomLeft, true);
		scaleArea.removeFromBottom(tick);
		g.drawLine(scaleArea.getRight(), scaleArea.getBottom(), scaleArea.getWidth(), scaleArea.getBottom());
		
	}

	float wid = getWidth() / 5.0f;
	g.setFont(margin*.75f);
	Rectangle<float> labelArea(wid, getHeight() - margin, getWidth() - wid, margin);
	Attack->setBounds(wid, (getHeight()*.15f) / 2, getWidth() * .1f, getHeight() - margin);
	Decay->setBounds(wid * 2, (getHeight()*.15f) / 2, getWidth() * .1f, getHeight() - margin);
	Sustain->setBounds(wid * 3, (getHeight()*.15f) / 2, getWidth() * .1f, getHeight() - margin);
	Release->setBounds(wid * 4, (getHeight()*.15f) / 2, getWidth() * .1f, getHeight() - margin);
	g.drawText("Attack", labelArea.removeFromLeft(wid), Justification::centredLeft, true);
	g.drawText("Decay", labelArea.removeFromLeft(wid), Justification::centredLeft, true);
	g.drawText("Sustain", labelArea.removeFromLeft(wid), Justification::centredLeft, true);
	g.drawText("Release", labelArea.removeFromLeft(wid), Justification::centredLeft, true);
}

void envelope::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	//emptyList();
	auto area = getLocalBounds();
	area.reduce(0.0f,(getHeight() * .15f)/2);
	area.removeFromLeft(getWidth() / 5);
	auto rec1 = area.removeFromLeft(getWidth() / 2.5f);
	area.removeFromLeft(getWidth() / 2.5f);
	auto rec2 = area.removeFromLeft(getWidth() / 2.5f);
	area.removeFromLeft(getWidth() / 2.5f);
	/*auto * attack = createSlider(new Slider());
	attack->setRange(0, 100, 1.0);
	attack->setSliderStyle(Slider::LinearVertical);*/
	//attack->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	//attack->setBounds(getWidth() / 5, getHeight() * .15f, area.getWidth() / 5, area.getHeight());
	//Attack->setBounds(rec1);
	//area.removeFromLeft(getWidth() / 2.5f);
	//attack->setTextValueSuffix("Attack");

	//attack = createSlider(new Slider());
	//attack->setRange(0, 100, 1.0);
	//attack->setSliderStyle(Slider::LinearVertical);
	//Decay->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	//Decay->setBounds(getWidth() / 5, getHeight() * .15f, area.getWidth() / 5, area.getHeight());
	//Decay->setBounds(rec2);
	//area.removeFromLeft(getWidth() / 2.5f);
	//attack->setTextValueSuffix("Decay");
}
