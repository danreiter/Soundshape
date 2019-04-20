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
	delete zoom;
}
//==============================================================================

//==============================================================================
// Function set zoom to a reference from the parent and passes refences to the frequency domain component
//==============================================================================
void FeqWindow::setZoom(double * _zoom, int* _harm, int * _add, Slider::Listener * _parent,Button::Listener* _bParent,  Converter * _profile, int _size, int* _chunk, CustomLookAndFeel * _laf)
{

	zoom = _zoom;                                                   // pass reference to zoom
	base.setBase(_harm, _add, _parent, _bParent, _profile, _size, _chunk, _laf);
	view.getHorizontalScrollBar().setLookAndFeel(_laf); // passes referneces to frequency domain component
}
//==============================================================================

//==============================================================================
// Function set zoom to a reference from the parent and passes refences to the frequency domain component
//==============================================================================
void FeqWindow::setProfile()
{
	base.setProfile();
}
//==============================================================================


void FeqWindow::paint(Graphics& g)
{
	// set the base (domain frequency) to the level of zoom and the bounds of the viewport
	base.setSize(*(zoom)*getWidth(), getHeight() - view.getScrollBarThickness());
	view.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
}

void FeqWindow::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
}
