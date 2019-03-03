/*
  ==============================================================================

    envelope.h
    Created: 24 Dec 2018 1:00:17pm
    Author:  danre

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define ENVELOPE_ATTACK 7002
#define ENVELOPE_DECAY 7003
#define ENVELOPE_SUSTAIN 7004
#define ENVELOPE_RELEASE 7005

//==============================================================================
/*
*/
class envelope    : public Component
{
public:
    envelope();
    ~envelope();

    void paint (Graphics&) override;
    void resized() override;

private:
	Slider * Attack, *Decay, *Sustain, *Release;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (envelope)
};
