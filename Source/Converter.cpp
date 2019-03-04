#include "Converter.h"

Converter::Converter() : envelope(), thumbnailCache(2), thumbnail(50, formatManager, thumbnailCache),
    profile(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE, 0),
    localChunk(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    tempChunk(2 * SOUNDSHAPE_CHUNK_SIZE),
    previousDFT(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    inverseTransform(SOUNDSHAPE_FFT_ORDER)
{
    for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE; i++) {
        //DBG(profile[i]);
    }
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


// The sample rate is set when the plugin becomes ready-to-play
void Converter::setSampleRate(double _sampleRate)
{
    sampleRate = _sampleRate;
}


// Internally, the 2d array is just a 1D vector, so we do the proper
// conversion to find the right spot in the array that represents 
// the frequency freq
void Converter::updateFrequencyValue(int chunk, int freq, float value)
{
    setProfileRawPoint(chunk, freqToBin(freq), value);
}



float Converter::getFrequencyValue(int chunk, int freq) {

    return getProfileRawPoint(chunk, freqToBin(freq));
}



AudioThumbnail & Converter::getThumbnail()
{
    return thumbnail;
}



void Converter::synthesize(int profileChunk, AudioBuffer<float>& buffer, MidiKeyboardState& keyState)
{
    // TODO : CHANGE THIS TO INTERPOLATE BETWEEN CHUNKS WHEN IT'S TIME

    // fill the buffer with the transformed and processed data derived from row profileChunk of the profile
    // First make local copy so profile bins used in this method won't change during synthesis
    // copy the appropriate chunk from the converter object's profile matrix
    // Then, duplicate it according to each downed MIDI note.

    for (int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++) {
        localChunk[i] = getProfileRawPoint(profileChunk, i);
    }
    // TODO duplicate according to keyboardState
    inverseTransform.performRealOnlyInverseTransform(localChunk.data());
    //for (int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++) {
        //DBG(localChunk[i]);
    //}
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        // copy the current DFT into the temporary chunk
        tempChunk[i] = localChunk[currentIndex];
        currentIndex += 1;
        if (currentIndex == SOUNDSHAPE_CHUNK_SIZE) {
            currentIndex = 0;
        }
    }

    buffer.copyFrom(0,0, tempChunk.data(), buffer.getNumSamples());
    buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
}



void Converter::UpdateThumbnail()
{
    // TODO
}



int Converter::freqToBin(int freq) {
    return (int)(2 * SOUNDSHAPE_CHUNK_SIZE * freq / sampleRate);
}



float Converter::getProfileRawPoint(int chunk, int i) {
    return profile[chunk * SOUNDSHAPE_CHUNK_SIZE + i];
}



void Converter::setProfileRawPoint(int chunk, int i, float value) {
    profile[chunk * SOUNDSHAPE_CHUNK_SIZE + i] = value;
}