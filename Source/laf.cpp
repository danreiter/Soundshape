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
	setColour(Slider::textBoxOutlineColourId, base1);
	setColour(Slider::textBoxBackgroundColourId, base2);
	setColour(Slider::textBoxTextColourId, base2text);

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


}

//int LookAndFeel_V2::getSliderThumbRadius(Slider& slider)
//{
//	return jmin(7,
//		slider.getHeight() / 2,
//		slider.getWidth() / 2) + 2;
//}

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
		g.setColour(findColour(SoundshapeLAFs::base2ID));
	else
		g.setColour(findColour(SoundshapeLAFs::base1ID));
	g.fillPath(on);

	if(slider.isTwoValue())
		g.setColour(findColour(SoundshapeLAFs::base1textID));
	else
		g.setColour(findColour(SoundshapeLAFs::base2ID));
	g.fillPath(off);
}
