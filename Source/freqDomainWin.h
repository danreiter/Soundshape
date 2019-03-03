/*
  ==============================================================================

    freqDomainWin.h
    Created: 28 Dec 2018 11:48:50am
    Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define FREQ_DOMAIN 3000

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

	void setBase(int * _harm, int * _add, Slider::Listener* _parent,Button::Listener* _bParent, float* _profile, int _size);
	void setProfileControl(float * _profile, int _size);

private:
	int first;
	int * harm;
	int *add;
	float * profile;
	Slider::Listener* parent;
	Button::Listener* buttonParent;

	OwnedArray<TextButton> components;
	TextButton* addToList(TextButton * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		newComp->onClick = [this] {
			auto * focused = Component::getCurrentlyFocusedComponent();
			float margin = this->getHeight() *.10f;
			this->profile[focused->getComponentID().getIntValue()] = 0.0f;
			if (this->first < 0)
			{
				this->first = this->getComponentID().getIntValue();
			}
			focused->setVisible(false);

			sliders[this->getComponentID().getIntValue()]->setVisible(true);
		};
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
