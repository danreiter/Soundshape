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

void SoundshapeLAFs::setColors(LookAndFeel& laf) // there will be one of these for each theme
{// need to create an array parameter to hold the base and background colours

	Colour base1 = Colour(base1ID);
	Colour base1text = Colour(base1textID);
	Colour base2 = Colour(base2ID);
	Colour base2text = Colour(base2textID);

	laf.setColour(ResizableWindow::backgroundColourId, base2); // may need to change

	laf.setColour(Slider::thumbColourId, base1);
	laf.setColour(Slider::trackColourId, base1);
	laf.setColour(Slider::textBoxOutlineColourId, base1);
	laf.setColour(Slider::textBoxBackgroundColourId, base2);
	laf.setColour(Slider::textBoxTextColourId, base2text);

	laf.setColour(TextButton::buttonColourId, base1);
	laf.setColour(TextButton::textColourOffId, base1text);
	laf.setColour(TextButton::buttonOnColourId, base2);
	laf.setColour(TextButton::textColourOnId, base2text);

	laf.setColour(ComboBox::backgroundColourId, Colours::peachpuff); // change
	laf.setColour(ComboBox::textColourId, base1text);
	laf.setColour(ComboBox::arrowColourId, base1text);
	laf.setColour(ComboBox::buttonColourId, base2);
	laf.setColour(ComboBox::focusedOutlineColourId, base1text);
	laf.setColour(ComboBox::outlineColourId, base2);

	laf.setColour(ScrollBar::thumbColourId, base1);
	laf.setColour(ScrollBar::trackColourId, base2);
	laf.setColour(ScrollBar::backgroundColourId, base2);


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
		g.setColour(Colour(SoundshapeLAFs::base2ID));
	else
		g.setColour(Colour(SoundshapeLAFs::base1ID));
	g.fillPath(on);

	if(slider.isTwoValue())
		g.setColour(Colour(SoundshapeLAFs::base1textID));
	else
		g.setColour(Colour(SoundshapeLAFs::base2ID));
	g.fillPath(off);
}
