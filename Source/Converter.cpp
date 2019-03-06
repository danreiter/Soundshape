#include "Converter.h"

Converter::Converter() : envelope(), thumbnailCache(2), thumbnail(50, formatManager, thumbnailCache),
    profile(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    shiftedProfile(2 * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    tempChunk(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    previousDFT(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    noteVelocities({{0}})
{

    // set up inverse FFT config object
    // This needs manually freed (use destructor)
    inverseFFT = kiss_fftr_alloc(SOUNDSHAPE_CHUNK_SIZE, 1, NULL, NULL);

    for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE; i++) {
        //DBG(profile[i]);
    }
}

Converter::~Converter() {
    kiss_fftr_free(inverseFFT);
}


void Converter::synthesize(int profileChunk, AudioBuffer<float>& buffer, MidiKeyboardState& keyState)
{
    // TODO : CHANGE THIS TO INTERPOLATE BETWEEN CHUNKS WHEN IT'S TIME

    // fill the buffer with the transformed and processed data derived from row profileChunk of the profile
    // First make local copy so profile bins used in this method won't change during synthesis
    // copy the appropriate chunk from the converter object's profile matrix
    // Then, duplicate it according to each downed MIDI note.

    // clear the current shifted profile data, then shift profile to match pressed keys (writes to shiftedProfile)
    //for (int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++) {
    //    shiftedProfile[i].r = 0.0f;
    //}
    addShiftedProfiles(profileChunk);
    

    //inverseTransform.performRealOnlyInverseTransform(localChunk.data());
    kiss_fftri(inverseFFT, shiftedProfile.data(), previousDFT.data());

    //for (int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++) {
        //DBG(localChunk[i]);
    //}
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        // copy the current DFT into the temporary chunk
        tempChunk[i] = previousDFT[currentIndex] / SOUNDSHAPE_CHUNK_SIZE;
        currentIndex += 1;
        if (currentIndex == SOUNDSHAPE_CHUNK_SIZE) {
            currentIndex = 0;
        }
    }

    buffer.copyFrom(0,0, tempChunk.data(), buffer.getNumSamples());
    buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
}


void Converter::addShiftedProfiles(int chunk)
{
    // cycles through active notes (noteVelocities), adding a shifted profile to a temporary "profile structure" for each.
    for (int i = 0; i < noteVelocities.size(); i++) {
        if (noteVelocities[i] != 0) { // expensive operation, so should only be done when absolutely necessary
            
            // Calculate as a percentage of max value. This can be changed to something more appropriate if needed (exponential?)
            // It probably also needs to be considered how a change in velocity would change the frequency domain
            float velocityScale = noteVelocities[i] / 127; 
            float noteFreq = (float)MidiMessage::getMidiNoteInHertz(i);
            float ratio = noteFreq / referenceFrequency;
            
            for (int j = 0; j < profile.size(); j++) {
                if (profile[j].r != 0) {
                    float targetFreq = binToFreq(j) * ratio;
                    if (targetFreq < sampleRate / 2) { // avoid aliasing
                        int bin = freqToBin(targetFreq * ratio);
                        shiftedProfile[bin].r += getProfileRawPoint(chunk, j).r;
                    }
                }
            }
        }
    }
}

void Converter::handleNoteOn(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{
    // Possibl;y change this in the future to only work for user-specified MIDI channels?
    noteVelocities[midiNoteNumber] = velocity;
}

void Converter::handleNoteOff(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{
    noteVelocities[midiNoteNumber] = 0;
}

void Converter::midiPanic() {
    for (int i = 0; i < 128; i++) {
        noteVelocities[i] = 0.0f;
    }
}


void Converter::UpdateThumbnail()
{
    // TODO
}


int Converter::freqToBin(int freq) {
    return (int)std::round(SOUNDSHAPE_CHUNK_SIZE * freq / sampleRate);
}

float Converter::binToFreq(int bin) {
    return sampleRate * bin / SOUNDSHAPE_CHUNK_SIZE;
}



kiss_fft_cpx Converter::getProfileRawPoint(int chunk, int i) {
    return profile[chunk * SOUNDSHAPE_CHUNK_SIZE + i];
}



void Converter::setProfileRawPoint(int chunk, int i, float value) {
    // set real part only; assume 0 phase
    profile[chunk * SOUNDSHAPE_CHUNK_SIZE + i].r = value;
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

    return getProfileRawPoint(chunk, freqToBin(freq)).r;
}



AudioThumbnail & Converter::getThumbnail()
{
    return thumbnail;
}

