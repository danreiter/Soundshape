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

#define VOLUME_SLIDER 2001
#define PLAY_BUTTON 2002
#define EXPORT_BUTTON 2003
#define PANIC_BUTTON 2004
#define FUND_FREQ_BUTTON 2005

//==============================================================================
//   volumeBox is a slider component drawn in a box with volume symbols
//==============================================================================
class volumeBox : public Component
{
public:
	volumeBox();
	~volumeBox();

	void paint(Graphics&) override;
	
	void resized() override;
	void setVolumeListener(Slider::Listener * _listener);
private:
	Slider * volume;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(volumeBox)
};
//==============================================================================


//==============================================================================
//  GuiFunc contains volume component and expor, play, and panic buttons
//==============================================================================
class GuiFunc : public Component
{
public:
	GuiFunc();
	~GuiFunc();

	void paint(Graphics&) override;
	void resized() override;
	void setListeners(Slider::Listener* _sliderListener, Button::Listener* _buttonListener);


private:
	Slider::Listener * sListen;							// reference to parent for slider listener
	Button::Listener * bListen;							// refernence to parent for button listener
	
	//List of components
	OwnedArray<Component> components;					
	template <typename ComponentType>
	ComponentType* addToList(ComponentType * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}

	// emptys list
	void emptyList()
	{
		components.clear(true);
	}

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
	void setListener(Button::Listener* _listener);

private:
	Label *txtBox;							// displays fundmental frequency
	int num;								// tracks the current fundmental frequency
	Button::Listener * bListener;			// reference to parent as button listener

	// list of components
	OwnedArray<Component> components;
	template <typename ComponentType>
	ComponentType* addToList(ComponentType * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}

	// clears component list
	void emptyList()
	{
		components.clear(true);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(fundFreq)
};
//==============================================================================
//end fundFreq Functions
//==============================================================================
