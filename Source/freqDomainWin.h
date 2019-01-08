/*
  ==============================================================================

    freqDomainWin.h
    Created: 28 Dec 2018 11:48:50am
    Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class freqDomainWin    : public Component
{
public:
    freqDomainWin();
    ~freqDomainWin();

    void paint (Graphics&) override;
    void resized() override;

	void setBase(int * _harm, int * _add, Slider::Listener* _parent, float* _profile, int _size);

private:
	int first;
	int * harm;
	int *add;
	float * profile;
	Slider::Listener* parent;

	OwnedArray<Component> components;
	template <typename ComponentType>
	ComponentType* addToList(ComponentType * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}

	OwnedArray<Slider> sliders;
	Slider * createSlider()
	{
		Slider * s = new Slider();
		sliders.add(s);
		addAndMakeVisible(s);
		return s;
	}

	void emptyList()
	{
		components.clear(true);
		sliders.clear(true);
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (freqDomainWin)
};
