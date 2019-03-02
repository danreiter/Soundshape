/*
  ==============================================================================

	FreqWindow.h
	Created: 20 Dec 2018 12:59:50pm
	Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "freqDomainWin.h"

//==============================================================================
/*
*/
class FeqWindow : public Component
{
public:
	FeqWindow();
	~FeqWindow();

	void paint(Graphics&) override;
	void resized() override;
	void setZoom(double* _zoom, int* _harm, int * _add, Slider::Listener * _parent, Button::Listener* _bParent, float * _profile, int _size);

private:
	freqDomainWin base;
	Viewport view;

	double *zoom;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeqWindow)
};
