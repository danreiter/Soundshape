/*
  ==============================================================================

    keyboarPopup.h
    Created: 9 Apr 2019 9:55:25pm
    Author:  Daniel Reiter
	Description:Program creates an instance of a MID keyboard in a popup window.  

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"

//==============================================================================
/**
Displays a playable MIDI keyboard.

@param _converter Instiates the keyboard state to send message to the back-end via a converter pointer
*/ 
//==============================================================================
class keyboardPopup : public Component
{
	public:
		keyboardPopup(Converter * _converter);
		~keyboardPopup();
		/**
		Used to start dragging the popup window on mouse down event.

		@see 
		JUCE documentation: MouseEvent::mouseDown().
		@param e Register a mouse down event.
		*/
		void mouseDown(const MouseEvent& e) override;
		/**
		Used to dragging the popup window while the mouse button is down.

		@see
		JUCE documentation: MouseEvent::mouseDrag().
		@param e Register a mouse drag event.
		*/
		void mouseDrag(const MouseEvent& e) override;
		/**
		Paints the keyboard and background above the keyboard.

		Function is called using the repaint() function.
		@see
		JUCE documentation: Component::paint(), Component::repaint()
		*/
		void paint(Graphics&) override;


	private:
		MidiKeyboardState keyboardState;            ///< Reference to the keyboard's MIDI state.
		MidiKeyboardComponent keyboardComponent;	///< MIDI keyboard.
		ComponentDragger dragger;					///< Used to handle dragging the window.
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(keyboardPopup)
};
//==============================================================================