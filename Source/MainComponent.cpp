/*
  ==============================================================================

	GuiFunc.cpp
	Created: 24 Dec 2018 12:44:39pm
	Author:  Daniel Reiter.

  ==============================================================================
*/

#include "MainComponent.h"


float MainComponent::notes[12] = { 27.5f, 29.50f, 30.87f, 16.35f, 17.32f, 18.35f, 19.45f, 20.60f, 21.83f, 23.12f, 24.5f, 25.96 };

// Id numbers passed to sub components

//==============================================================================
//  Component declares and instaites other gui components and passes variables from 
//==============================================================================
MainComponent::MainComponent(Soundshape_pluginAudioProcessor& p):
    processor(p)
{
    setConverter(&(processor.getConverter()));
    
    //----------Default settings----------------------------------
    
	amp = 0.0f;
	freq = 0;
	add = -1;
	harm = -1;
	zoom = 4.0;
	timeBlock = 0;
	selectedProfile = -1;
	timeSize = 10;
	for (int i = 0; i < 4000; i++)
	{
		profile[i] = -2;
	}
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 2048; j++)
		{
			soundWaveProfile[i][j] = 0;
		}
	}

	//------------------------------------------------------------

	//------Passing references to child components----------------
	fWindow.setZoom(&zoom, &harm, &add, this, this, &profile[0], (sizeof(profile) / sizeof(*profile)));
	//fWindow.setZoom(&zoom, &harm, &add, this, this, converterPtr->getFrequencyValue(0,0), (sizeof(profile) / sizeof(*profile)));
	sTWindow.setTimeDomain(&timeBlock, &selectedProfile, &timeSize, this);
	bTWindow.setProfile(&timeBlock, &selectedProfile, &timeSize, this);
	volComp.setListeners(this, this);
	fund.setListener(this);
	enve.setListener(this);

	//------------------------------------------------------------

	addAndMakeVisible(fWindow);
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
    cb.setTooltip("Contains all the current presets");

	//------------------------------------------------------------

	//------------Setting Button Values---------------------------

	// Harmonic button to toggle harmonic filter for selecting 
	harmonicButton = new TextButton("Harmonic");
	harmonicButton->setClickingTogglesState(true);
	harmonicButton->setColour(TextButton::textColourOffId, Colours::white);
	harmonicButton->setColour(TextButton::textColourOnId, Colours::white);
	harmonicButton->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	harmonicButton->setColour(TextButton::buttonOnColourId, Colours::orange);
    harmonicButton->setTooltip("When enabled, the harmonic setting will only allow the creation of new frequency spikes at harmonically correct frequencies");
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
	addButton->addListener(this);
    addButton->setTooltip("Allows the creation of a frequency spike at any available frequency");
	addButton->onClick = [this]
	{
        
		add = -1 * (add);
		repaint();
	};

	// Write button to save sound profiles to the preset folder
	writeButton = new TextButton("Write");
    writeButton->setTooltip("Writes the chosen preset");
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


void MainComponent::setConverter(Converter *_converter) {
	converterPtr = _converter;
}


