/*
  ==============================================================================

	smallTime.h
	Created: 20 Dec 2018 1:00:26pm
	Author: Daniel Reiter
	Description: Window has a time domian object that it show in a viewport at one second intervals. The component has five 
					 buttons used to select the current frequency profile.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "timeDomainWin.h"

#define PROFILE_SELECT_BUTTON 5001

//==============================================================================
/*
*/
class smallTime : public Component
{
public:
	smallTime();
	~smallTime();

	void paint(Graphics&) override;
	void resized() override;

	// Function passes references from parent
	void setTimeDomain(int * _start, int * _profile, int * _time, Button::Listener* _parent, Converter * _cp);


private:
	int * xStart;			  //   Start time of the second to shown in the viewport 
	int * xProfile;			  //   Currently selected frequency profile
	timeDomainWin tdTest;     //   Time Domain 
	Viewport view;			  //   Viewport
	int * time;				  //   Number subsections of a second is split into for frequency profiles
	Button::Listener* parent; //   Refences of the parent to listen to buttons

	// List of components
	OwnedArray<TextButton> components;
	TextButton* addToList(TextButton * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}
	// Empty component list
	void emptyList()
	{
		components.clear(true);
	}


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(smallTime)
};
