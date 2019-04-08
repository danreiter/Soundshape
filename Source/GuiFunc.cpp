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
	//claf = new CustomLookAndFeel(); // will be moved to MainComponent
	//SoundshapeLAFs::setDefaultColors(*claf); // ditto

}
volumeBox::~volumeBox(){}

void volumeBox::paint(Graphics& g)
{
	// paints the graphics surounding the volume slider
	g.fillAll(Colour(SoundshapeLAFs::base2ID));
	g.setColour(Colour(SoundshapeLAFs::base1ID));// this may or may not work.
	g.drawRect(getLocalBounds(), 1);
	Path tri1, tri2, squ, squ1;
	float h = getHeight() * .25f;
	//tri1.addTriangle(h, h, h, getHeight() - h, 3*h, getHeight()/2);
	tri1.addTriangle(h, getHeight() / 2, 3*h, getHeight() - h,  3*h, h );
	tri2.addTriangle(getWidth() - h, h, getWidth() - h, getHeight() - h, getWidth() - (3 * h), getHeight() / 2);
	squ.addRectangle(getWidth() - (7 / 2 * h), (getHeight() / 2) - (h / 2), h, h);
	squ1.addRectangle((3/2 * h), (getHeight() / 2) - (h / 2), h, h);
	g.fillPath(tri1);
	g.fillPath(tri2);
	g.fillPath(squ);
	g.fillPath(squ1);

	// sets slider bounds
	//volume->setColour(Slider::thumbColourId, Colours::orange);
	//volume->setColour(Slider::trackColourId, Colours::orange);
	volume->setBounds(3.5f * h, 0.0f, getWidth() - (h*6), getHeight());
	//volume->setLookAndFeel(claf);

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
	//emptyList();

	// add panic button
	panicBtn = new TextButton("Panic");
	panicBtn->setClickingTogglesState(false);
	panicBtn->setComponentID((String)PANIC_BUTTON);
	/*panicBtn->setColour(TextButton::textColourOffId, Colours::black);
	panicBtn->setColour(TextButton::textColourOnId, Colours::white);
	panicBtn->setColour(TextButton::buttonColourId, Colours::red);
	panicBtn->setColour(TextButton::buttonOnColourId, Colours::orange);*/
	panicBtn->setTooltip("Halts all audio");

	// add export button
	exportBtn = new TextButton("Export");
	exportBtn->setComponentID((String)EXPORT_BUTTON);
	/*exportBtn->setColour(TextButton::textColourOffId, Colours::white);
	exportBtn->setColour(TextButton::textColourOnId, Colours::black);
	exportBtn->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	exportBtn->setColour(TextButton::buttonOnColourId, Colours::orange);*/
	exportBtn->setTooltip("Saves the sound to a specified file location");

	// add import button
	importBtn = new TextButton("Import");
	importBtn->setComponentID((String)EXPORT_BUTTON);
	//importBtn->setColour(TextButton::textColourOffId, Colours::white);
	//importBtn->setColour(TextButton::textColourOnId, Colours::black);
	//importBtn->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	//importBtn->setColour(TextButton::buttonOnColourId, Colours::orange);
	importBtn->setTooltip("Loads a sound from a specified file location");

	// add play button
	playBtn = new TextButton("Play");
	playBtn->setComponentID((String)PLAY_BUTTON);
	//playBtn->setColour(TextButton::textColourOffId, Colours::black);
	//playBtn->setColour(TextButton::textColourOnId, Colours::black);
	//playBtn->setColour(TextButton::buttonColourId, Colours::orange);
	//playBtn->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	//playBtn->setColour(TextButton::buttonOnColourId, Colours::red);
	playBtn->setClickingTogglesState(false);
	playBtn->setTooltip("Plays the sound once"); // is this right?

	// add sustained play button
	sustainPlyBtn = new TextButton("Sustained Play");
	sustainPlyBtn->setComponentID((String)SUSTAIN_PLAY_BUTTON);
	/*sustainPlyBtn->setColour(TextButton::textColourOffId, Colours::black);
	sustainPlyBtn->setColour(TextButton::textColourOnId, Colours::black);
	sustainPlyBtn->setColour(TextButton::buttonColourId, Colours::orange);
	sustainPlyBtn->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	sustainPlyBtn->setColour(TextButton::buttonOnColourId, Colours::red);*/
	sustainPlyBtn->setClickingTogglesState(true);
	sustainPlyBtn->setTooltip("Plays the sound on a loop"); // is this right?

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
// Functions for drawing and controling the fundumental frequency
//==============================================================================
fundFreq::fundFreq()
{
	num = 0;       // variable tracks current index for a note

		// text box to display current note in fundamental frequency
	txtBox = new Label("fundFreq", "A");
	txtBox->setColour(Label::textColourId, Colour(SoundshapeLAFs::base2textID));
	updateText();


	// set bounds and location for up button
	upBtn = new DrawableButton("up", DrawableButton::ImageRaw);
	upBtn->setComponentID((String)FUND_FREQ_BUTTON);
	upBtn->setClickingTogglesState(false);
	upBtn->setTooltip("Cycles up the list of keys");
	upBtn->onClick = [this] {
		num = (num++) % 12;
		updateText();
	};


	// set bounds and location for down button
	downBtn = new DrawableButton("down", DrawableButton::ImageRaw);
	downBtn->setComponentID((String)FUND_FREQ_BUTTON);
	downBtn->setClickingTogglesState(false);
	downBtn->setTooltip("Cycles down the list of keys");
	downBtn->onClick = [this] {
		if (num <= 0)
		{
			num = 11;
		}
		else
		{
			num = (num--) % 12;
		}
		updateText();
	};

	addAndMakeVisible(txtBox);
	addAndMakeVisible(upBtn);
	addAndMakeVisible(downBtn);


}

fundFreq::~fundFreq()
{
}

void fundFreq::paint(Graphics & g)
{
	//  sets background color
	Colour bgColour = Colour(SoundshapeLAFs::base2ID);
	g.fillAll(bgColour);   // clear the background
	
	// draws outline of the component
	g.setColour(Colour(SoundshapeLAFs::base1ID));
	g.drawRect(getLocalBounds(), 1);
	Point<float> p1(getWidth()*7.0f/12.0f, 0.0f);
	Point<float> p2(getWidth()*7.0f/12.0f,getBottom());
	Line<float> l(p1, p2);
	Point<float> ph1(getWidth()*7.0f / 12.0f, getHeight() / 2.0f);
	Point<float> ph2(getWidth(), getHeight() / 2.0f);
	Line<float> l1(ph1, ph2);
	g.drawLine(l, 1.0f);
	g.drawLine(l1, 1.0f);

	// set text box location
	txtBox->setBounds(0.0f, 0.0f, getWidth() * 7 / 12, getHeight());
	txtBox->setJustificationType(Justification::centred);

	// draw paths for up and down buttons for fundemantal frequency
	DrawablePath upNormal, downNormal, upOver, downOver, upClicked, downClicked;
	Path upPath, downPath;
	auto downArea = Rectangle<int>(getWidth() * 7.0f / 12.0f, getHeight() / 2.0f, getWidth() * 5 / 12, getHeight() / 2);
	auto upArea = Rectangle<int>(getWidth() * 7.0f / 12.0f, 0.0f, getWidth() * 5 / 12, getHeight() / 2);
	float xMargin = upArea.getWidth() * .2f;
	float yMargin = upArea.getHeight() *.2f;
	upPath.addTriangle(xMargin, upArea.getHeight() - yMargin, upArea.getWidth() / 2, yMargin, upArea.getWidth() - xMargin, upArea.getHeight() - yMargin);
	downPath.addTriangle(xMargin, yMargin, downArea.getWidth() - xMargin, yMargin, downArea.getWidth() / 2, downArea.getHeight() - yMargin);


	upNormal.setPath(upPath);
	upNormal.setFill(Colour(SoundshapeLAFs::base1ID));
	upNormal.setStrokeFill(Colour(SoundshapeLAFs::base1textID));
	upNormal.setStrokeThickness(.5f);

	upOver.setPath(upPath);
	upOver.setFill(Colour(SoundshapeLAFs::background2ID));
	upOver.setStrokeFill(Colour(SoundshapeLAFs::base1textID));
	upOver.setStrokeThickness(.50f);

	upClicked.setPath(upPath);
	upClicked.setFill(Colour(SoundshapeLAFs::base1ID));
	upClicked.setStrokeFill(Colour(SoundshapeLAFs::base1textID));
	upClicked.setStrokeThickness(.50f);

	downNormal.setPath(downPath);
	downNormal.setFill(Colour(SoundshapeLAFs::base1ID));
	downNormal.setStrokeFill(Colour(SoundshapeLAFs::base1textID));
	downNormal.setStrokeThickness(.50f);

	downOver.setPath(downPath);
	downOver.setFill(Colour(SoundshapeLAFs::background2ID));
	downOver.setStrokeFill(Colour(SoundshapeLAFs::base1textID));
	downOver.setStrokeThickness(.50f);

	downClicked.setPath(downPath);
	downClicked.setFill(Colour(SoundshapeLAFs::base1ID));
	downClicked.setStrokeFill(Colour(SoundshapeLAFs::base1textID));
	downClicked.setStrokeThickness(.50f);


	// set up button loaction and shape
	upBtn->setImages(&upNormal, &upOver, &upClicked);
	upBtn->setBounds(upArea);



	// set down button location and shape
	downBtn->setImages(&downNormal, &downOver, &downClicked);
	downBtn->setBounds(downArea);

}

void fundFreq::resized()
{
	// sets component list to empty

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
void fundFreq::setListener(Button::Listener *_listener)
{
	bListener = _listener;

	upBtn->addListener(bListener);
	downBtn->addListener(bListener);
}


//==============================================================================
// End fundFreq functions
//==============================================================================
