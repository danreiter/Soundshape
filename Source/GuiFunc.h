/*
  ==============================================================================

    GuiFunc.h
    Created: 24 Dec 2018 12:44:39pm
    Author:  Daniel Reiter
	Description:Custom Components that are used to control some of Soundshape's functions.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "laf.h"

#define VOLUME_SLIDER 2001
#define PLAY_BUTTON 2002
#define EXPORT_BUTTON 2003
#define IMPORT_BUTTON 2004
#define PANIC_BUTTON 2005
#define FUND_FREQ_SLIDER 2006
#define SUSTAIN_PLAY_BUTTON 2007

//==============================================================================
//   volumeBox is a slider component drawn in a box with volume symbols
//==============================================================================
class volumeBox : public Component
{
public:
	enum ColourIds
	{
		volColourIds = 80000
	};

	volumeBox(AudioProcessorValueTreeState& _valueTreeState);
	~volumeBox();

	void paint(Graphics&) override;
	
	void resized() override;
	void setVolumeListener(Slider::Listener * _listener);
private:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	Slider * volume;
    std::unique_ptr<SliderAttachment> gainAttachment; // lets the back and front end volume control each other
	//CustomLookAndFeel * claf; // will be moved to MainComponent
    

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(volumeBox)
};
//==============================================================================


//==============================================================================
//  GuiFunc contains volume component and expor, play, and panic buttons
//==============================================================================
class GuiFunc : public Component
{
public:
	GuiFunc(AudioProcessorValueTreeState& _valueTreeState);
	~GuiFunc();

	void paint(Graphics&) override;
	void resized() override;
	void setListeners(Slider::Listener* _sliderListener, Button::Listener* _buttonListener);
    bool getSustainedPlayBtnToggled();

private:
    AudioProcessorValueTreeState& valueTreeState;
	Slider::Listener * sListen;							// reference to parent for slider listener
	Button::Listener * bListen;							// refernence to parent for button listener
	Button *panicBtn, *exportBtn, *playBtn, *importBtn, *sustainPlyBtn; // buttons for import, export, and play functions
	volumeBox *volBox;									// Component to control the volume

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiFunc)
};
//==============================================================================

//==============================================================================
//Component contains controls and displays fundemental frequency
//==============================================================================
class fundFreq : public Component
{
public:
	fundFreq();
	~fundFreq();

	void paint(Graphics&) override;
	void resized() override;
	int getNote();
	void updateText();
	void setListener(Slider::Listener* _listener);

private:
	Label *txtBox;							// displays fundmental frequency
	int num;								// tracks the current fundmental frequency
	Slider::Listener * sListener;			// reference to parent as button listener
	DrawableButton * upBtn, * downBtn;		// Buttons to change the fundmental frequency
	Slider * fundFreqSlider;
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(fundFreq)
};
//==============================================================================
//end fundFreq Functions
//==============================================================================
