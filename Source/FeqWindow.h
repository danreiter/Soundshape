/*
  ==============================================================================

	FreqWindow.h
	Created: 20 Dec 2018 12:59:50pm
	Author:  Daniel Reiter
	Description: Handles the viewport for the frequency domain

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

	// Function get referneces from the parent component
	void setZoom(double* _zoom, int* _harm, int * _add, Slider::Listener * _parent, Button::Listener* _bParent, float * _profile, int _size);

private:
	freqDomainWin base;	   // Frequency domian for the GUI 
	Viewport view;         // Viewport for frequency domain

	double *zoom;          // Factor of horizontal zoom for the frequency domain window

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeqWindow)
};
