/*
  ==============================================================================

	smallTime.cpp
	Created: 20 Dec 2018 1:00:26pm
	Author:  danre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "smallTime.h"
//#include "DemoUtilities.h"

//==============================================================================
smallTime::smallTime()
{

	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	xStart = new int;
	xProfile = new int;
	time = new int;
	*time = 10;
	*xStart = 0;
	*xProfile = -1;

	view.setViewedComponent(&tdTest, false);
	view.setScrollBarsShown(false,false);
	
	addAndMakeVisible(view);
}

smallTime::~smallTime()
{
}

void smallTime::paint(Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/
	int btnWidth = (int)(getWidth() / 5);
	int width = getWidth() - (getWidth() - (5 * btnWidth));
	Rectangle<float> backGround(0.0f, 0.0f, width, getHeight() * .8f);
	g.setColour(Colours::white);
	//g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
	g.fillRect(backGround);

	g.setColour (Colours::black);
	g.drawRect (backGround, 1);   // draw an outline around the component

	//g.setColour (Colours::white);
	g.setFont (14.0f);
	g.drawText ("smallTime", getLocalBounds(),
	            Justification::centred, true);   // draw some placeholder text

	view.setViewPosition(*xStart*(tdTest.getWidth() / (*time)), 0);

	float pixel = (getWidth() - (getWidth() - (5 * btnWidth))) * .01f;
	int n = (getWidth() - (getWidth() - (5 * btnWidth))) * 10;
	float xMark = 0.0f;
	int colourMod = 0;
	bool flag = true;
	Colour c1;

	while (xMark + pixel  <= (n / 10))
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


	int profileMark = *xProfile - (*xStart*5);
	if (profileMark >= 0 && profileMark < 5)
	{
		Rectangle<float> profileArea(profileMark * btnWidth, 0.0f, btnWidth, getHeight() * .8f);
		g.setColour(Colours::red);
		g.fillRect(profileArea);
	}
}

void smallTime::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	emptyList();
	int btnWidth = (int)(getWidth() / 5);
	int width = btnWidth * 5;
	//view.setBounds(0.0f, 0.0f, width, getHeight() * .8f);
	tdTest.setSize(width*(*time), getHeight() - (getHeight() * .20f));

	view.setBoundsRelative( 0.0f, 0.0f, 1.0f, 1.0f);
	view.setViewPosition(*xStart*(tdTest.getWidth() / (*time)), 0);
	view.setBounds(0.0f, 0.0f, width, getHeight() * .8f);


	for (int i = 0; i < 5; ++i)
	{

		auto* tb = addToList(new TextButton("Button " + String(i + 1)));
		tb->setRadioGroupId(34567);
		tb->setClickingTogglesState(false);
		tb->setComponentID(String(i));
		tb->setColour(TextButton::textColourOffId, Colours::black);
		tb->setColour(TextButton::textColourOnId, Colours::black);
		tb->setColour(TextButton::buttonColourId, Colours::orange);
		tb->setColour(TextButton::buttonOnColourId, Colours::red);
		tb->onClick = [this]
		{
			auto * focused = Component::getCurrentlyFocusedComponent();
			*xProfile = focused->getComponentID().getIntValue() + (*xStart * 5);
			repaint();
		};
		tb->setBounds(btnWidth * i, getHeight() - (getHeight() * .20f), btnWidth, getHeight() * .20f);
		tb->setConnectedEdges(((i != -1) ? Button::ConnectedOnLeft : 0)
			| ((i != -1) ? Button::ConnectedOnRight : 0));
	}


}

void smallTime::setTimeDomain(int * _start, int * _profile, int* _time)
{

	xStart = _start;
	xProfile = _profile;
	time = _time;

}



