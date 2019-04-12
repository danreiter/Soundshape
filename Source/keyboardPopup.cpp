/*
  ==============================================================================

    keyboarPopup.cpp
    Created: 9 Apr 2019 9:55:25pm
    Author:  danre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "keyboardPopup.h"

//==============================================================================

	keyboardPopup::keyboardPopup(Converter* _converter) : keyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard) {

		keyboardState.addListener(_converter);
		addAndMakeVisible(keyboardComponent);

		setSize(1000, 400);
	}

	keyboardPopup::~keyboardPopup() {}

	void keyboardPopup::mouseDown(const MouseEvent& e)
	{
		dragger.startDraggingComponent(this, e);
	}

	void keyboardPopup::mouseDrag(const MouseEvent& e)
	{
		// as there's no titlebar we have to manage the dragging ourselves
		dragger.dragComponent(this, e, nullptr);
	}

	void keyboardPopup::paint(Graphics &g)
	{
		g.fillAll(Colours::black);
		Rectangle<float> titleArea(0, 0, getWidth(), getHeight() / 2);
		g.setColour(Colours::white);


		g.setFont(Font::bold);
		g.setFont(Font::italic);
		g.setFont((getHeight() / 2) * .8);
		g.drawText("Soundshape", titleArea, Justification::centred, true);
		keyboardComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);

	}


