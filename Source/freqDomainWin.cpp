/*
  ==============================================================================

    freqDomainWin.cpp
    Created: 28 Dec 2018 11:48:50am
    Author:  danre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "freqDomainWin.h"

//==============================================================================
freqDomainWin::freqDomainWin()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	size = 4000;
	profile = new float[size];
	for (int i = 0; i < size; i++)
	{
		profile[i] = -1.0f;
	}
	first = -1;
	int temp = -1;
	add = &temp;
	harm = &temp;

}

freqDomainWin::~freqDomainWin()
{
}

void freqDomainWin::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	//g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	float pixel = getWidth() * .01f;
	int n = getWidth() * 10;
	float xMark = 0.0f;
	int colourMod = 0;
	bool flag = true;
	Colour c1;
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
	for (int i = 0; i <= n; i++)
	{


		Rectangle<float> rec5(xMark, 0.0f, pixel + (pixel * .1f), getHeight());
		xMark += pixel;
		if (flag)
		{
			c1 = Colour(255, (170 + colourMod), 0);
		}
		else
		{
			c1 = Colour(255, (200 - colourMod), 0);
		}
		g.setColour(c1);
		g.fillRect(rec5);
		colourMod = (++colourMod % 31);
		if (colourMod == 0)
		{
			flag = !flag;
		}
	}

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

	g.setColour(Colours::black);
	g.setFont(14.0f);
	g.drawText("Base Window", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text
	float margin = getHeight() *.10f;
	Line<float> vLine(margin, margin, margin, getHeight() - margin);
	Line<float> hLine(margin, getHeight() - margin, getWidth() - margin, getHeight() - margin);
	g.drawLine(vLine);
	g.drawLine(hLine);

	float vTick = vLine.getLength() / 10;
	float hTick = hLine.getLength() / 100;
	float smallTick = hLine.getLength() / size;
	Rectangle<float> vArea(.5f * margin, margin, margin, vLine.getLength());
	Rectangle<float> hArea(margin, getHeight() - (1.5f * margin), hLine.getLength(), margin);
	Rectangle<float> smallArea(margin, getHeight() - (1.25f *margin), hLine.getLength(), margin / 2);

	Rectangle<float> testPrint(margin, 1.25f *margin, hLine.getLength(), margin / 2);

	g.setFont(vTick * .8f);

	//g.fillRect(vArea);
	//g.fillRect(hArea);
	Rectangle<float> hTickArea(hArea);

	for (int i = 0; i < size; i++)
	{

		String temp = std::to_string(i);
		smallArea.removeFromLeft(smallTick);
		hTickArea.removeFromLeft(smallTick);
		if (i < 100 && i % 10 == 0)
		{
			g.drawText(temp, vArea, Justification::bottomLeft, true);
			vArea.removeFromBottom(vTick);
			g.drawLine(vArea.getRight(), vArea.getBottom(), vArea.getWidth(), vArea.getBottom());
		}
		if (i % 10 == 0)
		{
			g.drawText(temp, hTickArea, Justification::bottomLeft, true);
			//g.drawText(temp, smallArea, Justification::centred, true);
			/*hArea.removeFromLeft(hTick)*/;
			//Line<float> tLine(hArea.getBottomLeft(), hArea.getTopLeft());
			Line<float> tLine(hTickArea.getBottomLeft(), hTickArea.getTopLeft());
			g.drawLine(tLine);
		}

		//smallArea.removeFromLeft(smallTick);
		Line<float> tLine(smallArea.getBottomLeft(), smallArea.getTopLeft());
		g.drawLine(tLine);
		testPrint.removeFromLeft(smallTick);
		int t1 = (int)profile[i];
		String temp2 = std::to_string(t1);
	}

	emptyList();

	margin = getHeight() *.10f;

	//Rectangle<float> smallArea(margin, getHeight() - (1.25f *margin), hLine.getLength(), margin / 2);
	Rectangle<float> btnArea(margin, getHeight() - (1.25f *margin), getWidth() - (2 * margin), margin / 4);
	float tick = btnArea.getWidth() / 1024;


	for (int i = 0; i < size; i++)
	{

		Path btnPath;
		DrawablePath normal, down, over;

		btnArea.removeFromLeft(tick);

		if (profile[i] < 0)
		{
			if (*add > 0)
			{
				if (harm && first > 0 && (i % first == 0))
				{

				}
				else if (harm && first > 0)
				{

				}
				else
				{

					auto * tb = addToList(new TextButton(""));
					tb->setComponentID(String(i));
					tb->setClickingTogglesState(true);
					tb->addListener(buttonParent);
					tb->onClick = [this] {
						auto * focused = Component::getCurrentlyFocusedComponent();
						float margin = this->getHeight() *.10f;
						this->profile[focused->getComponentID().getIntValue()] = 0.0f;
						if (this->first < 0)
						{
							this->first = this->getComponentID().getIntValue();
						}
						focused->setVisible(false);

						auto * sb = this->createSlider();
						sb->setRange(0.0, 100.0, 0.1);
						sb->setSliderStyle(Slider::LinearBarVertical);
						sb->setComponentID(String(focused->getComponentID().getIntValue()));
						sb->setValue((double)(profile[focused->getComponentID().getIntValue()]), sendNotificationAsync);
						sb->setColour(Slider::trackColourId, Colours::red);
						sb->setTextBoxIsEditable(false);
						sb->setPopupDisplayEnabled(true, true, this);
						sb->addListener(parent);
						sb->setBounds(focused->getX() + ((margin * 3) / 8), margin, margin / 2, this->getHeight() - (2 * margin));
						this->repaint();
					};
					tb->setColour(TextButton::textColourOnId, Colours::black);
					tb->setColour(TextButton::buttonColourId, Colours::white);
					tb->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
					tb->setColour(TextButton::textColourOffId, Colours::black);
					tb->setBounds(btnArea.getX() - (margin / 2), btnArea.getY() - (margin / 4), margin, margin);
				}
			}
		}
		else
		{
			auto * sb = createSlider();
			sb->setRange(0.0, 100.0, 0.1);
			sb->setSliderStyle(Slider::LinearBarVertical);
			sb->setComponentID(String(i));
			sb->setValue((double)(profile[i]), sendNotificationAsync);
			sb->setColour(Slider::trackColourId, Colours::red);
			sb->setTextBoxIsEditable(false);
			sb->setPopupDisplayEnabled(true, true, this);
			sb->addListener(parent);
			sb->setBounds(btnArea.getX() - (margin / 8), margin, margin / 2, getHeight() - (2 * margin));
		}


	}
}

