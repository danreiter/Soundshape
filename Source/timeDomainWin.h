/*
  ==============================================================================

    timeDomainWin.h
    Created: 28 Dec 2018 11:49:15am
    Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"
#include "laf.h"

//==============================================================================
/**
Renders Soundshape's sound in the time domain as a wave. 
Draws background of the time domain window including scale in seconds, background color, and a different 
background color for the currently selected frequency profile.

*/
class timeDomainWin    : public Component
{
public:
    timeDomainWin();
    ~timeDomainWin();
	/**
	Sets locations and size of of the sound wave in the time domain. Draws the background and scale for the time domain.

	Function is called using the repaint() function.
	@see
	JUCE documentation: Component::paint(), Component::repaint()
	*/
    void paint (Graphics&) override;
	/**
	Repaints the component on resize.

	@see
	JUCE documentation: Component::resized(), Component::repaint()
	*/
    void resized() override;
	/**
	Sets a reference to the back-end's converter for rendering the sond wave.

	@see smallTime::setTimeDomain()
	@see bigTime::setProfile()
	@param _conPtr Reference to the converter on Soundshape's back-end.
	*/
	void setConverter(Converter* _conPtr);
	/**
	Sets a reference to the back-end's converter for rendering the sond wave.

	@see smallTime::setTimeDomain()
	@see bigTime::setProfile()
	@param _currentProfile Reference to the currently selected on frequency profile.
	*/
	void setCurrentProfile(int * _currentProfile);


private:
	int * chunk_size;			///< Reference to the number of frequency spikes in a frequency profile.
	int* currentProfile;		///< Reference to the index of the currently selected frequency profile.
	Converter * converterPtr;	///< Reference to a Converter to get frequency values from the back-end
    

    // THIS WAS ADDED HERE FOR TESTING AUDIO DRAWING.
//===============================
    //std::unique_ptr<AudioFormatReaderSource> readerSource;
//========================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (timeDomainWin)
};
