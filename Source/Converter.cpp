#include "Converter.h"

Converter::Converter() : envelope(), thumbnailCache(2), inverseTransform(SOUNDSHAPE_FFT_ORDER),
    thumbnail(50, formatManager, thumbnailCache)
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
    profile[chunk][bin] = value;
}

AudioThumbnail & Converter::getThumbnail()
{
    return thumbnail;
}

// Returns a pointer to the specified chunk of the entire profile
float* Converter::getProfile(int chunk){
	float * tempArr = new float[4000];
	for (int i = 0; i < 4000; i++)
	{
		tempArr[i] = profile[chunk][i];
	}
	return &tempArr[0];
}

void Converter::synthesize(int profileChunk, AudioBuffer<float>& buffer, MidiKeyboardState& keyboardState)
{
    // TODO

    // fill the buffer with the transformed and processed data derived from row profileChunk of the profile
    // First make local copy so profile bins used in this method won't change during synthesis
    // copy the appropriate chunk from the converter object's profile matrix
    // Then, duplicate it according to each downed MIDI note.
    float localChunk[2*SOUNDSHAPE_CHUNK_SIZE];
    for(int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++){
        localChunk[i] = profile[profileChunk][i];
    }
    // TODO duplicate according to keyboardState
    inverseTransform.performRealOnlyInverseTransform(localChunk);

    buffer.copyFrom(0,0,localChunk,buffer.getNumSamples());
}


void Converter::UpdateThumbnail()
{
    // TODO
}

