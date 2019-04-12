/*
  ==============================================================================

    laf.cpp
    Created: 5 Apr 2019 8:37:27pm
    Author:  Gregory

  ==============================================================================
*/

#include "laf.h"

SoundshapeLAFs::SoundshapeLAFs()
{

	

}

SoundshapeLAFs::~SoundshapeLAFs() {}

void CustomLookAndFeel::initColors(Colour base1, Colour base1text, Colour base2, Colour base2text, Colour background1, Colour background2, Colour background3)
{

	setColour(SoundshapeLAFs::base1ID, base1);
	setColour(SoundshapeLAFs::base1textID, base1text);
	setColour(SoundshapeLAFs::base2ID, base2);
	setColour(SoundshapeLAFs::base2textID, base2text);
	setColour(SoundshapeLAFs::background1ID, background1);
	setColour(SoundshapeLAFs::background2ID, background2);
	setColour(SoundshapeLAFs::background3ID, background3);

}

void CustomLookAndFeel::setColors() // there will be one of these for each theme
{// need to create an array parameter to hold the base and background colours

	Colour base1 = findColour(SoundshapeLAFs::base1ID);
	Colour base1text = findColour(SoundshapeLAFs::base1textID);
	Colour base2 = findColour(SoundshapeLAFs::base2ID);
	Colour base2text = findColour(SoundshapeLAFs::base2textID);
	Colour background2 = findColour(SoundshapeLAFs::background2ID);

	setColour(ResizableWindow::backgroundColourId, base2); // may need to change

	setColour(Slider::thumbColourId, base1);
	setColour(Slider::trackColourId, base1);
	setColour(Slider::textBoxOutlineColourId, Colours::black); // these never changed for some reason so I'm making them neutral colors
	setColour(Slider::textBoxBackgroundColourId, Colours::white); // ditto
	setColour(Slider::textBoxTextColourId, Colours::black);
	setColour(Slider::textBoxHighlightColourId, Colours::yellow);

	setColour(TextButton::buttonColourId, base1);
	setColour(TextButton::textColourOffId, base1text);
	setColour(TextButton::buttonOnColourId, base2);
	setColour(TextButton::textColourOnId, base2text);

	setColour(ComboBox::backgroundColourId, background2); // change
	setColour(ComboBox::textColourId, base1text);
	setColour(ComboBox::arrowColourId, base1text);
	setColour(ComboBox::buttonColourId, base2);
	setColour(ComboBox::focusedOutlineColourId, base1text);
	setColour(ComboBox::outlineColourId, base2);

	setColour(ScrollBar::thumbColourId, base1);
	setColour(ScrollBar::trackColourId, base2);
	setColour(ScrollBar::backgroundColourId, base2);

	setColour(Label::textColourId, base2text);


}

namespace LookAndFeelHelpers
{
	static Colour createBaseColour(Colour buttonColour,
		bool hasKeyboardFocus,
		bool shouldDrawButtonAsHighlighted,
		bool shouldDrawButtonAsDown) noexcept
	{
		const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
		const Colour baseColour(buttonColour.withMultipliedSaturation(sat));

		if (shouldDrawButtonAsDown)        return baseColour.contrasting(0.2f);
		if (shouldDrawButtonAsHighlighted) return baseColour.contrasting(0.1f);

		return baseColour;
	}

	static TextLayout layoutTooltipText(const String& text, Colour colour) noexcept
	{
		const float tooltipFontSize = 13.0f;
		const int maxToolTipWidth = 400;

		AttributedString s;
		s.setJustification(Justification::centred);
		s.append(text, Font(tooltipFontSize, Font::bold), colour);

		TextLayout tl;
		tl.createLayoutWithBalancedLineLengths(s, (float)maxToolTipWidth);
		return tl;
	}
}

