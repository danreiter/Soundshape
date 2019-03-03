/*
  ==============================================================================

    GuiFunc.h
    Created: 24 Dec 2018 12:44:39pm
    Author:  Daniel Reiter
	Description:Custome Components that are used to control some of Soundshape's functions.

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
//   volumeBox is a slider component drawn in a box with volume simbols
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
//  
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
	Slider::Listener * sListen;
	Button::Listener * bListen;
	OwnedArray<Component> components;
	template <typename ComponentType>
	ComponentType* addToList(ComponentType * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}
	void emptyList()
	{
		components.clear(true);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiFunc)
};
//==============================================================================

//==============================================================================
//
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
	Label *txtBox;
	int num;
	Button::Listener * bListener;
	OwnedArray<Component> components;
	template <typename ComponentType>
	ComponentType* addToList(ComponentType * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}
	void emptyList()
	{
		components.clear(true);
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(fundFreq)
};
//==============================================================================
//end fundFreq Functions
//==============================================================================
