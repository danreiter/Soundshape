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
private:
	Slider * volume;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(volumeBox)
};
//==============================================================================


//==============================================================================
/*
*/
class GuiFunc : public Component
{
public:
	GuiFunc();
	~GuiFunc();

	void paint(Graphics&) override;
	void resized() override;


private:
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
/*
*/
class fundFreq : public Component
{
public:
	fundFreq();
	~fundFreq();

	void paint(Graphics&) override;
	void resized() override;
	void updateText();

private:
	Label *txtBox;
	int num;
	int * note;
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
/*
*/
class lowMidQuadComp : public Component
{
public:
	lowMidQuadComp();
	~lowMidQuadComp();

	void paint(Graphics&) override;
	void resized() override;


private:
	ComboBox cb;
	//addAndMakeVisible(cb);
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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(lowMidQuadComp)
};