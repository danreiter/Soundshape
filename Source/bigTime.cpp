/*
  ==============================================================================

	bigTime.cpp
	Created: 20 Dec 2018 1:00:48pm
	Description: Component uses a view port to display the time domain adn contains buttons to allow the 
					user to select a magnified view in the small time window.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "bigTime.h"



//==============================================================================
//  Constructor
//==============================================================================
bigTime::bigTime()
{
	
	xPoint = new int;
	//xProfile = new int;
	time = new int;
	//*xProfile = -1;
	*xPoint = 0;
	*time = 10;
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	//  Set viewport settings
	timeBase.setSize(1000, 1000);
	view.setViewedComponent(&timeBase, false);
	view.setScrollBarsShown(false, false);
	addAndMakeVisible(view);

	//  Play time slider 
    playTime = new Slider();
    addAndMakeVisible(playTime);
	playTime->setComponentID((String)PLAYTIME_SLIDER);
    playTime->setRange(0, 600, 12);
    playTime->setSliderStyle(Slider::TwoValueHorizontal);
	playTime->setRange(0, 50, 5);
	playTime->setMinAndMaxValues(0, 50);
    playTime->setColour(Slider::trackColourId, Colours::red);
    playTime->setColour(Slider::thumbColourId, Colours::red);
    playTime->setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

	// Create buttons
	for (int i = 0; i < *time; ++i)
	{
		auto* tb = addToList(new TextButton("Sec " + String(i + 1)));
		tb->setRadioGroupId(TIME_SELECT_BUTTON);
		tb->setClickingTogglesState(false);
		tb->setComponentID(String(i));
		tb->setColour(TextButton::textColourOffId, Colours::black);
		tb->setColour(TextButton::textColourOnId, Colours::black);
		tb->setColour(TextButton::buttonColourId, Colours::orange);
		tb->setColour(TextButton::buttonOnColourId, Colours::red);
		//tb->onStateChange = [this]
		//{
		//	auto * focused = Component::getCurrentlyFocusedComponent();
		//	*xPoint = focused->getComponentID().getIntValue();
		//};
	}
}

bigTime::~bigTime()
{
}
//==============================================================================

//==============================================================================
//   Function paint
//==============================================================================
void bigTime::paint(Graphics& g)
{
	//g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
	int btnWidth = (int)(getWidth() / (*time));
	Rectangle<float> backGround(0.0f, 0.0f, getWidth() - (getWidth() - ((*time) * btnWidth)), getHeight());
	g.setColour(Colours::white);
	g.fillRect(backGround);
	g.setColour (Colours::black);
	g.drawRect (backGround, 1);   // draw an outline around the component
	g.setFont (14.0f);
	g.drawText("bigTime", getLocalBounds(),
	            Justification::centred, true);   // draw some placeholder text


	// paints background
	float pixel = (getWidth() - (getWidth() - ((int)(getWidth() / (*time))*(*time)))) * .01f;
	int n = (getWidth() - (getWidth()-((int)(getWidth()/(*time))*(*time)))) * 10;
	float xMark = 0.0f;
	int colourMod = 0;
	bool flag = true;
	Colour c1;
	while(xMark + pixel  <= (n/10))
	{
		Rectangle<float> rec5(xMark, 0.0f, pixel + (pixel * .1f), getHeight());
		xMark += pixel;
		if (flag)
		{
				c1 = Colour(255, (170 + colourMod), 0);
		}
		else
		{
			c1 = Colour(255, (200 - colourMod), 0);
		}
		g.setColour(c1);
		g.fillRect(rec5);

		colourMod = (++colourMod % 31);
		if (colourMod == 0)
		{
			flag = !flag;
		}

	}

	


	// Fills background color of selected time domain
	g.setColour(Colours::lightgreen);
	Rectangle<float> selected(*xPoint * btnWidth, 0.0f, getWidth()/(*time), getHeight() * .80f );
	g.fillRect(selected);

	// Fills background color of selected frequnecy domain
	int profileWidth = btnWidth / 5;
	if (*xProfile >= 0)
	{
		Rectangle<float> profileMarkArea(((int)(getWidth() / (*time)) / 5) * (*xProfile), 0.0f, ((int)(getWidth() / (*time)) / 5), getHeight()* .8f);
		g.setColour(Colours::red);
		g.fillRect(profileMarkArea);
	}

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->setBounds(btnWidth * i, getHeight() - (getHeight() * .20f), btnWidth, getHeight() * .20f);
		components[i]->setConnectedEdges(((i != -1) ? Button::ConnectedOnLeft : 0)
			| ((i != -1) ? Button::ConnectedOnRight : 0));
	}

	// Set bounds and location for the play time slider
    g.setColour(Colours::black);
    playTime->setBounds(timeBase.getX() - ((getHeight() * .15f) / 2.0f), (timeBase.getHeight() / 2.0f) - ((getHeight() * .15f) / 2.0f), getWidth() + ((getHeight() * .15f) / 2.0f), getHeight() * .15f);

}
//==============================================================================

//==============================================================================
//	Function repaint
//==============================================================================
void bigTime::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

	// Set viewport bounds and locations
	int btnWidth = (int)(getWidth() / *time);
	Rectangle<float> rec(btnWidth, getHeight() * .20f);
	view.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
	timeBase.setSize(getWidth() - (getWidth()- (btnWidth* (*time))), getHeight() - rec.getHeight());

}
//==============================================================================


//==============================================================================
//  Function passes references from the parent to varibles in bigTime
//==============================================================================
void bigTime::setProfile(int * _Xpoint, int * _profile, int * _time, Button::Listener * _parent, Converter* _cp)
{
	xProfile = _profile;
	xPoint = _Xpoint;
	time = _time;
	parent = _parent;		
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->addListener(parent);
	}
	timeBase.setConverter(_cp);
	timeBase.repaint();
}
//==============================================================================
