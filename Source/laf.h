/*
  ==============================================================================

    laf.h
    Created: 5 Apr 2019 8:37:05pm
    Author:  Gregory

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SoundshapeLAFs : public Component
{
public:
	SoundshapeLAFs();
	~SoundshapeLAFs();
	static void setColors(LookAndFeel& laf); // there will be one of these for each theme.

	// custom enum for all the colour IDs Soundshape needs
	enum SSColours
	{
		// these are the default values
		base1ID = 0xffffa500, // orange
		base1textID = 0xff000000, // black
		base2ID = 0xff00008b, // darkblue
		base2textID = 0xffffffff, // white
		background1ID = 0xffd3d3d3, // lightgrey
		background2ID = 0xffffdab9, // peachpuff
		background3ID = 0xffff0000 // red

	};

private:
	
	//CustomLookAndFeel * claf;

};

struct CustomLookAndFeel : public LookAndFeel_V2
{

	
	void drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;

};