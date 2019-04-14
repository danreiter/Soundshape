/*
  ==============================================================================

    otherPopups.h
    Created: 13 Apr 2019 12:55:29am
    Author:  Gregory

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//License Window
class LWindow : public DocumentWindow
{

public:

	LWindow(const String& name, Colour backgroundColour, int buttonsNeeded, bool * vis);
	~LWindow();
	void closeButtonPressed() override;
	void paint(Graphics& g) override;

private:

	bool * vis;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LWindow)


};