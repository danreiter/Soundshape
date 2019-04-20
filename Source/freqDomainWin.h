/*
  ==============================================================================

    freqDomainWin.h
    Created: 28 Dec 2018 11:48:50am
    Author:  Daniel Reiter
	Component: Render current frequency seleced frequency profile and allow users to 
			   add and modify frequency spikes using sliders and buttons

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"
#include "laf.h"

#define FREQ_DOMAIN 3000 ///< Component ID number for the freqDomainWin Component.

//==============================================================================
/**
Handles user interface for the frequency domain.

Component draws background for the frequency domain, frequency spike that are being used and the frequency spike add buttons.
The mainComponent listens to frequency spike sliders for changes to the in the selected frequency profile. The first harmonic is
always set to 440 hz and frquency spikes are displayed at even intervals.
*/
class freqDomainWin    : public Component
{
public:
    freqDomainWin();
    ~freqDomainWin();
	/**
	Sets frequency spike sliders and frequency spike add buttons locations. Draws vertical and horivontal scales for the frequency domain.
	Draws visible sliders and buttons.

	Function is called using the repaint() function.
	@see
	JUCE documentation: Component::paint(), Component::repaint()
	*/
    void paint (Graphics&) override;
	/**
	Repaints the component on resize.

	@see
	JUCE documentation: Component::resized(), Component::repaint()
	*/
    void resized() override;
	/**
	Gets reference varaibles from parent component.

	Is called on start up. After getting references to the current profile settings calls the setProfileControl()
	function to set the values for frequency spike add buttons and sliders. 
	See the function parameters and class variables.

	@see FeqWindow::setZoom()
	@see setProfileControl()
	@param _harm Reference whether the harmic correctness filter is on.
	@param _parent Reference to a slider listener to handle slider changes in mainComponent.
	@param _bParent Reference to a button listener to handle button changes in the mainComponent.
	@param _profile Reference to a Converter to get frequency values from the back-end.
	@param _size Reference to the number of frequency spikes per frequency profile.
	@param _chunk Reference to the index number of the currently selected frequency profile.
	@param _laf Reference to the current lookAndFeel theme.
	*/
	void setBase(int * _harm, int * _add, Slider::Listener* _parent,Button::Listener* _bParent, Converter* _profile, int _size, int * _chunk, CustomLookAndFeel * _laf);
	/**
	Declares and initaializes frequency spike add buttons and sliders and sets values on startup. 


	Check visibles acorrding to frquency spike values (values over 0 are set to visible)
	*/
	void setProfileControl();
	/**
	Sets visiblity and values for frequency spike add buttons and sliders on frequency profile change.


	Check visibles acorrding to frquency spike values (values over 0 are set to visible)
	*/
	void setProfile();
	/**
	Set the current lookAndFeel colors for this component.

	Needed to keep the frequency spike always red or blue with the use of different LookAndFeels

	@see CustomLookAndFeel::setColors()
	*/
	void resetColors();


private:
	int size;							///> The number of frequencies in Hz the is in a single profile.
	int * harm;                         ///> Flag for checking if harmonic correctness is on/off. 
	int *add;							///> Flag for checking if add button is on/off.
	int *chunk;							///> The index of the current frequency spike (sound chunk).
	Converter * profile;				///> Reference to the back-end converter used to get frequency spike values.
	Slider::Listener* parent;			///> Reference to parent as a slider event listener for mainComponent to update a sound and repaint on changes to frequency spikes.
	Button::Listener* buttonParent;     ///> Reference to parent as a button  listener for mainComponent to check for changes to the frequency spike add buttons.
	CustomLookAndFeel * laf;            ///> Reference to Soundshapes lookAndFeel theme.
	/**
	List of buttons to add frequency spikes.

	@see addToList()
	*/
	OwnedArray<TextButton> components;	
	/**
	Adds buttons to the components list of buttons which add frequency spikes.

	@see components
	*/
	TextButton* addToList(TextButton * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		newComp->onClick = [this] {
			auto * focused = Component::getCurrentlyFocusedComponent();
			float margin = this->getHeight() *.10f;
			focused->setVisible(false);
			sliders[(int)(focused->getComponentID().getIntValue()/2)]->setVisible(true);
		};
		return newComp;
	}
	/**
	List of sliders to store frequnecy spikes.

	@see createSlider()
	@see emptyList()
	*/
	OwnedArray<Slider> sliders;
	/**
	Adds sliders to the sliders list to represent of frequency spikes.

	@see sliders
	@see emptyList()
	*/
	Slider * createSlider()
	{
		Slider * s = new Slider();
		sliders.add(s);
		addAndMakeVisible(s);
	    return s;
	}
	/**
	Clears both sliders and components lists.

	@see sliders
	@see components
	*/
	void emptyList()
	{
		components.clear(true);
		sliders.clear(true);
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (freqDomainWin)
};
