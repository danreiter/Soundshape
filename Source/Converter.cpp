#include "Converter.h"

Converter::Converter() : envelope(), thumbnailCache(2), thumbnail(50, formatManager, thumbnailCache),
    inverseTransform(SOUNDSHAPE_FFT_ORDER)
{
    for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
        for (int j = 0; j < SOUNDSHAPE_CHUNK_SIZE; j++) {
            profile[i][j] = 0.0f;
        }
    }
    // for testing purposes ONLY.
    // make a simple profile (just the first chunk) for a simple sound.
    // *******************
    profile[0][(int)referenceFrequency] = 500.0f;
    profile[0][(int)(2 * referenceFrequency)] = 300.0f;
    profile[0][(int)(4 * referenceFrequency)] = 200.0f;
    profile[0][(int)(6 * referenceFrequency)] = 100.0f;
    // *******************
}

Converter::~Converter() {
}

EnvelopeParams & Converter::getEnvelope()
{
    return envelope;
}

AudioParameterFloat * Converter::getGain()
{
    return gain;
}

void Converter::updateProfileBin(int chunk, int bin, float value)
{
    // TODO
}

float Converter::getProfileBin(int chunk, int bin) {
    return profile[chunk][bin];
}

AudioThumbnail & Converter::getThumbnail()
{
    return thumbnail;
}

void Converter::synthesize(int profileChunk, AudioBuffer<float>& buffer, MidiKeyboardState& keyState)
{
    // TODO

    // fill the buffer with the transformed and processed data derived from row profileChunk of the profile
    // First make local copy so profile bins used in this method won't change during synthesis
    // copy the appropriate chunk from the converter object's profile matrix
    // Then, duplicate it according to each downed MIDI note.
    float localChunk[2 * SOUNDSHAPE_CHUNK_SIZE] = { 0.0f };
    float tempChunk[2 * SOUNDSHAPE_CHUNK_SIZE] = { 0.0f }; // so we don't overwrite previousDFT before it's used
    for (int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++) {
        localChunk[i] = profile[profileChunk][i];
    }
    // TODO duplicate according to keyboardState
    inverseTransform.performRealOnlyInverseTransform(localChunk);

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        // copy the current DFT into the temporary chunk
        tempChunk[i] = localChunk[currentIndex];
        currentIndex += 1;
        if (currentIndex == SOUNDSHAPE_CHUNK_SIZE) {
            currentIndex = 0;
        }
    }

    buffer.copyFrom(0,0, tempChunk, buffer.getNumSamples());
    buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
}

void Converter::UpdateThumbnail()
{
    // TODO
}

