/*
  ==============================================================================

    laf.h
    Created: 5 Apr 2019 8:37:05pm
    Author:  Gregory

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SoundshapeLAFs
{
public:
	SoundshapeLAFs();
	~SoundshapeLAFs();
	//void setColors(LookAndFeel& laf);

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

public:
	
	void drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;

	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;

	void drawShinyButtonShape(Graphics& g, float x, float y, float w, float h,
		float maxCornerSize, const Colour& baseColour, float strokeWidth,
		bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom);

	void initColors(Colour base1, Colour base1text, Colour base2, Colour base2text, Colour background1, Colour background2,  Colour background3);

	void setColors();

};