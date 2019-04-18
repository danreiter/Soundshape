/*
  ==============================================================================

    keyboarPopup.h
    Created: 9 Apr 2019 9:55:25pm
    Author:  Daniel Reiter
	Description:

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"

//==============================================================================
/* KeyboardPopup window
*/ 
//==============================================================================
class keyboardPopup : public Component
{
	public:
		keyboardPopup(Converter * _converter);
		~keyboardPopup();
		void mouseDown(const MouseEvent& e) override;
		void mouseDrag(const MouseEvent& e) override;
		void paint(Graphics&) override;


	private:
		MidiKeyboardState keyboardState;
		MidiKeyboardComponent keyboardComponent;
		ComponentDragger dragger;
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(keyboardPopup)
};
//==============================================================================