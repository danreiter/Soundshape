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
#include "PluginProcessor.h"

//==============================================================================
/*
	This component lives inside our window, and this is where you should put all
	your controls and content.
*/
class MainComponent : public Component, public Slider::Listener, public Button::Listener
{
public:
	//==============================================================================
	MainComponent(Soundshape_pluginAudioProcessor& p);
	~MainComponent();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;
	void sliderValueChanged(Slider* slider) override;
	void buttonClicked(Button* button) override;
	void setConverter(Converter* _converter);

private:
	//==============================================================================
	// Your private member variables go here...
	FeqWindow fWindow;  // Window displays the selected frequency profile element
	smallTime sTWindow; // Window displays a zoomed in view of the time domain 
	bigTime bTWindow;	// Window displays the time domain
	fundFreq fund;		// Component handles seeting for the fundmental frequency for redering the time domain
	GuiFunc volComp;	// Component handles volume settings
	envelope enve;		// Component handles Envelope settings
    TooltipWindow ttp;  // Component handles tooltip messages
	AudioThumbnail * thumbnail //reference to audio rendered in time domain


	TextButton *harmonicButton;			// Toggles harmonic constraint for frequency profile
	TextButton *addButton;				// Toggles buttons to add frequency spikes to frequency profile
	Slider *zoomSlider;					// Handles magnification settings for frequency profile window
	ComboBox cb;						// Toggles files for quick access 
	float amp;							// Currently not used
	TextButton *writeButton;			// Currently not used
	int freq, harm, add, timeSize;		// Varibles for tracking software and gui state
	int timeBlock, selectedProfile;		// Varible for tracking current selected time and frequency
	double zoom;						// Varible for tracking current degree of zoom of frquency profile
	float profile[4000];				// Varible for testing frequency profile
	float soundWaveProfile[50][2048];   // Varible for testing/store the soundWave values
	// Varible for notes in hz
    static float notes[12];
    float currentNoteFreq = 27.5;

    Converter *converterPtr;
    Soundshape_pluginAudioProcessor& processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
