/*
  ==============================================================================

	MainComponent.h
	Created: 24 Dec 2018 12:44:39pm
	Author:  Daniel Reiter
	Description:Main Gui Component. Parent component for all Gui components. 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FeqWindow.h"
#include "smallTime.h"
#include "bigTime.h"
#include "GuiFunc.h"
#include "envelope.h"
#include "Converter.h"


//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent : public Component, public Slider::Listener, public Button::Listener
{
public:
	//==============================================================================
	MainComponent();
	~MainComponent();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void sliderValueChanged(Slider* slider) override;
	void buttonClicked(Button* button) override;
	void setConverter(Converter & _converter);
	void setProfile(int _index);

private:
	//==============================================================================
	// Your private member variables go here...
	FeqWindow fWindow;  // Window displays the selected frequency profile element
	smallTime sTWindow; // Window displays a zoomed in view of the time domain 
	bigTime bTWindow;	// Window displays the time domain
	fundFreq fund;		// Component handles seeting for the fundmental frequency for redering the time domain
	GuiFunc volComp;	// Component handles volume settings
	envelope enve;		// Component handles Envelope settings



	TextButton *harmonicButton;			// Toggles harmonic constraint for frequency profile
	TextButton *addButton;				// Toggles buttons to add frequency spikes to frequency profile
	Slider *zoomSlider;					// Handles magnification settings for frequency profile window
	ComboBox cb;						// Toggles files for quick access 
	float amp;							// Currently not used
	TextButton *writeButton;			// Currently not used
	int freq, harm, add, timeSize;		// Varibles for tracking software and gui state
	int timeBlock, selectedProfile;		// Varible for tracking current selected time and frequency
	double zoom;						// Varible for tracking current degree of zoom of frquency profile
	int currentProfile;                 // track currently selected profile chuck
	// Varible for notes in hz
	float notes[12] = { 27.5f,29.50f,30.87f, 16.35f, 17.32f, 18.35f, 19.45f, 20.60f, 21.83f, 23.12f, 24.5f, 25.96 };
	Converter * converter;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
