#include "EnvelopeParams.h"

EnvelopeParams::EnvelopeParams() {
    adsrEnvelope.setSampleRate(44100.0f); // gets reset by host, just needed for constructions
    adsrEnvelope.setParameters({ 0.5f, 0.1f, 1.0f, 0.25f });
    return;
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
