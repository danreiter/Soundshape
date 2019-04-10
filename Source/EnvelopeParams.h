#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

/**
* EnvelopeParams handles updating the envelope when AudioParameters change.
* In the Converter, there is one of these objects stored for 
* each note on a MIDI keyboard. Because of the way the Converter works,
* this lets us treat each note as a synthesizer "voice."
*/
class EnvelopeParams : public AudioProcessorValueTreeState::Listener{
public:
    EnvelopeParams();
    ~EnvelopeParams();

    /**
    If this EnvelopeParams object is set up to listen to an AudioProcessorValueTreeState,
    this is called whenevver a parameter is changed. It manges the state of the internal JUCE ADSR
    envelope generator. 
    */
    void parameterChanged(const String &parameterID, float newValue) override;

    /** A JUCE-provided envelope with attack, decay, sustain, and release. */
    ADSR adsrEnvelope;

    /** Tracks if the note associated with this envelope was released but is bein held by the sustain pedal. */
    bool sustained = false;


private:

};
