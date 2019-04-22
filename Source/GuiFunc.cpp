/*
  ==============================================================================

    GuiFunc.cpp
    Created: 24 Dec 2018 12:44:39pm
    Author:  Daniel Reiter
	Description:Custom Components that are used to control some of Soundshape's functions.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GuiFunc.h"

//==============================================================================
//  volumeBox contains slider for volume and draws box and shapes for volume controls
//==============================================================================
volumeBox::volumeBox(AudioProcessorValueTreeState& _valueTreeState)
{
	// slider to control volume
	volume = new Slider();
    gainAttachment.reset(new SliderAttachment(_valueTreeState, "gain", *volume));
	addAndMakeVisible(volume);
	volume->setComponentID((String)VOLUME_SLIDER);
	volume->setSliderStyle(Slider::LinearHorizontal);
	volume->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	volume->setTooltip("Controls the volume of the whole sound");
}
volumeBox::~volumeBox()
{
	free(volume);
}

void volumeBox::paint(Graphics& g)
{
	// paints the graphics surounding the volume slider
	g.fillAll(findColour(SoundshapeLAFs::base2ID));
	g.setColour(findColour(SoundshapeLAFs::base1ID));
	g.drawRect(getLocalBounds(), 1);
	Path tri1, tri2, squ, squ1;
	float h = getHeight() * .25f;
	tri1.addTriangle(h, getHeight() / 2, 3*h, getHeight() - h,  3*h, h );
	tri2.addTriangle(getWidth() - h, h, getWidth() - h, getHeight() - h, getWidth() - (3 * h), getHeight() / 2);
	squ.addRectangle(getWidth() - (7 / 2 * h), (getHeight() / 2) - (h / 2), h, h);
	squ1.addRectangle((3/2 * h), (getHeight() / 2) - (h / 2), h, h);
	g.fillPath(tri1);
	g.fillPath(tri2);
	g.fillPath(squ);
	g.fillPath(squ1);

	// sets slider bounds
	volume->setBounds(3.5f * h, 0.0f, getWidth() - (h*6), getHeight());
	

}

void volumeBox::resized()
{
}

// functions sets a listener to the volume slider
void volumeBox::setVolumeListener(Slider::Listener * _listener)
{
	volume->addListener(_listener);
}
//==============================================================================
// end volumeBox funtions
//==============================================================================

//==============================================================================
//  Fuction GuiFunc add and draw buttons for soundshap's play, export, and panic 
//		controls and volume slider
//==============================================================================
GuiFunc::GuiFunc(AudioProcessorValueTreeState& _valueTreeState) : valueTreeState(_valueTreeState)
{

	// add panic button
	panicBtn = new TextButton("Panic");
	panicBtn->setClickingTogglesState(false);
	panicBtn->setComponentID((String)PANIC_BUTTON);
	panicBtn->setTooltip("Halts all audio");

	// add export button
	exportBtn = new TextButton("Export");
	exportBtn->setComponentID((String)EXPORT_BUTTON);
	exportBtn->setTooltip("Saves the sound to a specified file location");

	// add import button
	importBtn = new TextButton("Import");
	importBtn->setComponentID((String)IMPORT_BUTTON);
	importBtn->setTooltip("Loads a sound from a specified file location");

	// add play button
	playBtn = new TextButton("Play");
	playBtn->setComponentID((String)PLAY_BUTTON);
	playBtn->setTooltip("Plays the sound until button is released"); // is this right?

	// add sustained play button
	sustainPlyBtn = new TextButton("Sustained Play");
	sustainPlyBtn->setComponentID((String)SUSTAIN_PLAY_BUTTON);
	sustainPlyBtn->setClickingTogglesState(true);
	sustainPlyBtn->setTooltip("Plays the sound until button is pressed again"); // is this right?

	// add volume slider component
	volBox = new volumeBox(valueTreeState);

	addAndMakeVisible(exportBtn);
	addAndMakeVisible(importBtn);
	addAndMakeVisible(panicBtn);
	addAndMakeVisible(playBtn);
	addAndMakeVisible(sustainPlyBtn);
	addAndMakeVisible(volBox);
}

GuiFunc::~GuiFunc()
{
    free(volBox);
	free(panicBtn);
	free(exportBtn);
	free(playBtn);
	free(importBtn);
	free(sustainPlyBtn);
}

void GuiFunc::paint(Graphics& g)
{
	// define areas to put sub components 
	auto area = getLocalBounds();
	float yMargin = getHeight() * .1f;
	auto buttonArea = area.removeFromTop((getHeight() * 2) / 3);
	auto playArea = buttonArea.removeFromRight(getWidth() / 3);
	//buttonArea.reduce(2 * yMargin, 0.0f);
	buttonArea.reduce(yMargin/2, 0.0f);
	float h = buttonArea.getHeight() / 3;

	// set location of the panic button
	auto panicArea = buttonArea.removeFromTop(h);
	panicBtn->setBounds(panicArea.removeFromLeft(panicArea.getWidth()/2));

	// set location of the sustained play button
	sustainPlyBtn->setBounds(panicArea);

	// set location of the export button
	buttonArea.removeFromTop(h / 2);
	auto importArea = buttonArea.removeFromTop(h);
	float xMargin = (importArea.getWidth() / 2);
	exportBtn->setBounds(importArea.removeFromRight(xMargin));

	// set location of the export button
	importBtn->setBounds(importArea);

	// set location of the play button
	playArea.removeFromBottom(buttonArea.getHeight());
	playArea.reduce(buttonArea.getHeight() / 2, 0.0f);
	playBtn->setBounds(playArea);

	// set location of the play button
	volBox->setBounds(area.removeFromTop(h));
}

void GuiFunc::resized()
{

}

// Passes reference for button and slider listeners
void GuiFunc::setListeners(Slider::Listener* _sliderListener, Button::Listener* _buttonListener)
{
	sListen = _sliderListener;
	bListen = _buttonListener;

	panicBtn->addListener(bListen);
	exportBtn->addListener(bListen);
	importBtn->addListener(bListen);
	playBtn->addListener(bListen);
	sustainPlyBtn->addListener(bListen);
	volBox->setVolumeListener(sListen);
}
//==============================================================================
// end GuiFunc funtions
//==============================================================================


//==============================================================================
// accessor for whether the sustain play button is pressed
//==============================================================================
bool GuiFunc::getSustainedPlayBtnToggled() {
    return sustainPlyBtn->getToggleState();
}


//==============================================================================
// Functions for drawing and controling the fundumental frequency
//==============================================================================
fundFreq::fundFreq()
{
	num = 0;       // variable tracks current index for a note

	// text box to display current note in fundamental frequency
	txtBox = new Label("fundFreq", "A");
	updateText();

	fundFreqSlider = new Slider(Slider::IncDecButtons, Slider::NoTextBox);
	fundFreqSlider->setComponentID((String)FUND_FREQ_SLIDER);
	fundFreqSlider->setRange(0.0f, 12.0f, 1.0);
	fundFreqSlider->setValue(num, sendNotificationAsync);
	fundFreqSlider->setTooltip("Set the fundamental frequency.");
	fundFreqSlider->onValueChange = [this] 
	{
		num = fundFreqSlider->getValue();
		num = num % 12;
		fundFreqSlider->setValue(num, sendNotificationAsync);
		updateText();
	};

	addAndMakeVisible(txtBox);
	addAndMakeVisible(fundFreqSlider);

}

fundFreq::~fundFreq()
{
	free(txtBox);
	free(fundFreqSlider);

}

void fundFreq::paint(Graphics & g)
{
	//  sets background color
	Colour bgColour = findColour(SoundshapeLAFs::base2ID);
	g.fillAll(bgColour);   // clear the background
	
	// draws outline of the component
	g.setColour(findColour(SoundshapeLAFs::base1ID));
	g.drawRect(getLocalBounds(), 1);

	// set text box location
	txtBox->setBounds(0.0f, 0.0f, getWidth() * 7 / 12, getHeight());
	txtBox->setJustificationType(Justification::centred);

	auto sliderArea = Rectangle<int>(getWidth() * 7.0f / 12.0f, 0.0f, getWidth() * 5 / 12, getHeight());
	fundFreqSlider->setBounds(sliderArea);

}

void fundFreq::resized()
{


}

// function update text box to display current note selected
void fundFreq::updateText()
{
	switch (num)
	{
	case 0:
		txtBox->setText("A", dontSendNotification);
		break;
	case 1:
		txtBox->setText("A#", dontSendNotification);
		break;
	case 2:
		txtBox->setText("B", dontSendNotification);
		break;
	case 3:
		txtBox->setText("C", dontSendNotification);
		break;
	case 4:
		txtBox->setText("C#", dontSendNotification);
		break;
	case 5:
		txtBox->setText("D", dontSendNotification);
		break;
	case 6:
		txtBox->setText("D#", dontSendNotification);
		break;
	case 7:
		txtBox->setText("E", dontSendNotification);
		break;
	case 8:
		txtBox->setText("F", dontSendNotification);
		break;
	case 9:
		txtBox->setText("F#", dontSendNotification);
		break;
	case 10:
		txtBox->setText("G", dontSendNotification);
		break;
	default:
		txtBox->setText("G#", dontSendNotification);
		break;

	}
}

// returns the index of the note currently seleced
int fundFreq::getNote()
{
	return num;
}

// adds a button listener to listen for click events
void fundFreq::setListener(Slider::Listener *_listener)
{
	sListener = _listener;
	fundFreqSlider->addListener(sListener);
}


//==============================================================================
// End fundFreq functions
//==============================================================================
