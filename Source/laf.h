/*
  ==============================================================================

    laf.h
    Created: 5 Apr 2019 8:37:05pm
    Author:  Gregory

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/**
Class that holds the enum for Soundshape's Colors

This class has the SSColours enumeration that enables Soundshape
to swap color themes.  It could be incorporated into CustomLookAndFeel
in the future
*/
class SoundshapeLAFs
{
public:
	SoundshapeLAFs();
	~SoundshapeLAFs();
	//void setColors(LookAndFeel& laf);

	/**
	A custom enumeration for all the colour IDs Soundshape needs
	The default values do not matter. They will be overwritten when the initColors
	function in the CustomLookAndFeel class is called. The IDs correspond to the 
	following components:
	base1ID - The sliders, the scrollbar, the play button, the off position of the
	sustained play, add, and harmonic buttons, the time domain buttons, and the 
	background of the time domain windows
	base1textID - the text for every element colored with base1ID
	base2ID - the background of the siders and scrollbar, the write, import, and export
	buttons, the combo box, and the soundwave
	base2textID - the text for every element colored with base2ID
	background1ID - the entire background layer
	background2ID - the background of the combo box, the frequency domain window and the
	portions of the	time domain window that correspond to the frequency domain.
	background3ID - the background of the portion of the big time domain window that
	corresponds to the small time domain window
	*/
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

/**
Class that controls the LookAndFeel of Soundshape. Inherits from LookAndFeel_V2.

This class is used to not only have a LookAndFeel_V2 GUI but to also have the ability to change
colors, give sliders two different track colors, and treat LinearBarSliders differently.
*/
struct CustomLookAndFeel : public LookAndFeel_V2
{

public:
	/**
	This function is overridden in order to color both sides of each slider.  By default,
	LookAndFeel_V2 sliders only color the 'full' portion of the slider.  With this
	CustomLookAndFeel, all the sliders Soundshape needs (except for LinearBarSliders) have both 
	their 'full' and 'empty' halves colored in.
	*/
	void drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;
	/**
	This function is overridden in order to make sure the LinearBarSliders for the frequnecy spikes
	stay red and blue regardless of theme.  Everything else is default LookAndFeel_V2
	*/
	void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle style, Slider& slider) override;
	/**
	This function is overridden because the override of drawLinearSlider would not build without a 
	CustomLookAndFeel::drawShinyButtonShape
	*/
	void drawShinyButtonShape(Graphics& g, float x, float y, float w, float h,
		float maxCornerSize, const Colour& baseColour, float strokeWidth,
		bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom);
	/**
	Initializes the colors for the themes. Each of the seven color parameters gets set to the corresponding ID
	in the SoundshapeLAFs::SSColours enum
	*/
	void initColors(Colour base1, Colour base1text, Colour base2, Colour base2text, Colour background1, Colour background2,  Colour background3);
	/**
	Applies the colors initialized by initColors to every ColourID needed by Soundshape using the setColour
	function from LookAndFeel. Should be called after every initColors call.
	*/
	void setColors();

};