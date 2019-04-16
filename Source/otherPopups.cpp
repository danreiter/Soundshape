/*
  ==============================================================================

    otherPopups.cpp
    Created: 13 Apr 2019 12:55:29am
    Author:  Gregory

  ==============================================================================
*/

#include "otherPopups.h"

LWindow::LWindow(const String& name, Colour backgroundColour, int buttonsNeeded, bool * vis)
	: DocumentWindow (name, backgroundColour, buttonsNeeded)
{
	

	this->vis = vis;
	setTitleBarTextCentred(false);
	setUsingNativeTitleBar(false);

}

LWindow::~LWindow()
{

	

}

void LWindow::closeButtonPressed()
{
	*vis = false;
	delete this;

}

void LWindow::paint(Graphics& g)
{
	String m;

	m << "Dialog Windows can be used to quickly show a component, usually blocking mouse input to other windows." << newLine
		<< newLine
		<< "They can also be quickly closed with the escape key, try it now.";


	//DialogWindow::LaunchOptions options

	//g.fillAll(Colours::white);

	//Rectangle<float> titleArea(0, 10, getWidth(), getHeight() / 8);
	//g.setFont(Font::bold);
	//g.setFont(Font::italic);
	//g.setFont((getHeight() / 2) * .8);
	//g.drawText("License", titleArea, Justification::centred, true);

}