void freqDomainWin::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	//emptyList();

	//float margin = getHeight() *.10f;

	////Rectangle<float> smallArea(margin, getHeight() - (1.25f *margin), hLine.getLength(), margin / 2);
	//Rectangle<float> btnArea(margin, getHeight() - (1.25f *margin), getWidth() - (2 * margin), margin / 4);
	//float tick = btnArea.getWidth() / 1024;


	//for (int i = 0; i < 1024; i++)
	//{

	//	Path btnPath;
	//	DrawablePath normal, down, over;

	//	btnArea.removeFromLeft(tick);

	//	if (profile[i] < 0)
	//	{
	//		if (*add > 0)
	//		{
	//			if (harm && first > 0 && (i % first == 0))
	//			{

	//			}
	//			else if (harm && first > 0)
	//			{

	//			}
	//			else
	//			{

	//				auto * tb = addToList(new TextButton(""));
	//				tb->setComponentID(String(i));
	//				tb->setClickingTogglesState(true);
	//				tb->onClick = [this] {
	//					auto * focused = Component::getCurrentlyFocusedComponent();
	//					float margin = this->getHeight() *.10f;
	//					this->profile[focused->getComponentID().getIntValue()] = 0.0f;
	//					if (this->first < 0)
	//					{
	//						this->first = this->getComponentID().getIntValue();
	//					}
	//					focused->setVisible(false);

	//					auto * sb = this->createSlider();
	//					sb->setRange(0.0, 100.0, 0.1);
	//					sb->setSliderStyle(Slider::LinearBarVertical);
	//					sb->setComponentID(String(focused->getComponentID().getIntValue()));
	//					sb->setValue((double)(profile[focused->getComponentID().getIntValue()]), sendNotificationAsync);
	//					sb->setColour(Slider::trackColourId, Colours::red);
	//					sb->setTextBoxIsEditable(false);
	//					sb->setPopupDisplayEnabled(true, true, this);
	//					sb->addListener(parent);
	//					sb->setBounds(focused->getX() + ((margin * 3) / 8), margin, margin / 2, this->getHeight() - (2 * margin));
	//					this->repaint();
	//				};
	//				tb->setColour(TextButton::textColourOnId, Colours::black);
	//				tb->setColour(TextButton::buttonColourId, Colours::white);
	//				tb->setColour(TextButton::buttonOnColourId, Colours::blueviolet.brighter());
	//				tb->setColour(TextButton::textColourOffId, Colours::black);
	//				tb->setBounds(btnArea.getX() - (margin / 2), btnArea.getY() - (margin / 4), margin, margin);
	//			}
	//		}
	//	}
	//	else
	//	{
	//		auto * sb = createSlider();
	//		sb->setRange(0.0, 100.0, 0.1);
	//		sb->setSliderStyle(Slider::LinearBarVertical);
	//		sb->setComponentID(String(i));
	//		sb->setValue((double)(profile[i]), sendNotificationAsync);
	//		sb->setColour(Slider::trackColourId, Colours::red);
	//		sb->setTextBoxIsEditable(false);
	//		sb->setPopupDisplayEnabled(true, true, this);
	//		sb->addListener(parent);
	//		sb->setBounds(btnArea.getX() - (margin / 8), margin, margin / 2, getHeight() - (2 * margin));
	//	}


	//}

}

void freqDomainWin::setBase(int * _harm, int * _add, Slider::Listener* _parent, Button::Listener* _bParent, float* _profile, int _size)
{
	harm = _harm;
	add = _add;
	parent = _parent;
	//profile = new float[_size];
	profile = _profile;
	buttonParent = _bParent;
	size = _size;
}

void freqDomainWin::setProfile(float* _profile)
{
	profile = _profile;
}
