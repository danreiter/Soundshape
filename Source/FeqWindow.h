/*
  ==============================================================================

	FreqWindow.h
	Created: 20 Dec 2018 12:59:50pm
	Author:  Daniel Reiter
	Description: Handles the viewport for the frequency domain

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "freqDomainWin.h"
#include "laf.h"

//==============================================================================
/**
Sets a viewport of the currently selected frequency domain profile. 
Passes references from parent component to the frequency domain component. Handles zooming in and out of thefrequency domain
view.
*/
class FeqWindow : public Component
{
public:
	FeqWindow();
	~FeqWindow();
	/**
	Renders viewport for the frequency doamin window according to the zoom settings.

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
	Gets reference varaibles from parent component, set the zoom variable reference and view lookAndFeel, and pass references of parent variables to the frequency domain
	base component.

	@see freqDomainWin::setBase()
	@param _zoom Reference to the level of magnification the viewport renders the frequency domain at.
	@param _harm Reference whether the harmic correctness filter is on.
	@param _parent Reference to a slider listener to handle slider changes in mainComponent.
	@param _bParent Reference to a button listener to handle button changes in the mainComponent.
	@param _profile Reference to a Converter to get frequency values from the back-end.
	@param _size Reference to the number of frequency spikes per frequency profile.
	@param _chunk Reference to the index number of the currently selected frequency profile.
	@param _laf Reference to the current lookAndFeel theme.
	*/
	void setZoom(double* _zoom, int* _harm, int * _add, Slider::Listener * _parent, Button::Listener* _bParent, Converter * _profile, int _size, int * _chunk, CustomLookAndFeel * _laf);
	/**
	Calls the frequency domain profile's setProfile() function to single the change of frequency profiles.

	@see freqDomainWin::setProfile()
	*/
	void setProfile();

private:
	freqDomainWin base;	   ///< Frequency domian for the GUI. 
	Viewport view;         ///< Viewport for frequency domain.
	double *zoom;          ///< Factor of horizontal zoom for the frequency domain window.

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeqWindow)
};
