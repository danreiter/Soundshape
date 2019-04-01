/*
  ==============================================================================

    bigTime.h
    Created: 24 Dec 2018 12:43:15pm
    Author:  Daniel Reiter
	Description: Component uses a view port to display the time domain adn contains buttons to allow the 
					user to select a magnified view in the small time window.

*/
//==============================================================================

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "timeDomainWin.h"

//==============================================================================
class bigTime : public Component
{
public:
	bigTime();
	~bigTime();

	void paint(Graphics&) override;
	void resized() override;

	// Function to pass refernces from the parent
	void setProfile(int * _Xpoint,int * _profile, int * _time, Button::Listener * _parent, Converter* _cp);

private:
	timeDomainWin timeBase; // time domain                  
	Viewport view;			// viewport

	int * xPoint;			// Start of the currently selected time domain
	int * xProfile;			// Start of the currently selected frequency domain
	int *time;				// Size of the time domain
	Button::Listener* parent; // Referense to the parent
	Slider* playTime;		  // Slide to pick the section of time domian to to play

	// list of components
	OwnedArray<TextButton> components;
	TextButton* addToList(TextButton * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}

	// Empty list of compnents
	void emptyList()
	{
		components.clear(true);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(bigTime)
};
