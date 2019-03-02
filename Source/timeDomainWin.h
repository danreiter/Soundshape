/*
  ==============================================================================

    timeDomainWin.h
    Created: 28 Dec 2018 11:49:15am
    Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class timeDomainWin    : public Component
{
public:
    timeDomainWin();
    ~timeDomainWin();

    void paint (Graphics&) override;
    void resized() override;

private:
	float ** soundWave;

    // THIS WAS ADDED HERE FOR TESTING AUDIO DRAWING.
//===============================
    std::unique_ptr<AudioFormatReaderSource> readerSource;
//========================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (timeDomainWin)
};
