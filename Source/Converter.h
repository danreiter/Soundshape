#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeParams.h"

// A DFT of size 2**16 gives us 1Hz precision. Output is real-valued so we only need 2**15 samples per row
// 2**15 is 32768.
// We decided to give a maximum of 50 chunks.
#define SOUNDSHAPE_FFT_ORDER 15
#define SOUNDSHAPE_CHUNK_SIZE (1 << SOUNDSHAPE_FFT_ORDER)
#define SOUNDSHAPE_PROFILE_ROWS 50

// Stores parameters like the envelope and performs common tasks for time domain <-> frequency domain transformations.
class Converter
{
public:
    Converter();
    ~Converter();
    EnvelopeParams& getEnvelope();
    AudioParameterFloat *getGain();
    void updateProfileBin(int, int, float);
    AudioThumbnail& getThumbnail();
    float getProfileBin(int chunk, int bin);

    void synthesize(int currentChunk, AudioBuffer<float>& buffer, MidiKeyboardState& keyState);


private:
    // The frequency domain profile of the current preset.
    // Each row is the spectrum of the desired sound for a single note at the specified time.
    float profile[SOUNDSHAPE_PROFILE_ROWS][SOUNDSHAPE_CHUNK_SIZE];

    // temporary storage of last rendered DFT for crossfading
    float previousDFT[2 * SOUNDSHAPE_CHUNK_SIZE];

    AudioThumbnail thumbnail;
    AudioThumbnailCache thumbnailCache;

    int maxNumChunks = 50;
    int samplesPerChunk = 32768;
    float referenceFrequency = 440.0f; // Hz. This is what the lowest spike is assumed to represent.

    // the converter logically stores the audio parameters for better abstraction,
    // but these actually need to be setup in the audio processor with addParameter().
    AudioParameterFloat *gain;
    // the envelope contains the pointers to the ADSR values and the envelope size (which is not and audio parameter)
    EnvelopeParams envelope;

    void UpdateThumbnail();

    AudioFormatManager formatManager;

    dsp::FFT inverseTransform;

};