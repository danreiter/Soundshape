/*
  ==============================================================================

    envelope.h
    Created: 24 Dec 2018 1:00:17pm
    Author:  Daniel Reiter
	Description: Component contains slider for the user to manipulate the envelope settings

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "laf.h"

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
    envelope(AudioProcessorValueTreeState& _valueTreeState);
    ~envelope();

    void paint (Graphics&) override;
    void resized() override;

	void setListener(Slider::Listener *_listener);

private:
	Slider * Attack, *Decay, *Sustain, *Release;  // Sliders for the ADSR Envelope

    // attachments to the back-end parameters
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    AudioProcessorValueTreeState& valueTreeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (envelope)
};
