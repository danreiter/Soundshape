/*
  ==============================================================================

    otherPopups.cpp
    Created: 13 Apr 2019 12:55:29am
    Author:  Gregory

  ==============================================================================
*/

#include "otherPopups.h"

LWindow::LWindow()
{

	setSize(1200, 1200);

}

LWindow::~LWindow(){}

void LWindow::paint(Graphics& g)
{

	g.fillAll(Colours::white);

	Rectangle<float> titleArea(0, 10, getWidth(), getHeight() / 8);
	g.setFont(Font::bold);
	g.setFont(Font::italic);
	g.setFont((getHeight() / 2) * .8);
	g.drawText("License", titleArea, Justification::centred, true);

}