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
bigTime::bigTime(AudioProcessorValueTreeState& _valueStateTree)
{
	
	xPoint = new int;

	time = new int;

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
    playTime->setSliderStyle(Slider::TwoValueHorizontal);
	playTime->setRange(0, 50, 1);
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
	}

    // hook up as a listener for changes to the play slider
    _valueStateTree.addParameterListener("beginningChunk", this);
    _valueStateTree.addParameterListener("endingChunk", this);

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

	int btnWidth = (int)(getWidth() / (*time));

	Rectangle<float> BackGround(0, 0, 10 * btnWidth, getHeight());
	g.setColour(Colours::burlywood);
	g.fillRect(BackGround);

	// Fills background color of selected time domain
	// this colour selects the zoom time domain
	g.setColour(Colours::lightgreen);
	Rectangle<float> selected(*xPoint * btnWidth, 0.0f, getWidth()/(*time), getHeight() * .80f );
	g.fillRect(selected);
	

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
void bigTime::setProfile(int * _Xpoint, int * _profile, int * _time, Button::Listener * _parent, Slider::Listener* _sliderParent, Converter* _cp)
{
	xProfile = _profile;
	xPoint = _Xpoint;
	time = _time;
	parent = _parent;	
	sliderParent = _sliderParent;

	playTime->addListener(sliderParent);
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->addListener(parent);
	}
	timeBase.setConverter(_cp);
	timeBase.setCurrentProfile(xProfile);
	timeBase.repaint();
}
//==============================================================================



void bigTime::parameterChanged(const String & parameterID, float newValue)
{
    // needs to repsond if the backend changes the position of the double-sided play range selector
    // (this can happen if the range is changed by the DAW or when a preset is loaded)
    if (parameterID == "beginningChunk") {
        playTime->setMinValue((int)newValue);
    }
    if (parameterID == "endingChunk") {
        playTime->setMaxValue((int)newValue);
    }
}
