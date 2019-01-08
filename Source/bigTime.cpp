/*
  ==============================================================================

	bigTime.cpp
	Created: 20 Dec 2018 1:00:48pm
	Author:  danreiter

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "bigTime.h"

//==============================================================================
bigTime::bigTime()
{
	
	xPoint = new int;
	xProfile = new int;
	time = new int;
	*xProfile = -1;
	*xPoint = 0;
	*time = 10;
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.

	timeBase.setSize(1000, 1000);
	view.setViewedComponent(&timeBase, false);
	view.setScrollBarsShown(false, false);
	addAndMakeVisible(view);

}

bigTime::~bigTime()
{
}

void bigTime::paint(Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	//g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
	int btnWidth = (int)(getWidth() / (*time));
	Rectangle<float> backGround(0.0f, 0.0f, getWidth() - (getWidth() - ((*time) * btnWidth)), getHeight());
	//g.fillAll(Colours::white);
	g.setColour(Colours::white);
	g.fillRect(backGround);
	g.setColour (Colours::black);
	g.drawRect (backGround, 1);   // draw an outline around the component

	//g.setColour (Colours::white);
	g.setFont (14.0f);
	g.drawText("bigTime", getLocalBounds(),
	            Justification::centred, true);   // draw some placeholder text

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

	



	g.setColour(Colours::lightgreen);
	Rectangle<float> selected(*xPoint * btnWidth, 0.0f, getWidth()/(*time), getHeight() * .80f );
	g.fillRect(selected);

	int profileWidth = btnWidth / 5;
	if (*xProfile >= 0)
	{
		Rectangle<float> profileMarkArea(((int)(getWidth() / (*time)) / 5) * (*xProfile), 0.0f, ((int)(getWidth() / (*time)) / 5), getHeight()* .8f);
		g.setColour(Colours::red);
		g.fillRect(profileMarkArea);
	}
}

void bigTime::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

	int btnWidth = (int)(getWidth() / *time);
	

	Rectangle<float> rec(btnWidth, getHeight() * .20f);
	view.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
	timeBase.setSize(getWidth() - (getWidth()- (btnWidth* (*time))), getHeight() - rec.getHeight());
	emptyList();
	for (int i = 0; i < *time; ++i)
	{
		Path p;
		DrawablePath normal, over, selected;

		p.addRectangle(.0f, .0f, getWidth(), getHeight());

		normal.setPath(p);
		normal.setFill(Colours::orange);
		normal.setStrokeFill(Colours::black);
		normal.setStrokeThickness(4.0f);

		over.setPath(p);
		over.setFill(Colours::darkorange);
		over.setStrokeFill(Colours::black);
		over.setStrokeThickness(4.0f);

		selected.setPath(p);
		selected.setFill(Colours::red);
		selected.setStrokeFill(Colours::black);
		selected.setStrokeThickness(4.0f);

		auto* tb = addToList(new DrawableButton("Button " + String(i + 1), DrawableButton::ImageRaw));
		//tb->setRadioGroupId(34567);
		tb->setComponentID(String(i));
		tb->setImages(&normal, &over, &selected);
		tb->setClickingTogglesState(true);
		tb->onClick = [this]
		{
			auto * focused = Component::getCurrentlyFocusedComponent();
			*xPoint = focused->getComponentID().getIntValue();
			repaint();
		};
		tb->setBounds(btnWidth * i, getHeight() - (getHeight() * .20f), btnWidth, rec.getHeight());
		if (i == 0)
			tb->setToggleState(true, dontSendNotification);
	}

}



void bigTime::setProfile(int * _Xpoint, int * _profile, int * _time)
{
	xProfile = _profile;
	xPoint = _Xpoint;
	time = _time;
}
