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

#define TIME_SELECT_BUTTON 5002
#define PLAYTIME_SLIDER 5003

//==============================================================================
class bigTime : public Component,
                public AudioProcessorValueTreeState::Listener
{
public:
	bigTime(AudioProcessorValueTreeState& _valueStateTree);
	~bigTime();

	void paint(Graphics&) override;
	void resized() override;

	// Function to pass refernces from the parent
	void setProfile(int * _Xpoint, int * _profile, int * _time, Button::Listener * _parent, Slider::Listener * _sliderParent, Converter* _cp);

    /**
    Whenever the backend converter changes the beginning and ending of the the play range,
    it should call this callback so the GUI can update itself.
    */
    void parameterChanged(const String &parameterID, float newValue) override;

private:
	timeDomainWin timeBase; // time domain                  
	Viewport view;			// viewport

	int * xPoint;			// Start of the currently selected time domain
	int * xProfile;			// Start of the currently selected frequency domain
	int *time;				// Size of the time domain
	Button::Listener* parent; // Referense to the parent
	Slider::Listener* sliderParent;
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
