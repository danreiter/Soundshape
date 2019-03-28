#include "EnvelopeParams.h"

EnvelopeParams::EnvelopeParams() {
}
EnvelopeParams::~EnvelopeParams() {

}

void EnvelopeParams::parameterChanged(const String & parameterID, float newValue)
{
    ADSR::Parameters newParams = adsrEnvelope.getParameters();
    if (parameterID == "attack") {
        newParams.attack = newValue;
    }
    if (parameterID == "decay") {
        newParams.decay = newValue;
    }
    if (parameterID == "sustain") {
        newParams.sustain = newValue;
    }
    if (parameterID == "release") {
        newParams.release = newValue;
    }
    adsrEnvelope.setParameters(newParams);
}
