#include "Converter.h"

Converter::Converter() : envelope(), thumbnailCache(2), thumbnail(50, formatManager, thumbnailCache)
{
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

AudioThumbnail & Converter::getThumbnail()
{
    return thumbnail;
}

void Converter::synthesize(int profileChunk, AudioBuffer<float>& buffer)
{
    // TODO

    // fill the buffer with the transformed and processed data derived from row profileChunk of the profile
}

void Converter::UpdateThumbnail()
{
    // TODO
}