void CustomLookAndFeel::drawShinyButtonShape(Graphics& g, float x, float y, float w, float h,
	float maxCornerSize, const Colour& baseColour, float strokeWidth,
	bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom)
{
	if (w <= strokeWidth * 1.1f || h <= strokeWidth * 1.1f)
		return;

	auto cs = jmin(maxCornerSize, w * 0.5f, h * 0.5f);

	Path outline;
	outline.addRoundedRectangle(x, y, w, h, cs, cs,
		!(flatOnLeft || flatOnTop),
		!(flatOnRight || flatOnTop),
		!(flatOnLeft || flatOnBottom),
		!(flatOnRight || flatOnBottom));

	ColourGradient cg(baseColour, 0.0f, y,
		baseColour.overlaidWith(Colour(0x070000ff)), 0.0f, y + h,
		false);

	cg.addColour(0.5, baseColour.overlaidWith(Colour(0x33ffffff)));
	cg.addColour(0.51, baseColour.overlaidWith(Colour(0x110000ff)));

	g.setGradientFill(cg);
	g.fillPath(outline);

	g.setColour(Colour(0x80000000));
	g.strokePath(outline, PathStrokeType(strokeWidth));
}

void CustomLookAndFeel::drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height,
	float /*sliderPos*/,
	float /*minSliderPos*/,
	float /*maxSliderPos*/,
	const Slider::SliderStyle /*style*/, Slider& slider)
{
	auto sliderRadius = getSliderThumbRadius(slider) - 5.0f;
	Path on, off;

	
	if (slider.isTwoValue() && slider.isHorizontal())
	{

		auto iy = y + height * 0.5f - sliderRadius * 0.5f;
		Rectangle<float> r(x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius);
		auto onWleft = r.getWidth() * ((float)slider.valueToProportionOfLength(slider.getMinValue()));
		auto onWright = r.getWidth() * ((float)slider.valueToProportionOfLength(slider.getMaxValue()));

		off.addRectangle(r.removeFromLeft(onWleft));
		on.addRectangle(r.removeFromLeft(onWright - onWleft));
		off.addRectangle(r);

	}
	else if (slider.isHorizontal())
	{
		auto iy = y + height * 0.5f - sliderRadius * 0.5f;
		Rectangle<float> r(x - sliderRadius * 0.5f, iy, width + sliderRadius, sliderRadius);
		auto onW = r.getWidth() * ((float)slider.valueToProportionOfLength(slider.getValue()));

		on.addRectangle(r.removeFromLeft(onW));
		off.addRectangle(r);
	}
	else
	{
		auto ix = x + width * 0.5f - sliderRadius * 0.5f;
		Rectangle<float> r(ix, y - sliderRadius * 0.5f, sliderRadius, height + sliderRadius);
		auto onH = r.getHeight() * ((float)slider.valueToProportionOfLength(slider.getValue()));

		on.addRectangle(r.removeFromBottom(onH));
		off.addRectangle(r);
	}
	if(slider.isTwoValue())
		g.setColour(Colours::white);
	else
		g.setColour(findColour(SoundshapeLAFs::base1ID));
	g.fillPath(on);

	if(slider.isTwoValue())
		g.setColour(findColour(SoundshapeLAFs::base2ID));
	else
		g.setColour(Colours::grey);
	g.fillPath(off);
}

void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
	float sliderPos, float minSliderPos, float maxSliderPos,
	const Slider::SliderStyle style, Slider& slider)
{
	//g.fillAll(slider.findColour(Slider::backgroundColourId));

	if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
	{
		width *= 2;
		const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

		Colour baseColour;
		int sliderID = slider.getComponentID().getIntValue();
		if (sliderID % 440 == 0 && sliderID != 0)
			baseColour = Colour(0xffee0000);
		else
			baseColour = Colour(0xff0000ee);

		drawShinyButtonShape(g,
			(float)x,
			style == Slider::LinearBarVertical ? sliderPos
			: (float)y,
			style == Slider::LinearBarVertical ? (float)(width)
			: (sliderPos - x),
			style == Slider::LinearBarVertical ? (height - sliderPos)
			: (float)height, 0.0f,
			baseColour,
			slider.isEnabled() ? 0.9f : 0.3f,
			true, true, true, true);
	}
	else
	{
		drawLinearSliderBackground(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
		drawLinearSliderThumb(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
	}
}
