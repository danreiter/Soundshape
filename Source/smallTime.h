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
#include "laf.h"

#define PROFILE_SELECT_BUTTON 5001 ///< Component ID number for the frequency profile select buttons Component.

//==============================================================================
/**
Renders a window with a viewport of the time domain in a one second interval.

The buttom of the time domain view has five buttons for selecting frequency domain profiles.
Each button represents one fifth of a second of the sound profile in the smallTime viewpot. 
*/
class smallTime : public Component
{
public:
	smallTime();
	~smallTime();
	/**
	Sets locations and size of buttons to change frequency profiles. Set viewport to current time domain
	selected in the bigTime window.

	Function is called using the repaint() function.
	@see
	JUCE documentation: Component::paint(), Component::repaint()
	*/
	void paint(Graphics&) override;
	/**
	Repaints the component on resize.

	@see
	JUCE documentation: Component::resized(), Component::repaint()
	*/
	void resized() override;
	/**
	Gets reference varaibles from parent component, set the variable reference for the currently selected 
	time and frequency domain profiles, the amount of time each frequency profile repesents.
	Function also pass references of parent variables to the time domain object.


	@see timeDomainWin::setConverter()
	@see timeDomainWin::setCurrentProfile()
	@param _start Refenerence to the currently selected time profile to zoom into change in the big time domain window.  
	@param _currentProfile Reference to the frequency profile that is currently selected.
	@param _time Reference to the size to number sub-section of time that make out a each frequency profile.
	@param _parent Reference to a button listener to handle button changes in the mainComponent.
	@param _cp Reference to a Converter to get frequency values from the back-end.
	*/
	void setTimeDomain(int * _start, int * _currentProfile, int * _time, Button::Listener* _parent, Converter * _cp);


private:
	int * xStart;			  ///<  Start time of the second to shown in the viewport.
	int * currentProfile; 	  ///<  Currently selected frequency profile.
	int * time;				  ///<  Number subsections of a second is split into for frequency profiles current is set to 5.
	timeDomainWin tdTest;     ///<  Time Domain object.
	Viewport view;			  ///<  Viewport used to render the time domain view.
	Button::Listener* parent; ///<  Refences of the parent to listen to buttons.
	/**
	List of buttons to change the currently selected frequency profile.

	@see addToList()
	*/
	OwnedArray<TextButton> components;
	/**
	Adds buttons to the components list of buttons which change the currently selected frequency profile.

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


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(smallTime)
};
