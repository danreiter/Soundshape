#include "Converter.h"

Converter::Converter(AudioProcessorValueTreeState& _valueTreeState) : 
    valueTreeState(_valueTreeState), envelope(),
    profile(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    tempProfile(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    shiftedProfile(2 * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    tempChunk(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    previousDFT(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    previewChunks(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_PREVIEW_CHUNK_SIZE),
    tempRenderbuffer(2*SOUNDSHAPE_CHUNK_SIZE),
    tempRenderProfile(2*SOUNDSHAPE_CHUNK_SIZE),
    noteVelocities({{0}}),
    sustainedNoteVelocities({{0}})
{
    // set up inverse FFT config objects
    // This needs manually freed (use destructor)
    inverseFFT = kiss_fftr_alloc(SOUNDSHAPE_CHUNK_SIZE, 1, NULL, NULL);
    previewInverseFFT = kiss_fftr_alloc(SOUNDSHAPE_CHUNK_SIZE, 1, NULL, NULL);

    envelope.adsrEnvelope.setParameters({ 0.5f, 0.1f, 1.0f, 0.25f }); // DEFUALT VALUES HERE?
}

Converter::~Converter() {
    kiss_fftr_free(inverseFFT);
    kiss_fftr_free(previewInverseFFT);
}

void Converter::synthesize(int profileChunk, AudioBuffer<float>& buffer, MidiKeyboardState& keyState)
{
    // TODO : CHANGE THIS TO INTERPOLATE BETWEEN CHUNKS WHEN IT'S TIME

    // fill the buffer with the transformed and processed data derived from row profileChunk of the profile
    // First make local copy so profile bins used in this method won't change during synthesis
    // copy the appropriate chunk from the converter object's profile matrix
    // Then, duplicate it according to each downed MIDI note.

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
    // apply envelope
    envelope.adsrEnvelope.applyEnvelopeToBuffer(buffer, 0, buffer.getNumSamples());
    // apply gain
    buffer.applyGain(gain);
    if (buffer.getNumChannels() == 2) {
        // stereo, just copy to the other channel
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples()); // copy to Right channel
    }   
}


void Converter::addShiftedProfiles(int chunk)
{
    // *******************
    // TODO : instead of building the shifted profile while synthesizing each chunk, should it be gradually built as
    // each MIDI messaage is received?
    // *******************

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

void Converter::renderPreview(int chunk)
{
    // do an IFFT on the chunk by copying the profile points into
    // a temporary buffer, do the IFFT into tempRenderBuffer, then copy
    // that output back into previewChunks

    for (int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++) {
        tempRenderProfile[i] = getProfileRawPoint(chunk, i);
    }
    kiss_fftri(previewInverseFFT, tempRenderProfile.data(), tempRenderbuffer.data());
    // write some samples to the structure that the GUI can use for drawing
    for (int i = 0; i < SOUNDSHAPE_PREVIEW_CHUNK_SIZE; i++) {
        size_t renderIndex = chunk * SOUNDSHAPE_PREVIEW_CHUNK_SIZE + i;
        const int crossfadeSize = 10;
        previewChunks[renderIndex] = tempRenderbuffer[i] / SOUNDSHAPE_CHUNK_SIZE;
        if (chunk > 0) {
            float thisRowWeight = ((float)i) / crossfadeSize;
            thisRowWeight = thisRowWeight < 1.0f ? thisRowWeight : 1.0f;
            size_t lastRowIndex = chunk * SOUNDSHAPE_PREVIEW_CHUNK_SIZE - 1;
            previewChunks[renderIndex] =  (thisRowWeight * previewChunks[renderIndex]) + ((1.0f - thisRowWeight) * previewChunks[lastRowIndex]);
        }
    }
    
}

float Converter::getPreviewSample(int chunk, int index)
{
    return previewChunks[chunk * SOUNDSHAPE_PREVIEW_CHUNK_SIZE + index];
}


void Converter::handleNoteOn(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{

    // Only enter the noteOn phase of the envelope if there are no notes already playing or being sustained
    bool isTimeForAttack = true;
    for (int i = 0; i < 128; i++) {
        if (noteVelocities[i] != 0 || sustainedNoteVelocities[i] != 0) {
            isTimeForAttack = false;
        }
    }

    // Possibly change this in the future to only work for user-specified MIDI channels?
    noteVelocities[midiNoteNumber] = velocity;

    if (isTimeForAttack) {
        envelope.adsrEnvelope.noteOn();
    }

}

void Converter::handleNoteOff(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{
    if (sustainPressed == true) {
        sustainedNoteVelocities[midiNoteNumber] = noteVelocities[midiNoteNumber];
    }
    noteVelocities[midiNoteNumber] = 0;
    // if no notes being sustained or currently pressed, go into release phase
    bool isTimeForRelease = true;
    for (int i = 0; i < 128; i++) {
        if (sustainedNoteVelocities[i] != 0 || noteVelocities[i] != 0) {
            isTimeForRelease = false;
        }
    }
    if (isTimeForRelease) {
        envelope.adsrEnvelope.noteOff();
    }
}

void Converter::setSustain(bool sustainState)
{
    sustainPressed = sustainState;
    if (sustainState == false) {
        // turn off all notes that arent currently pressed,
        // and check if any notes are still pressed.
        bool isTimeForRelease = true;
        for (int i = 0; i < 128; i++) {
            sustainedNoteVelocities[i] = 0.0f;
            if (noteVelocities[i] != 0) {
                isTimeForRelease = false;
            }
        }
        if (isTimeForRelease) {
            envelope.adsrEnvelope.noteOff();
        }
    }
}


void Converter::setChunkRange(int beginning, int end)
{
    beginningChunk = beginning;
    endingChunk = end;
}

int Converter::freqToBin(int freq, double rate) {
    double ratio = (double)(SOUNDSHAPE_CHUNK_SIZE * freq) / rate;
    int result = (int)std::round(ratio);
    return result;
}

float Converter::binToFreq(int bin, double rate) {
    return rate * bin / SOUNDSHAPE_CHUNK_SIZE;
}



kiss_fft_cpx Converter::getProfileRawPoint(int chunk, int i) {
    size_t index = chunk * SOUNDSHAPE_CHUNK_SIZE + i;
    return profile[index];
}



void Converter::setProfileRawPoint(int chunk, int i, float value) {
    // set real part only; assume 0 phase
    profile[chunk * SOUNDSHAPE_CHUNK_SIZE + i].r = value;
}


EnvelopeParams & Converter::getEnvelope()
{
    return envelope;
}

void Converter::parameterChanged(const String & parameterID, float newValue)
{
    if (parameterID == "gain") {
        gain = newValue;
    }
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

    for (int chunk = 0; chunk < SOUNDSHAPE_PROFILE_ROWS; chunk++) {
        for (int sourceBin = 0; sourceBin < SOUNDSHAPE_CHUNK_SIZE; sourceBin++) {
            // recopy frequency spikes from the old profile, putting them in the correct bins (by using the old sample rate)
            size_t sourceRawIndex = chunk * SOUNDSHAPE_CHUNK_SIZE + sourceBin;
            int destinationBin = freqToBin(binToFreq(sourceBin, oldSampleRate), sampleRate);
            size_t destinationRawIndex = chunk * SOUNDSHAPE_CHUNK_SIZE + destinationBin;
            if (destinationBin < SOUNDSHAPE_CHUNK_SIZE / 2) {
                profile[destinationRawIndex] = tempProfile[sourceRawIndex];
            }

            // cleanup temporary structure for future use
            tempProfile[sourceRawIndex] = { 0,0 };
        }
    }

    // we also need to inform the envelope
    envelope.adsrEnvelope.setSampleRate(_sampleRate);

}

double Converter::getSampleRate()
{
    return sampleRate;
}


// Internally, the 2d array is just a 1D vector, so we do the proper
// conversion to find the right spot in the array that represents 
// the frequency freq
void Converter::updateFrequencyValue(int chunk, int freq, float value)
{
    //DBG(""<<chunk<<" "<<freq<<" "<<value);

    if (freq > sampleRate / 2 - 1) {
        return; // do nothing
    }
    setProfileRawPoint(chunk, freqToBin(freq,sampleRate), value);
}



float Converter::getFrequencyValue(int chunk, int freq) {
    return getProfileRawPoint(chunk, freqToBin(freq,sampleRate)).r;
}