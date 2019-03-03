/*
  ==============================================================================

	FreqWindow.cpp
	Created: 20 Dec 2018 12:59:50pm
	Author:  danre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FeqWindow.h"

//==============================================================================
FeqWindow::FeqWindow()
{
	base.setSize(1000, 1000);
	view.setViewedComponent(&base, false);
	addAndMakeVisible(view);
	double num = 3.0;
	zoom = &num;

}

FeqWindow::~FeqWindow()
{
}



void FeqWindow::setZoom(double * _zoom, int* _harm, int * _add, Slider::Listener * _parent,Button::Listener* _bParent,  float * _profile, int _size)
{
	zoom = _zoom;
	base.setBase(_harm, _add, _parent, _bParent, _profile, _size);
}

void FeqWindow::paint(Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	


	g.setColour(Colours::white);
	g.setFont(14.0f);
	g.drawText("viewWindow", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text

	base.setSize(*(zoom)*getWidth(), getHeight() - view.getScrollBarThickness());
	view.setBoundsRelative(0.0f, 0.0f, 1.0f, 1.0f);
	
	g.setColour(Colours::black);
	g.drawLine(0.0f, getHeight() - view.getScrollBarThickness(), getWidth(), getHeight() - view.getScrollBarThickness());
	g.drawRect(getLocalBounds(), 1);


}

void FeqWindow::resized()
{
	// This method is where you should set the bounds of any child
	// components that your component contains..


}

void FeqWindow::setProfile(float* _profile)
{
	base.setProfile(_profile);
}