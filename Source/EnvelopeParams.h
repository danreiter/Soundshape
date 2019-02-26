#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class EnvelopeParams {
public:
    EnvelopeParams();
    ~EnvelopeParams();

    // setters
    // These actually return the pointer thats passed into them because of how registering params has to work
    AudioParameterInt *setAttackParamPtr(AudioParameterInt *);
    AudioParameterInt *setDecayParamPtr(AudioParameterInt *);
    AudioParameterInt *setSustainParamPtr(AudioParameterInt *);
    AudioParameterInt *setReleaseParamPtr(AudioParameterInt *);

    // getters
    // TODO

private:
    // pointers to ADSR values.
    // These need to be registered as parameters in the audio processor with addParameter().
    // values have units of samples.
    AudioParameterInt *attack;
    AudioParameterInt *decay;
    AudioParameterInt *sustain;
    AudioParameterInt *release;

};
