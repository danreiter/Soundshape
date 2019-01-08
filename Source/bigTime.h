/*
  ==============================================================================

    bigTime.h
    Created: 24 Dec 2018 12:43:15pm
    Author:  danre

  ==============================================================================
/*
  ==============================================================================

	bigTime.h
	Created: 20 Dec 2018 1:00:48pm
	Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "timeDomainWin.h"

//==============================================================================
/*
*/
class bigTime : public Component
{
public:
	bigTime();
	~bigTime();

	void paint(Graphics&) override;
	void resized() override;

	void setProfile(int * _Xpoint, int * _profile, int * _time);

private:
	timeDomainWin timeBase;
	Viewport view;

	int * xPoint;
	int * xProfile;
	int *time;

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


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(bigTime)
};
