#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/**
* EnvelopeParams
* Handles updating the envelope when AudioParameters change
*/
class EnvelopeParams : public AudioProcessorValueTreeState::Listener{
public:
    EnvelopeParams();
    ~EnvelopeParams();

    void parameterChanged(const String &parameterID, float newValue) override;

    ADSR adsrEnvelope;

private:

};
