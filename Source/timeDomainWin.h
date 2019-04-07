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
	void setConverter(Converter* _conPtr);
	void setCurrentProfile(int * _currentProfile);


private:
	int * chunk_size;
	int* currentProfile;
	Converter * converterPtr;
    

    // THIS WAS ADDED HERE FOR TESTING AUDIO DRAWING.
//===============================
    std::unique_ptr<AudioFormatReaderSource> readerSource;
//========================


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (timeDomainWin)
};