//==============================================================================
void MainComponent::paint(Graphics& g)
{
	//// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::lightgrey);

	// setting the boundary components for the child components
	auto area = getLocalBounds();
	Point<int> bottomRight(area.getBottomRight());
	float margin = area.getWidth() * 0.01f;
	float h = (area.getHeight() / 5);
	auto sWindow = area.removeFromLeft(area.getWidth() - area.getWidth() / 10.0f);

	// sets the frequency domain window location and bounds
	fWindow.setBounds(sWindow.removeFromTop(h + (2 * h / 3)).reduced(margin));

	sWindow.removeFromTop(2 * margin);

	// sets the small time domain window location and bounds
	sTWindow.setBounds(sWindow.removeFromTop(h).reduced(margin));
	sWindow.removeFromLeft(-(area.getWidth() - area.getWidth() / 15.0f, 0));
	sWindow.setWidth(getLocalBounds().getWidth());

	// sets the big time domain window location and bounds
	bTWindow.setBounds(sWindow.removeFromTop(h).reduced(margin));
	
	// parameters to help layout soundshape's GUI controls (sliders, buttons, and combo boxes, i.e. non-widow components)
	Rectangle<float> topButtonArea(fWindow.getBounds().getTopRight().getX(), fWindow.getBounds().getTopRight().getY(), bottomRight.getX() - fWindow.getBounds().getTopRight().getX(), fWindow.getHeight());
	int h1 = (int)(topButtonArea.getHeight() * .15f);
	topButtonArea.reduce(topButtonArea.getWidth() * .15f, topButtonArea.getHeight() * .15f);

	// sets zoom slider location and bounds
	zoomSlider->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), 2 * h1);
	topButtonArea.removeFromTop((2 * h1) + (h1*.5));

	// sets harmonic button location and bounds
	harmonicButton->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), h1);
	topButtonArea.removeFromTop(h1 + (h1*.5));

	// sets add button location and bounds
	addButton->setBounds(topButtonArea.getX(), topButtonArea.getY(), topButtonArea.getWidth(), h1);

	// sets the fundumental frequency controls location and bounds
	Rectangle<float> fundFreqArea(sTWindow.getBounds().getTopRight().getX(), sTWindow.getBounds().getTopRight().getY(), bottomRight.getX() - sTWindow.getBounds().getTopRight().getX(), sTWindow.getHeight());
	fundFreqArea.reduce(fundFreqArea.getWidth() * .15f, 0.0f);
	fund.setBounds(fundFreqArea.getX(), fundFreqArea.getY(), fundFreqArea.getWidth(), h1);

	// sets the volume control location and bounds
	auto lowerRightQuad = sWindow.removeFromLeft(getLocalBounds().getWidth() / 3);
	auto lowerMiddleQuad = sWindow.removeFromLeft(getLocalBounds().getWidth() / 3);
	lowerRightQuad.reduce(lowerRightQuad.getWidth() * .10f, lowerRightQuad.getHeight() * .10f);
	volComp.setBounds(lowerRightQuad);

	float h2 = lowerMiddleQuad.getHeight() * .20f;
	lowerMiddleQuad.reduce(lowerMiddleQuad.getWidth() * .05f, lowerMiddleQuad.getHeight() * .20f);

	// sets the pre-sets and write button location and bounds
	cb.setBounds(lowerMiddleQuad.removeFromLeft((lowerMiddleQuad.getWidth() * 3) / 4).removeFromTop(h2));
	writeButton->setBounds(lowerMiddleQuad.removeFromTop(h2));

	// sets the envolope location and bounds
	sWindow.reduce(sWindow.getWidth() * .05f, sWindow.getHeight() * .05f);
	enve.setBounds(sWindow);

}

void MainComponent::resized()
{

}

//------------------------------------------------------------------------------------
// Function listens to sliders of child components
void MainComponent::sliderValueChanged(Slider * slider)
{
	// on change of a frequency spike slider updates conveter with new value
	if (slider->getParentComponent()->getComponentID().getIntValue() == FREQ_DOMAIN)
	{
		profile[slider->getComponentID().getIntValue()] = slider->getValue();
        float input = (selectedProfile < 0) ? 0 : selectedProfile;
		converterPtr->updateFrequencyValue(input, slider->getComponentID().getIntValue(), profile[slider->getComponentID().getIntValue()]);
	}
	// on change of zoom slider updates zoom for frequency domain view
	if(slider == zoomSlider)
	{
		zoom = slider->getValue();
		repaint();
	}

	// on change update volume settings
	if (slider->getComponentID().getIntValue() == VOLUME_SLIDER)
	{
		//Needs hook up to back end

		// use slider->getValue()
	}

	// on change update attack settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_ATTACK)
	{
		//Needs set up with back end calls
	}

	// on change update decay settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_DECAY)
	{
		//Needs set up with back end calls
	}

	// on change update release settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_RELEASE)
	{
		//Needs set up with back end calls
	}

	// on change update sustain settings
	if (slider->getComponentID().getIntValue() == ENVELOPE_SUSTAIN)
	{
		//Needs set up with back end calls
	}

}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// Function listens to buttons of child components
void MainComponent::buttonClicked(Button* button)
{
	// On frequnecy profile selection updates new frequency profile 
	if(button->getRadioGroupId() == PROFILE_SELECT_BUTTON)
	{
		repaint();
		DBG(button->getComponentID());
        float val = converterPtr->getFrequencyValue(0, 440);
		DBG(val);
	}

	// On Time domain selection repaint GUI
	if (button->getParentComponent() == &bTWindow)
	{
		repaint();
	}

	// add Button hides and shows buttons to add frequency spikes 
	if (button->getParentComponent()->getComponentID() == addButton)
	{
		//add = -1 * (add);
		repaint();
	}

	// Play button
	if (button->getComponentID().getIntValue() == PLAY_BUTTON)
	{
        if (button->getToggleState() == true) {
            float freq = fund.getNote();
            processor.playFreq(16.0f * notes[fund.getNote()]);
        }
        else {
            processor.panic();
        }
	}

	// Export button
	if (button->getComponentID().getIntValue() == EXPORT_BUTTON)
	{
		// need back end call for export
	}

	// Panic button
	if (button->getComponentID().getIntValue() == PANIC_BUTTON)
	{
		// need back end call for panic
        processor.panic();
	}

	// Funmental fruquency setting buttons
	if (button->getComponentID().getIntValue() == FUND_FREQ_BUTTON)
	{
		// need back end call for fundmental frequency
		// to button->getParentComonent()->getNote() to get the index 
		// for the notes array
		// i.e. - notes[button->getParentComonent()->getNote()]
	}
}
//-------------------------------------------------------------------------------------