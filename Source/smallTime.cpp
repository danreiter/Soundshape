/*
  ==============================================================================

	smallTime.cpp
	Created: 20 Dec 2018 1:00:26pm
	Author: Daniel Reiter
	Description: Window has a time domian object that it show in a viewport at one second intervals. The component has five 
					 buttons used to select the current frequency profile.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "smallTime.h"

//==============================================================================
//	Constructor
//==============================================================================
smallTime::smallTime()
{

	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	xStart = new int;
	xProfile = new int;
	time = new int;
	currentProfile = new int;
	*time = 10;
	*xStart = 0;
	*xProfile = 0;
	*currentProfile = 0;

	// Viewport settings
	view.setViewedComponent(&tdTest, false);
	view.setScrollBarsShown(false,false);
	addAndMakeVisible(view);

	//  loop creates buttons 
	for (int i = 0; i < 5; ++i)
	{

		auto* tb = addToList(new TextButton("Sec " + String(i + 1) + "/5"));
		tb->setRadioGroupId(PROFILE_SELECT_BUTTON);
		tb->setClickingTogglesState(false);
		tb->setComponentID(String(i));
		tb->setColour(TextButton::textColourOffId, Colours::black);
		tb->setColour(TextButton::textColourOnId, Colours::black);
		tb->setColour(TextButton::buttonColourId, Colours::orange);
		tb->setColour(TextButton::buttonOnColourId, Colours::red);
		tb->setTriggeredOnMouseDown(true);
		//tb->onStateChange = [this]
		//{
			//auto * focused = Component::getCurrentlyFocusedComponent();
			//*xProfile = focused->getComponentID().getIntValue();// + ((int)(*xStart) * 5);
			//*currentProfile = *xProfile + ((int)(*xStart) * 5);
			//repaint();
		//};
	}
}

smallTime::~smallTime()
{
}
//==============================================================================

//==============================================================================
//	Sets viewport focus and draws background
//==============================================================================
void smallTime::paint(Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	int btnWidth = (int)(getWidth() / 5);

	//  Sets viewport focus on time domain
	view.setViewPosition(*xStart*(tdTest.getWidth() / (*time)), 0);

	//  Draws background color
	g.setColour(Colours::blanchedalmond);
	Rectangle<float> background(0, 0, btnWidth * 5, getHeight());
	g.fillRect(background);


	for (int i = 0; i < components.size(); i++)
	{
		components[i]->setBounds(btnWidth * i, getHeight() - (getHeight() * .20f), btnWidth, getHeight() * .20f);
		components[i]->setConnectedEdges(((i != -1) ? Button::ConnectedOnLeft : 0)
			| ((i != -1) ? Button::ConnectedOnRight : 0));
	}
}
//==============================================================================

//==============================================================================
//	Function on resize sets button and viewport bounds and locations
//==============================================================================
void smallTime::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..

	// Sets viewport position and location
	int width = (int)(getWidth() / 5) * 5;
	tdTest.setSize(width*(*time), getHeight() - (getHeight() * .20f));
	view.setBoundsRelative( 0.0f, 0.0f, 1.0f, 1.0f);
	view.setViewPosition(*xStart*(tdTest.getWidth() / (*time)), 0);
	view.setBounds(0.0f, 0.0f, width, getHeight() * .8f);
}
//==============================================================================

//==============================================================================
//  Function pass references from parent
//==============================================================================
void smallTime::setTimeDomain(int * _start, int * _profile, int * _currentProfile, int* _time, Button::Listener* _parent, Converter * _cp)
{

	xStart = _start;
	xProfile = _profile;
	time = _time;
	currentProfile = _currentProfile;
	parent = _parent;
	tdTest.setConverter(_cp);
	tdTest.setCurrentProfile(currentProfile);
	tdTest.repaint();
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->addListener(parent);
	}

}
//==============================================================================


