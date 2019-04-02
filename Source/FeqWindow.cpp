/*
  ==============================================================================

	FreqWindow.cpp
	Created: 20 Dec 2018 12:59:50pm
	Author:  Daniel Reiter
	Description: Handles the viewport for the frequency domain

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FeqWindow.h"

//==============================================================================
//  FreqWindow constructor
//==============================================================================
FeqWindow::FeqWindow()
{
	// default variable settings
	base.setSize(1000, 1000);
	view.setViewedComponent(&base, false);
	addAndMakeVisible(view);
	double num = 3.0;
	zoom = &num;

}

FeqWindow::~FeqWindow()
{
}
//==============================================================================

//==============================================================================
// Function set zoom to a reference from the parent and passes refences to the frequency domain component
//==============================================================================
void FeqWindow::setZoom(double * _zoom, int* _harm, int * _add, Slider::Listener * _parent,Button::Listener* _bParent,  Converter * _profile, int _size, int* _chunk)
//void FeqWindow::setZoom(double * _zoom, int* _harm, int * _add, Slider::Listener * _parent, Button::Listener* _bParent, float * _profile, int _size, int* _chunk)
{
	zoom = _zoom;                                                   // pass reference to zoom
	base.setBase(_harm, _add, _parent, _bParent, _profile, _size, _chunk);  // passes referneces to frequency domain component
}
//==============================================================================

//==============================================================================
// Function set zoom to a reference from the parent and passes refences to the frequency domain component
//==============================================================================
void FeqWindow::setProfile(int _timeBlock, int _selectedProfile)
{
	base.setProfile(_timeBlock, _selectedProfile);
}
//==============================================================================


void FeqWindow::paint(Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/
	// default widow settings
	g.setColour(Colours::white);
	g.setFont(14.0f);
	g.drawText("viewWindow", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text

	// set the base (domain frequency) to the level of zoom and the bounds of the viewport
	base.setSize(*(zoom)*getWidth(), getHeight() - view.getScrollBarThickness());
	view.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
	
	// draw outline around window
	g.setColour(Colours::black);
	g.drawLine(0.0f, getHeight() - view.getScrollBarThickness(), getWidth(), getHeight() - view.getScrollBarThickness());
	g.drawRect(getLocalBounds(), 1);


}

void FeqWindow::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
}
