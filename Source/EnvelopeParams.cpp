#include "EnvelopeParams.h"

EnvelopeParams::EnvelopeParams() {

}
EnvelopeParams::~EnvelopeParams() {

}


AudioParameterInt *EnvelopeParams::setAttackParamPtr(AudioParameterInt *atk) {
    attack = atk;
    return atk;
}
// TODO define rest of the setters like this