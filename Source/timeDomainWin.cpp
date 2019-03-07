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
    // temporarily load file here for testing. Needs to be moved to code for a Load File button

	//// Code for loading a sound
    //formatManager.registerBasicFormats();
    //FileChooser chooser("Select a Wave file to play...",
    //    {},
    //    "*");
    //if (chooser.browseForFileToOpen())
    //{
    //    File file(chooser.getResult());
    //    auto* reader = formatManager.createReaderFor(file);
    //    if (reader != nullptr)
    //    {
    //        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
    //        thumbnail.setSource(new FileInputSource(file));
    //        readerSource.reset(newSource.release());
    //    }
    //}
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
	g.drawLine(centerLine, 2.0f);

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
		g.drawLine(l);
		g.drawText(String(i), rec1, Justification::centredLeft);
		rec.removeFromLeft(tick);
		rec1.removeFromLeft(tick);
	}

    // draw thumbnail
    // TODO replace this once the backend is set up properly (the Converter has a thumbnail object)
    //thumbnail.drawChannels(g, waveRect, 0, thumbnail.getTotalLength(), 2.5f);

    g.setColour(Colours::mediumpurple);
    g.setFont(3.0f);
    
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
