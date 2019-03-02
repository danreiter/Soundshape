/*
  ==============================================================================

	smallTime.h
	Created: 20 Dec 2018 1:00:26pm
	Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "timeDomainWin.h"

//==============================================================================
/*
*/
class smallTime : public Component
{
public:
	smallTime();
	~smallTime();

	void paint(Graphics&) override;
	void resized() override;
	void setTimeDomain(int * _start, int * _profile, int * _time, Button::Listener* _parent);


private:
	int * xStart;
	int * xProfile;
	timeDomainWin tdTest;
	Viewport view;
	int * time;
	Button::Listener* parent;

	OwnedArray<Component> components;
	template <typename ComponentType>
	ComponentType* addToList(ComponentType * newComp)
	{
		components.add(newComp);
		addAndMakeVisible(newComp);
		return newComp;
	}
	//template <typename ComponentType>
	void emptyList()
	{
		components.clear(true);
	}


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(smallTime)
};
