/*
  ==============================================================================

    timeDomainWin.cpp
    Created: 28 Dec 2018 11:49:15am
    Author:  danre

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "timeDomainWin.h"

//==============================================================================
timeDomainWin::timeDomainWin()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

timeDomainWin::~timeDomainWin()
{
}

void timeDomainWin::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	//float pixel = getWidth() * .01f;
	//int n = getWidth() * 10;
	//float xMark = 0.0f;
	//int colourMod = 0;
	//bool flag = true;
	//Colour c1;
	//g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
	//for (int i = 0; i <= n; i++)
	//{


	//	Rectangle<float> rec5(xMark, 0.0f, pixel + (pixel * .1f), getHeight());
	//	xMark += pixel;
	//	if (flag)
	//	{
	//		c1 = Colour(255, (170 + colourMod), 0);
	//	}
	//	else
	//	{
	//		c1 = Colour(255, (200 - colourMod), 0);
	//	}
	//	g.setColour(c1);
	//	g.fillRect(rec5);
	//	colourMod = (++colourMod % 31);
	//	if (colourMod == 0)
	//	{
	//		flag = !flag;
	//	}
	//}

	g.setColour(Colours::black);
	Point<float> centerStart(0.0f, getHeight() / 2.0f);
	Point<float> centerEnd(getWidth(), getHeight()/2.0f);
	Line<float> centerLine(centerStart, centerEnd);
	g.drawLine(centerLine, 2.0f);

	float tick = getWidth() / 50.0f;


	Rectangle<float> rec(0.0f, (getHeight() / 2) - (getHeight() * .1f), getWidth(), getHeight() * .2f);
	Rectangle<float> rec1(0.0f, (getHeight() / 2) + (getHeight() * .1f), getWidth(), getHeight() * .2f);

	for (int i = 0; i <= 50; i++)
	{
		Point<float> p1(rec.getTopLeft());
		Point<float> p2(rec.getBottomLeft());
		if (i % 10 != 0)
		{
			p1.setY(p1.getY() + (getHeight()*.05f));
			p2.setY(p2.getY() - (getHeight()*.05f));
		}
		Line<float> l(p1, p2);
		g.drawLine(l);
		g.drawText(String(i), rec1, Justification::centredLeft);
		rec.removeFromLeft(tick);
		rec1.removeFromLeft(tick);
	}
}

void timeDomainWin::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
