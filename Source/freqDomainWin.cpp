/*
  ==============================================================================

    freqDomainWin.cpp
    Created: 28 Dec 2018 11:48:50am
    Author:  Daniel Reiter
	Component: Render current frequency seleced frequency profile and allow users to 
			   add and modify frequency spikes using sliders and buttons
  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "freqDomainWin.h"

//==============================================================================
//  freqDomainWin constructor set default values to variables to allow a GUI to open
//==============================================================================
freqDomainWin::freqDomainWin()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	setComponentID((String)FREQ_DOMAIN);
	parent = NULL;
	buttonParent = NULL;
	profile = new float[4000];
	for (int i = 0; i < 4000; i++)
	{
		profile[i] = -1.0f;

	}
	setProfileControl(&profile[0], 4000);

	first = -1;           
	int temp = -1;		  
	add = &temp;
	harm = &temp;

}


freqDomainWin::~freqDomainWin()
{
}
//==============================================================================

//==============================================================================
//  Paint Funciton
//==============================================================================
void freqDomainWin::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	float pixel = getWidth() * .01f;
	int n = getWidth() * 10;
	float xMark = 0.0f;
	int colourMod = 0;
	bool flag = true;
	Colour c1;
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	// draw color of background
	for (int i = 0; i <= n; i++)
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

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
	g.setColour(Colours::black);
	g.setFont(14.0f);
	g.drawText("Base Window", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text
	float margin = getHeight() *.10f;
	Line<float> vLine(margin, margin, margin, getHeight() - margin);
	Line<float> hLine(margin, getHeight() - margin, getWidth() - margin, getHeight() - margin);
	g.drawLine(vLine);
	g.drawLine(hLine);
	float vTick = vLine.getLength() / 10;
	float hTick = hLine.getLength() / 100;
	float smallTick = hLine.getLength() / 4000;
	Rectangle<float> vArea(.5f * margin, margin, margin, vLine.getLength());
	Rectangle<float> hArea(margin, getHeight() - (1.5f * margin), hLine.getLength(), margin);
	Rectangle<float> smallArea(margin, getHeight() - (1.25f *margin), hLine.getLength(), margin / 2);
	Rectangle<float> testPrint(margin, 1.25f *margin, hLine.getLength(), margin / 2);
	g.setFont(vTick * .8f);
	Rectangle<float> hTickArea(hArea);

	// draw horizontal scale
	for (int i = 0; i < 4000; i++)
	{

		String temp = std::to_string(i);
		smallArea.removeFromLeft(smallTick);
		hTickArea.removeFromLeft(smallTick);
		if (i < 100 && i % 10 == 0)
		{
			g.drawText(temp, vArea, Justification::bottomLeft, true);
			vArea.removeFromBottom(vTick);
			g.drawLine(vArea.getRight(), vArea.getBottom(), vArea.getWidth(), vArea.getBottom());
		}
		if (i % 10 == 0)
		{
			g.drawText(temp, hTickArea, Justification::bottomLeft, true);
			Line<float> tLine(hTickArea.getBottomLeft(), hTickArea.getTopLeft());
			g.drawLine(tLine);
		}

		Line<float> tLine(smallArea.getBottomLeft(), smallArea.getTopLeft());
		g.drawLine(tLine);
		testPrint.removeFromLeft(smallTick);
		int t1 = (int)profile[i];
		String temp2 = std::to_string(t1);
	}

	margin = getHeight() *.10f;
	Rectangle<float> btnArea(margin, getHeight() - (1.25f *margin), getWidth() - (2 * margin), margin / 4);
	float tick = btnArea.getWidth() / 1024;

	// loop sets location, bounds, and visiblity of sliders and buttons
	for (int i = 0; i < 4000; i++)
	{

		Path btnPath;
		DrawablePath normal, down, over;
		btnArea.removeFromLeft(tick);

		if (profile[i] < 0)
		{
			// set laction and bounds for each slider and button
			components[i]->setBounds(btnArea.getX() - (margin / 2), btnArea.getY() - (margin / 4), margin, margin);
			sliders[i]->setBounds(btnArea.getX() - (margin / 8), margin, margin / 2, getHeight() - (2 * margin));
			
			// If add button is on - set add buttons visilbe to true
			if (*add > 0)
			{
				// harmonic correctness case 
				if (harm && first > 0 && (i % first == 0))
				{

				}
				else if (harm && first > 0)
				{

				}
				// no harmonic correctness case
				else
				{

					components[i]->setVisible(true);

				}
			}
			// hide add buttons
			else
			{

				components[i]->setVisible(false);
			}
		}
		// show slider that are being used
		else
		{
			sliders[i]->setVisible(true);
		}


	}
}
//==============================================================================

//==============================================================================
//  Resize Funciton
//==============================================================================
void freqDomainWin::resized()
{

}
//==============================================================================

//==============================================================================
//  setBase passes need references to freqDomainWin
//==============================================================================
void freqDomainWin::setBase(int * _harm, int * _add, Slider::Listener* _parent, Button::Listener* _bParent, float* _profile, int _size)
{
	harm = _harm;                // flag for harmonic correctness is on/off
	add = _add;                  // flag for add buttons visiblity on/off
	parent = _parent;            // slider listener 
	profile = new float[_size];  // allocate for new sound profile space
	profile = _profile;          // set frequency profile values
	buttonParent = _bParent;	 // button listener

	// set button and slider listeners to parent
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->addListener(buttonParent);
		sliders[i]->addListener(parent);
	}

}
//==============================================================================

//==============================================================================
//  setProfileControl Funciton sets values of a frequency profile to a list of 
//  sliders, declares and instaniates a list of sliders and a list of buttons
//==============================================================================
void freqDomainWin::setProfileControl(float * _profile, int _size)
{
	// sets list to empty
	emptyList();

	// declares and instaniates a list of sliders and a list of buttons
	for (int i = 0; i < _size; i++)
	{
		auto * tb = addToList(new TextButton(""));
		tb->setComponentID(String(i));
		tb->setClickingTogglesState(true);
		tb->setColour(TextButton::textColourOnId, Colours::black);
		tb->setColour(TextButton::buttonColourId, Colours::white);
		tb->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
		tb->setColour(TextButton::textColourOffId, Colours::black);
		tb->setVisible(false);
		if (buttonParent != NULL)
		{
			tb->addListener(buttonParent);
		}


		auto * sb = createSlider();
		sb->setRange(0.0, 100.0, 0.1);
		sb->setSliderStyle(Slider::LinearBarVertical);
		sb->setComponentID(String(i));
		sb->setValue((double)(_profile[i]), sendNotificationAsync);
		sb->setColour(Slider::trackColourId, Colours::red);
		sb->setTextBoxIsEditable(false);
		sb->setPopupDisplayEnabled(true, true, this);
		sb->setVisible(false);
		if (parent != NULL)
		{
			sb->addListener(parent);
		}
	}

}
//==============================================================================
