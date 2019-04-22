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
#include "laf.h"

#define TIME_SELECT_BUTTON 5002 ///< Component ID number for the time profile select buttons Component.
#define PLAYTIME_SLIDER 5003	///< Component ID number for the slider to select the section of sound to play Component.

//==============================================================================
/**
Renders a window with a viewport of the entire time domain.

The buttom of the time domain view has ten buttons for selecting sections the time domain to zoom in to for the small time window.
Each button represents one second of the sound profile in the bigTime viewpot.

@param _valueTreeState Used to update the back-end of Soundshape on setting changes to the section of sound to play.
*/
class bigTime : public Component,
                public AudioProcessorValueTreeState::Listener
{
public:
	bigTime(AudioProcessorValueTreeState& _valueTreeState);
	~bigTime();

	void paint(Graphics&) override;
	void resized() override;
	/**
	Gets reference varaibles from parent component, set the variable reference for the currently selected
	time and frequency domain profiles, the amount of time each frequency profile repesents.
	Function also pass references of parent variables to the time domain object.


	@see timeDomainWin::setConverter()
	@see timeDomainWin::setCurrentProfile()
	@param _start Refenerence to the currently selected time profile to zoom into change in the small time domain window.
	@param _profile Reference to the frequency profile that is currently selected.
	@param _time The total number of seconds in a sound.
	@param _parent Reference to a button listener to handle button changes in the mainComponent.
	@param _sliderParent Reference to a slider listener to handle button changes in the mainComponent.
	@param _cp Reference to a Converter to get frequency values from the back-end.
	*/
	void setProfile(int * _Xpoint, int * _profile, int * _time, Button::Listener * _parent, Slider::Listener * _sliderParent, Converter* _cp);

    /**
    Whenever the backend converter changes the beginning and ending of the the play range,
    it should call this callback so the GUI can update itself.
    */
    void parameterChanged(const String &parameterID, float newValue) override;

private:
	timeDomainWin timeBase;		///< Time domain object used to render Soundshape's sound in the time domain.                 
	Viewport view;				///< Viewport to display the time domain in.

	int * xPoint;				///< Start of the currently selected time domain.
	int *time;					///< Size of the time domain.
	Button::Listener* parent;	///< Refences of the parent components to listen to buttons.
	Slider::Listener* sliderParent; ///< Refences of the parent components to listen to sliders.
	Slider* playTime;		  ///< Slide to pick the section of time domian to to play.
	AudioProcessorValueTreeState& valueTreeState; ///< event listen to update setting of Soundshape directly to the back-end. 
	/**
	List of buttons to change the currently selected time domain.

	@see addToList()
	*/
	OwnedArray<TextButton> components;
	/**
	Adds buttons to the components list of buttons which change the currently selected Ttime domain.

	@see components
	*/
	TextButton* addToList(TextButton * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}
	/**
	Clears the components lists.

	@see components
	*/
	void emptyList()
	{
		components.clear(true);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(bigTime)
};
