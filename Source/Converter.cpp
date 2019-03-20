#include "Converter.h"

Converter::Converter() : envelope(), thumbnailCache(2), thumbnail(50, formatManager, thumbnailCache),
    profile(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    tempProfile(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    shiftedProfile(2 * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    tempChunk(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    previousDFT(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    noteVelocities({{0}}),
    sustainedNoteVelocities({{0}})
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
    kiss_fft_cpx zeroCpx = { 0,0 };
    std::fill(shiftedProfile.begin(), shiftedProfile.end(), zeroCpx);
    addShiftedProfiles(profileChunk);
    

    // TODO : change this to only be computed when it NEEDS to be.
    kiss_fftri(inverseFFT, shiftedProfile.data(), previousDFT.data());

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        // copy the current DFT into the temporary chunk
        tempChunk[i] = previousDFT[currentIndex] / SOUNDSHAPE_CHUNK_SIZE; // kissfft inverse real-only needs scaled by fft size
        currentIndex += 1;
        if (currentIndex == SOUNDSHAPE_CHUNK_SIZE) {
            currentIndex = 0;
        }
    }

    buffer.copyFrom(0,0, tempChunk.data(), buffer.getNumSamples()); // left channel
    buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples()); // copy to Right channel
}

// *******************
// TODO : instead of building the shifted profile while synthesizing each chunk, should it be gradually built as
// each MIDI messaage is received?
// *******************
void Converter::addShiftedProfiles(int chunk)
{
    // cycles through active notes (noteVelocities), adding a shifted profile to a temporary "profile structure" for each.
    for (int i = 0; i < noteVelocities.size(); i++) {
        if (noteVelocities[i] != 0 || sustainedNoteVelocities[i] != 0) { // expensive operation, so should only be done when absolutely necessary
            
            // Calculate as a percentage of max value. This can be changed to something more appropriate if needed (exponential?)
            // It probably also needs to be considered how a change in velocity would change the frequency domain

            float velocityScale = noteVelocities[i] / 127; // TODO : or get velocity form sustained note
            float noteFreq = (float)MidiMessage::getMidiNoteInHertz(i);
            float ratio = noteFreq / referenceFrequency;
            
            for (int j = 0; j < SOUNDSHAPE_CHUNK_SIZE; j++) {
                if (profile[j].r != 0) { // MOST PROFILE BINS WILL BE ZERO, SO THIS CAN BE OPTIMIZED
                    float targetFreq = binToFreq(j, sampleRate) * ratio;
                    if (targetFreq < sampleRate / 2) { // avoid aliasing
                        int bin = freqToBin(targetFreq, sampleRate);
                        shiftedProfile[bin].r += getProfileRawPoint(chunk, j).r;// *velocityScale;
                    }
                }
            }
        }
    }
}

void Converter::handleNoteOn(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{
    // Possibly change this in the future to only work for user-specified MIDI channels?
    noteVelocities[midiNoteNumber] = velocity;
}

void Converter::handleNoteOff(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{
    if (sustainPressed == true) {
        sustainedNoteVelocities[midiNoteNumber] = noteVelocities[midiNoteNumber];
    }
    noteVelocities[midiNoteNumber] = 0;
}

void Converter::setSustain(bool sustainState)
{
    sustainPressed = sustainState;
    if (sustainState == false) {
        // turn off all notes that arent currently pressed
        for (int i = 0; i < 128; i++) {
            sustainedNoteVelocities[i] = 0.0f;
        }
    }
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


int Converter::freqToBin(int freq, double rate) {
    return (int)std::round(SOUNDSHAPE_CHUNK_SIZE * freq / rate);
}

float Converter::binToFreq(int bin, double rate) {
    return rate * bin / SOUNDSHAPE_CHUNK_SIZE;
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
    double oldSampleRate = sampleRate;
    sampleRate = _sampleRate; // new value for sample rate

    // the internal profile data structure needs to be completely remade when
    // the sample rate changes.
    // The current profile data structure needs to be zero'd out, and
    // replaced with a new version of its old self with spikes' locations shifted.

    for (int i = 0; i < profile.size(); i++) {
        tempProfile[i] = profile[i];
        profile[i] = { 0,0 };
    }
    for (int sourceBin = 0; sourceBin < tempProfile.size(); sourceBin++) {
        // recopy frequency spikes from the old profile, putting them in the correct bins (by using the old sample rate)
        int destinationBin = freqToBin(binToFreq(sourceBin, oldSampleRate), sampleRate);
        if (destinationBin < tempProfile.size() / 2) {
            profile[destinationBin] = tempProfile[sourceBin];
        }

        // cleanup temporary structure for future use
        tempProfile[sourceBin] = { 0,0 };
    }

}


// Internally, the 2d array is just a 1D vector, so we do the proper
// conversion to find the right spot in the array that represents 
// the frequency freq
void Converter::updateFrequencyValue(int chunk, int freq, float value)
{
    DBG(freq);
    // TODO : We probably need to check for index out-of-bounds here
    setProfileRawPoint(chunk, freqToBin(freq,sampleRate), value);
}



float Converter::getFrequencyValue(int chunk, int freq) {

    return getProfileRawPoint(chunk, freqToBin(freq,sampleRate)).r;
}



AudioThumbnail & Converter::getThumbnail()
{
    return thumbnail;
}

