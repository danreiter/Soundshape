/*
  ==============================================================================

	GuiFunc.cpp
	Created: 24 Dec 2018 12:44:39pm
	Author:  Daniel Reiter.

  ==============================================================================
*/

#include "MainComponent.h"

// Id numbers passed to sub components
#define GUI_ID 10000
#define FUN_FEQ_ID 10001
#define FEQ_WINDOW_ID 13000
#define BIG_WINDOW_ID 14000
#define SMALL_WINDOW_ID 15000
#define ENVOLOPE_ID 16000
#define ATTACK_ID 16001
#define DECAY_ID 16002
#define SUSTAIN_ID 16003
#define RELEASE_ID 16004
#define VOLUME_ID 17001
#define PLAY_ID 7002
#define PANIC_ID 7003



//==============================================================================
//  Component declares and instaites other gui components and passes variables from 
//==============================================================================
MainComponent::MainComponent()
{
	//----------Default settings----------------------------------

	amp = 0.0f;
	freq = 0;
	add = -1;
	harm = -1;
	zoom = 4.0;
	timeBlock = 0;
	selectedProfile = 0;



	//------------------------------------------------------------

	//------Passing references to child components----------------

	//fWindow.setZoom(&zoom, &harm, &add, this, this, (converter->getProfile(selectedProfile)), 4000);
	sTWindow.setTimeDomain(&timeBlock, &selectedProfile, &timeSize, this);
	bTWindow.setProfile(&timeBlock, &selectedProfile, &timeSize, this);

	//------------------------------------------------------------

	//addAndMakeVisible(fWindow);
	addAndMakeVisible(sTWindow);
	addAndMakeVisible(bTWindow);
	addAndMakeVisible(fund);
	addAndMakeVisible(volComp);
	addAndMakeVisible(enve);
	addAndMakeVisible(cb);

	//-----Setting testing values for the combo box-------------

	cb.addItem("Gregory's game tune", 1);
	cb.addItem("Daniel's groovy sound", 2);
	cb.addItem("Mardigon's math sound", 3);
	cb.addItem("My rad sound", 4);
	cb.addItem("The sound of angry paper", 5);
	cb.addItem("Love sound", 6);
	cb.addItem("The sound of not silence", 7);
	cb.setSelectedItemIndex(3);
	cb.setColour(ComboBox::backgroundColourId, Colours::peachpuff);
	cb.setColour(ComboBox::arrowColourId, Colours::black);
	cb.setColour(ComboBox::textColourId, Colours::black);

	//------------------------------------------------------------

	//------------Setting Button Values---------------------------

	// Harmonic button to toggle harmonic filter for selecting 
	harmonicButton = new TextButton("Harmonic");
	harmonicButton->setClickingTogglesState(true);
	harmonicButton->setColour(TextButton::textColourOffId, Colours::white);
	harmonicButton->setColour(TextButton::textColourOnId, Colours::white);
	harmonicButton->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	harmonicButton->setColour(TextButton::buttonOnColourId, Colours::orange);
	harmonicButton->onClick = [this]
	{
		harm = -1 * (harm);
		repaint();
        
	};

	// Button to add friquency spike to a frequency profile
	addButton = new TextButton("Add");
	addButton->setClickingTogglesState(true);
	addButton->setColour(TextButton::textColourOffId, Colours::white);
	addButton->setColour(TextButton::textColourOnId, Colours::white);
	addButton->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	addButton->setColour(TextButton::buttonOnColourId, Colours::orange);
	//addButton->addListener(this);
	addButton->onClick = [this]
	{
        
		add = -1 * (add);
		repaint();
	};

	// Write button to save sound profiles to the preset folder
	writeButton = new TextButton("Write");
	zoomSlider = new Slider(Slider::IncDecButtons, Slider::TextBoxAbove);
	zoomSlider->setColour(Slider::textBoxBackgroundColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	zoomSlider->setRange(1.0f, 20.0f, .5);
	zoomSlider->setValue(zoom, sendNotificationAsync);
	zoomSlider->setTextValueSuffix(" X");
	zoomSlider->addListener(this);

	//------------------------------------------------------------

	addAndMakeVisible(writeButton);
	addAndMakeVisible(harmonicButton);
	addAndMakeVisible(addButton);
	addAndMakeVisible(zoomSlider);



	setSize(600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint(Graphics& g)
{
	//// (Our component is opaque, so we must completely fill the background with a solid colour)
	//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	g.fillAll(Colours::lightgrey);

}

void MainComponent::resized()
{
	
	auto area = getLocalBounds();
	Point<int> bottomRight(area.getBottomRight());
	float margin = area.getWidth() * 0.01f;
	float h = (area.getHeight() / 5);
	auto sWindow = area.removeFromLeft(area.getWidth() - area.getWidth() / 10.0f);

	// 
	//fWindow.setBounds(sWindow.removeFromTop(h + (2 * h / 3)).reduced(margin));

	sWindow.removeFromTop(2 * margin);
	sTWindow.setBounds(sWindow.removeFromTop(h).reduced(margin));
	sWindow.removeFromLeft(-(area.getWidth() - area.getWidth() / 15.0f, 0));
	sWindow.setWidth(getLocalBounds().getWidth());
	bTWindow.setBounds(sWindow.removeFromTop(h).reduced(margin));

	//Rectangle<float> topButtonArea(fWindow.getBounds().getTopRight().getX(), fWindow.getBounds().getTopRight().getY(), bottomRight.getX() - fWindow.getBounds().getTopRight().getX(), fWindow.getHeight());
	Rectangle<float> topButtonArea(sWindow.getTopRight().getX(), sWindow.getTopRight().getY(), bottomRight.getX() - sWindow.getTopRight().getX(), sWindow.getHeight());
	int h1 = (int)(topButtonArea.getHeight() * .15f);
	topButtonArea.reduce(topButtonArea.getWidth() * .15f, topButtonArea.getHeight() * .15f);
	zoomSlider->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), 2 * h1);
	topButtonArea.removeFromTop((2*h1) + (h1*.5));
	harmonicButton->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), h1);
	topButtonArea.removeFromTop(h1 + (h1*.5));
	addButton->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), h1);

	Rectangle<float> fundFreqArea(sTWindow.getBounds().getTopRight().getX(), sTWindow.getBounds().getTopRight().getY(), bottomRight.getX() - sTWindow.getBounds().getTopRight().getX(), sTWindow.getHeight());
	fundFreqArea.reduce(fundFreqArea.getWidth() * .15f, 0.0f);
	fund.setBounds(fundFreqArea.getX(), fundFreqArea.getY(), fundFreqArea.getWidth(), h1);

	auto lowerRightQuad = sWindow.removeFromLeft(getLocalBounds().getWidth() / 3);
	auto lowerMiddleQuad = sWindow.removeFromLeft(getLocalBounds().getWidth() / 3);
	lowerRightQuad.reduce(lowerRightQuad.getWidth() * .10f, lowerRightQuad.getHeight() * .10f);
	volComp.setBounds(lowerRightQuad);
	
	float h2 = lowerMiddleQuad.getHeight() * .20f;
	lowerMiddleQuad.reduce(lowerMiddleQuad.getWidth() * .05f, lowerMiddleQuad.getHeight() * .20f);
	cb.setBounds(lowerMiddleQuad.removeFromLeft((lowerMiddleQuad.getWidth() * 3) / 4).removeFromTop( h2));
	writeButton->setBounds(lowerMiddleQuad.removeFromTop(h2));
	sWindow.reduce(sWindow.getWidth() * .05f, sWindow.getHeight() * .05f);
	enve.setBounds(sWindow);

}

//------------------------------------------------------------------------------------
// Function listento sliders of child components
void MainComponent::sliderValueChanged(Slider * slider)
{
	
	if (slider->getParentComponent() != this)
	{
		//profile[slider->getComponentID().getIntValue()] = slider->getValue();
		converter->updateProfileBin(currentProfile, slider->getComponentID().getIntValue() , slider->getValue());
	}
	if (slider->getParentComponent() == &volComp)
	{
		//setVolume();
	}
	else
	{
		zoom = slider->getValue();
		repaint();
	}
}
//-------------------------------------------------------------------------------------

void MainComponent::buttonClicked(Button* button)
{
	if (button->getParentComponent() == &sTWindow)
	{
		repaint();
	}
	if (button->getParentComponent() == &bTWindow)
	{

		setProfile(selectedProfile);
		repaint();
	}
	if (button->getParentComponent()->getComponentID() == addButton)
	{
		add = -1 * (add);
		repaint();
	}

}

void MainComponent::setConverter(Converter& _converter)
{
	converter = &_converter;
}

void MainComponent::setProfile(int _index)
{
	//fWindow.setProfile(converter->getProfile(selectedProfile));
}
