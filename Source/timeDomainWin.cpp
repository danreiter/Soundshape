/*
  ==============================================================================

    timeDomainWin.cpp
    Created: 28 Dec 2018 11:49:15am
    Author:  Daniel Reiter
	Description: Time domain object renders sound in the time domain and draw center line with a
					scale.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "timeDomainWin.h"

//==============================================================================
//  Constructor
//==============================================================================
timeDomainWin::timeDomainWin()
{
	converterPtr = NULL;
	currentProfile = new int();
	*currentProfile = -1;
}

timeDomainWin::~timeDomainWin()
{
}
//==============================================================================

//==============================================================================
// Function Paint
//==============================================================================
void timeDomainWin::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	// draw center line across window 
	g.setColour(Colours::black);
	Point<float> centerStart(0.0f, getHeight() / 2.0f);
	Point<float> centerEnd(getWidth(), getHeight()/2.0f);
	Line<float> centerLine(centerStart, centerEnd);
	//g.drawLine(centerLine, 2.0f);

	// draw tick marks and numbers across center line
	float tick = getWidth() / 50.0f;
	Rectangle<float> rec(0.0f, (getHeight() / 2) - (getHeight() * .1f), getWidth(), getHeight() * .2f);
	Rectangle<float> rec1(0.0f, (getHeight() / 2) + (getHeight() * .1f), getWidth(), getHeight() * .2f);
    Rectangle<int> waveRect = rec.toNearestInt();
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
		if (i == *currentProfile)
		{
			g.setColour(Colours::red);
			Rectangle<float> profile(rec.getTopLeft().getX(), 0, tick, getHeight());
			g.fillRect(profile);
			g.setColour(Colours::black);
		}
		g.drawLine(l);
		g.drawText(String(i), rec1, Justification::centredLeft);
		rec.removeFromLeft(tick);
		rec1.removeFromLeft(tick);
	}
	g.drawLine(centerLine, 2.0f);

	if (converterPtr != NULL)
	{
		Path wavePath;
		float waveHeight = (float)getHeight() * .8f;
		wavePath.startNewSubPath(0, getHeight() / 2);
		for(int j = 0; j < 50; j++)
		{
			for (int i = 0; i < SOUNDSHAPE_PREVIEW_CHUNK_SIZE; i++) {
				//float x = ((float)i / SOUNDSHAPE_PREVIEW_CHUNK_SIZE) * getWidth();
				//float y = (((float)getHeight()) / 2.0f - (0.5f * getHeight() * 15 * converterPtr->getPreviewSample(0, i)));
                float x = (((float)(i) / SOUNDSHAPE_PREVIEW_CHUNK_SIZE) * tick) + (j *tick);
				float y = (((float)getHeight()) / 2.0f - (0.5f * waveHeight * 15 * converterPtr->getPreviewSample(j, i)));
				wavePath.lineTo(x, y);
			}
		}
		g.setColour(getLookAndFeel().findColour(ComboBox::buttonColourId)); // this colourID should always be the base2 color of the current lookandfeel
		g.strokePath(wavePath, PathStrokeType(2.0f));
	}
    
}
//==============================================================================

//==============================================================================
//  Function Repaint
//==============================================================================
void timeDomainWin::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
//==============================================================================

//==============================================================================
//  setTumbnail setter ofr Audio thumbnail
//==============================================================================
void timeDomainWin::setConverter(Converter* _conPtr)
{
	// This method is where you should set the bounds of any child
	// components that your component contains..
	converterPtr = _conPtr;

}
void timeDomainWin::setCurrentProfile(int * _currentProfile)
{
	currentProfile = _currentProfile;
}
//==============================================================================