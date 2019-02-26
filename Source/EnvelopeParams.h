#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class EnvelopeParams {
public:
    EnvelopeParams();
    ~EnvelopeParams();

    // setters
    void setAttackParamPtr(AudioParameterFloat *);
    void setDecayParamPtr(AudioParameterFloat *);
    void setSustainParamPtr(AudioParameterFloat *);
    void setReleaseParamPtr(AudioParameterFloat *);

    // getters

private:
    // pointers to ADSR values.
    // These need to be registered as parameters in the audio processor with addParameter().
    // values have units of samples.
    AudioParameterFloat *attack;
    AudioParameterFloat *decay;
    AudioParameterFloat *sustain;
    AudioParameterFloat *release;
};
