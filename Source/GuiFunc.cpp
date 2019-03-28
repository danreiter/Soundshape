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
}
volumeBox::~volumeBox(){}

void volumeBox::paint(Graphics& g)
{
	// paints the graphics surounding the volume slider
	g.fillAll(Colours::darkgrey);
	g.setColour(Colours::orange);
	g.drawRect(getLocalBounds(), 1);
	Path tri1, tri2, squ;
	float h = getHeight() * .25f;
	tri1.addTriangle(h, h, h, getHeight() - h, 3*h, getHeight()/2);
	tri2.addTriangle(getWidth() - h, h, getWidth() - h, getHeight() - h, getWidth() - (3 * h), getHeight() / 2);
	squ.addRectangle(getWidth() - (7 / 2 * h), (getHeight() / 2) - (h / 2), h, h);
	g.fillPath(tri1);
	g.fillPath(tri2);
	g.fillPath(squ);

	// sets slider bounds
	volume->setColour(Slider::thumbColourId, Colours::orange);
	volume->setColour(Slider::trackColourId, Colours::orange);
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
}

GuiFunc::~GuiFunc()
{
}

void GuiFunc::paint(Graphics& g)
{

}

void GuiFunc::resized()
{
	// clears list of components
	emptyList();

	// define areas to put sub components 
	auto area = getLocalBounds();
	float yMargin = getHeight() * .1f;
	auto buttonArea = area.removeFromTop((getHeight()*2)/3);
	auto playArea = buttonArea.removeFromRight(getWidth()/3);
	buttonArea.reduce(2*yMargin, 0.0f);
	float h = buttonArea.getHeight() / 3;

	// add panic button
	auto * panicBtn = addToList(new TextButton("Panic"));
	panicBtn->setClickingTogglesState(false);
	panicBtn->setComponentID((String)PANIC_BUTTON);
	panicBtn->addListener(bListen);
	panicBtn->setColour(TextButton::textColourOffId, Colours::black);
	panicBtn->setColour(TextButton::textColourOnId, Colours::white);
	panicBtn->setColour(TextButton::buttonColourId, Colours::red);
	panicBtn->setColour(TextButton::buttonOnColourId, Colours::orange);
	panicBtn->setBounds(buttonArea.removeFromTop(h));
	buttonArea.removeFromTop(h/2);

	// add export button
	auto * exportBtn = addToList(new TextButton("Export"));
	exportBtn->setComponentID((String)EXPORT_BUTTON);
	exportBtn->addListener(bListen);
	exportBtn->setColour(TextButton::textColourOffId, Colours::white);
	exportBtn->setColour(TextButton::textColourOnId, Colours::black);
	exportBtn->setColour(TextButton::buttonColourId, getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	exportBtn->setColour(TextButton::buttonOnColourId, Colours::orange);
	exportBtn->setBounds(buttonArea.removeFromTop(h));

	playArea.removeFromBottom(buttonArea.getHeight());
	playArea.reduce(buttonArea.getHeight() / 2, 0.0f);

	// draw path for play button
	DrawablePath Normal, Over, Clicked;
	Path play;
	play.addEllipse(0.0f, 0.0f, playArea.getWidth(), playArea.getHeight());
	Normal.setPath(play);
	Normal.setFill(Colours::orange);
	Normal.setStrokeFill(Colours::black);
	Normal.setStrokeThickness(.5f);

	Over.setPath(play);
	Over.setFill(Colours::darkorange);
	Over.setStrokeFill(Colours::black);
	Over.setStrokeThickness(.50f);

	Clicked.setPath(play);
	Clicked.setFill(Colours::red);
	Clicked.setStrokeFill(Colours::black);
	Clicked.setStrokeThickness(.50f);

	// add play button
	auto * playBtn = addToList(new DrawableButton("Play", DrawableButton::ImageRaw));
	playBtn->setComponentID((String)PLAY_BUTTON);
	playBtn->addListener(bListen);
	playBtn->setImages(&Normal, &Over, &Clicked);
	playBtn->setBounds(playArea);
	playBtn->setClickingTogglesState(true);

	// add volume slider component
	auto * vBox = addToList(new volumeBox(valueTreeState));
	vBox->setVolumeListener(sListen);
	vBox->setBounds(area.removeFromTop(h));
}

// Passes reference for button and slider listeners
void GuiFunc::setListeners(Slider::Listener* _sliderListener, Button::Listener* _buttonListener)
{
	sListen = _sliderListener;
	bListen = _buttonListener;
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
}

fundFreq::~fundFreq()
{
}

void fundFreq::paint(Graphics & g)
{
	//  sets background color
	Colour bgColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
	g.fillAll(bgColour);   // clear the background
	
	// draws outline of the component
	g.setColour(Colours::orange);
	g.drawRect(getLocalBounds(), 1);
	Point<float> p1(getWidth()*7.0f/12.0f, 0.0f);
	Point<float> p2(getWidth()*7.0f/12.0f,getBottom());
	Line<float> l(p1, p2);
	Point<float> ph1(getWidth()*7.0f / 12.0f, getHeight() / 2.0f);
	Point<float> ph2(getWidth(), getHeight() / 2.0f);
	Line<float> l1(ph1, ph2);
	g.drawLine(l, 1.0f);
	g.drawLine(l1, 1.0f);

}

void fundFreq::resized()
{
	// sets component list to empty
	emptyList();

	// text box to display current note in fundamental frequency
	txtBox = new Label("fundFreq", "A");
	auto* tb = addToList(txtBox);
	updateText();
	tb->setBounds(0.0f,0.0f,getWidth() * 7/12,getHeight());
	tb->setJustificationType(Justification::centred);

	// draw paths for up and down buttons for fundemantal frequency
	DrawablePath upNormal, downNormal, upOver, downOver, upClicked, downClicked;
	Path upPath, downPath;
	auto downArea = Rectangle<int>(getWidth() * 7.0f / 12.0f, getHeight()/2.0f, getWidth() * 5/12, getHeight()/2);
	auto upArea = Rectangle<int>(getWidth() * 7.0f / 12.0f, 0.0f, getWidth() * 5/12, getHeight() / 2);
	float xMargin = upArea.getWidth() * .2f;
	float yMargin = upArea.getHeight() *.2f;
	upPath.addTriangle(xMargin, upArea.getHeight() - yMargin, upArea.getWidth()/2, yMargin, upArea.getWidth() - xMargin, upArea.getHeight() - yMargin);
	downPath.addTriangle(xMargin,yMargin,downArea.getWidth() - xMargin, yMargin, downArea.getWidth()/2, downArea.getHeight() - yMargin);


	upNormal.setPath(upPath);
	upNormal.setFill(Colours::orange);
	upNormal.setStrokeFill(Colours::black);
	upNormal.setStrokeThickness(.5f);

	upOver.setPath(upPath);
	upOver.setFill(Colours::darkorange);
	upOver.setStrokeFill(Colours::black);
	upOver.setStrokeThickness(.50f);

	upClicked.setPath(upPath);
	upClicked.setFill(Colours::red);
	upClicked.setStrokeFill(Colours::black);
	upClicked.setStrokeThickness(.50f);

	downNormal.setPath(downPath);
	downNormal.setFill(Colours::orange);
	downNormal.setStrokeFill(Colours::black);
	downNormal.setStrokeThickness(.50f);

	downOver.setPath(downPath);
	downOver.setFill(Colours::red);
	downOver.setStrokeFill(Colours::black);
	downOver.setStrokeThickness(.50f);

	downClicked.setPath(downPath);
	downClicked.setFill(Colours::darkorange);
	downClicked.setStrokeFill(Colours::black);
	downClicked.setStrokeThickness(.50f);

	// set bounds and location for up button
	DrawableButton* upBtn = new DrawableButton("up", DrawableButton::ImageRaw);
	auto * ubtn = addToList(upBtn);
	ubtn->setComponentID((String)FUND_FREQ_BUTTON);
	ubtn->addListener(bListener);
	ubtn->setImages(&upNormal, &upOver, &upClicked);
	ubtn->setBounds(upArea);
	ubtn->setClickingTogglesState(false);
	ubtn->onClick = [this] {
		num = (num++) % 12;
		updateText();
	};

	// set bounds and location for down button
	DrawableButton* downBtn = new DrawableButton("down", DrawableButton::ImageRaw);
	auto * dbtn = addToList(downBtn);
	dbtn->setComponentID((String)FUND_FREQ_BUTTON);
	dbtn->addListener(bListener);
	dbtn->setImages(&downNormal, &downOver, &downClicked);
	dbtn->setBounds(downArea);
	dbtn->setClickingTogglesState(false);
	dbtn->onClick = [this] {
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
}


//==============================================================================
// End fundFreq functions
//==============================================================================
