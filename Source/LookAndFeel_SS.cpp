/*
  ==============================================================================

	LookAndFeel_SS.h
	Created: 27 Mar 2019 1:18:40am
	Author:  Gregory Hughes
	Description: Components to control the Look and Feel of Soundshape

  ==============================================================================
*/

#include "LookAndFeel_SS.h"

SoundshapeLAFs::SoundshapeLAFs()
{

	darkLaf = new LookAndFeel_V2();

}

SoundshapeLAFs::~SoundshapeLAFs() {}

void SoundshapeLAFs::setDarkLafColors(LookAndFeel& laf)
{// need to change the colors

	laf.setColour(Slider::thumbColourId, Colour::greyLevel(0.95f));
	laf.setColour(Slider::textBoxOutlineColourId, Colours::transparentWhite);
	laf.setColour(Slider::rotarySliderFillColourId, Colour(0xff00b5f6));
	laf.setColour(Slider::rotarySliderOutlineColourId, Colours::white);

	laf.setColour(TextButton::buttonColourId, Colours::white);
	laf.setColour(TextButton::textColourOffId, Colour(0xff00b5f6));

	laf.setColour(TextButton::buttonOnColourId, laf.findColour(TextButton::textColourOffId));
	laf.setColour(TextButton::textColourOnId, laf.findColour(TextButton::buttonColourId));

}