#include "Converter.h"

Converter::Converter(AudioProcessorValueTreeState& _valueTreeState) :
    valueTreeState(_valueTreeState),
    profile(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    shiftedProfile(2 * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    tempChunk(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    previousDFT(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    previewChunks(SOUNDSHAPE_PROFILE_ROWS * SOUNDSHAPE_PREVIEW_CHUNK_SIZE),
    tempRenderbuffer(2 * SOUNDSHAPE_CHUNK_SIZE, 0),
    tempRenderProfile(2 * SOUNDSHAPE_CHUNK_SIZE, { 0,0 }),
    tempImportExportProfile(2 * SOUNDSHAPE_CHUNK_SIZE, { 0,0 })
{
    // set up inverse FFT config objects
    // This needs manually freed (use destructor)
    inverseFFT = kiss_fftr_alloc(SOUNDSHAPE_CHUNK_SIZE, 1, NULL, NULL); // for real-time use. Manual memory allocation.
    previewInverseFFT = kiss_fftr_alloc(SOUNDSHAPE_CHUNK_SIZE, 1, NULL, NULL);
    exportInverseFFT = kiss_fftr_alloc(SOUNDSHAPE_CHUNK_SIZE, 1, NULL, NULL);

    // set up forward FFT config for importing and analyzing audio
    importFFT = kiss_fftr_alloc(SOUNDSHAPE_CHUNK_SIZE, 0, NULL, NULL);

    //envelope.adsrEnvelope.setParameters({ 0.5f, 0.1f, 1.0f, 0.25f }); // DEFUALT VALUES HERE?
}

Converter::~Converter() {
    // free FFT and iFFT configs
    kiss_fftr_free(inverseFFT);
    kiss_fftr_free(previewInverseFFT);
    kiss_fftr_free(exportInverseFFT);
    kiss_fftr_free(importFFT);
}

void Converter::synthesize(AudioBuffer<float>& buffer)
{
    // fill the buffer with the transformed and processed data derived from row profileChunk of the profile
    // First make local copy so profile bins used in this method won't change during synthesis
    // copy the appropriate chunk from the converter object's profile matrix
    // Then, duplicate it according to each downed MIDI note.

    int numSamples = buffer.getNumSamples();

    kiss_fft_cpx zeroCpx = { 0,0 };
    std::fill(shiftedProfile.begin(), shiftedProfile.end(), zeroCpx); // clear the shifted profile data
    addShiftedProfiles(currentChunk, numSamples); //re-shift

    // perform an inverse FFT on the shifted profile. This scales according to each key's envelope state
    kiss_fftri(inverseFFT, shiftedProfile.data(), previousDFT.data());

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        // copy the current DFT into the temporary chunk
        tempChunk[i] = previousDFT[currentIndex] / SOUNDSHAPE_CHUNK_SIZE; // kissfft inverse real-only needs scaled by fft size
        currentIndex += 1;
        if (currentIndex == SOUNDSHAPE_CHUNK_SIZE) {
            currentIndex = 0;
        }
    }

    buffer.copyFrom(0,0, tempChunk.data(), numSamples); // left channel

    samplesWritten += numSamples;
    if (samplesWritten > SOUNDSHAPE_CHUNK_SIZE) {
        samplesWritten = 0;
        currentChunk += 1;

       if (currentChunk >= endingChunk || currentChunk > SOUNDSHAPE_PROFILE_ROWS) {
            currentChunk = beginningChunk;
       }
    }

    // reset play to beginning of section

    // apply gain
    buffer.applyGain(gain);
    if (buffer.getNumChannels() == 2) {
        // stereo, just copy to the other channel
        buffer.copyFrom(1, 0, buffer, 0, 0, numSamples); // copy to Right channel
    }   
}


void Converter::addShiftedProfiles(int chunk, int numSamples)
{

    // cycles through active notes, adding a shifted profile to a temporary "profile structure" for each.
    for (int i = 0; i < noteStates.size(); i++) {
        if (noteStates[i].adsrEnvelope.isActive()) { // expensive operation, so should only be done when absolutely necessary
            
            // Scale the shifted profile by the current strnength of the note it's being shifted too.
            
            float currentEnvelopeSample = noteStates[i].adsrEnvelope.getNextSample();

            float noteFreq = (float)MidiMessage::getMidiNoteInHertz(i);
            float ratio = noteFreq / referenceFrequency;

            for (int j = 0; j < SOUNDSHAPE_CHUNK_SIZE; j++) {
                if ( getProfileRawPoint(chunk,j).r != 0) { // MOST PROFILE BINS WILL BE ZERO, SO THIS CAN BE OPTIMIZED
                    float targetFreq = j* ratio;
                    if (targetFreq < sampleRate / 2) { // avoid aliasing
                        int bin = freqToBin(targetFreq, sampleRate);
                        shiftedProfile[bin].r += getProfileRawPoint(chunk, j).r * currentEnvelopeSample;
                    }
                }
            }

            // since we could only use 1 sample of this envelope, fast forward
            for (int sample = 0; sample < numSamples - 1; sample++) {
                noteStates[i].adsrEnvelope.getNextSample();
            }

        }
    }
}

// The sample rate is set when the plugin becomes ready-to-play
void Converter::setSampleRate(double _sampleRate)
{
    // we also need to inform the envelopes for each note
    for (int i = 0; i < 128; i++) {
        noteStates[i].adsrEnvelope.setSampleRate(_sampleRate);
    }
    exportRenderEnvelope.adsrEnvelope.setSampleRate(_sampleRate);
    //double oldSampleRate = sampleRate;
    sampleRate = _sampleRate; // new value for sample rate
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

void Converter::renderExportChunkToBuffer(int chunk, AudioBuffer<float> &buffer, double exportSampleRate) {
    if(chunk == 0){
        exportRenderEnvelope.adsrEnvelope.reset();
        exportRenderEnvelope.adsrEnvelope.noteOn();
    }
    kiss_fft_cpx zeroCpx = { 0,0 };
    std::fill(tempImportExportProfile.begin(), tempImportExportProfile.end(), zeroCpx);
    for (int i = 0; i < SOUNDSHAPE_CHUNK_SIZE; i++){
        int targetBin = freqToBin(i,exportSampleRate);
        if( binToFreq(i,sampleRate) < exportSampleRate / 2) {
            tempImportExportProfile[targetBin] = getProfileRawPoint(chunk, i);
        }
    }
    kiss_fftri(exportInverseFFT, tempImportExportProfile.data(),buffer.getWritePointer(0));
    buffer.applyGain(0,0,buffer.getNumSamples(), 1.0f/tempImportExportProfile.size());
    exportRenderEnvelope.adsrEnvelope.applyEnvelopeToBuffer(buffer,0,buffer.getNumSamples());
}

void Converter::analyzeChunkIntoProfile(int chunk, AudioBuffer<float> &buffer, double importSampleRate) {
    // the buffer represents one chunk at a specified sampling rate
    // First, perform an FFT to convert to the frequency domain.
    int numSamples = buffer.getNumSamples();
    kiss_fft_cpx zeroCpx = {0,0};
    std::fill(tempImportExportProfile.begin(), tempImportExportProfile.end(), zeroCpx);
    // apply windowing function because of leakage
    dsp::WindowingFunction<float> window(buffer.getNumSamples(), dsp::WindowingFunction<float>::hamming);
    window.multiplyWithWindowingTable(buffer.getWritePointer(0),buffer.getNumSamples());
    kiss_fftr(importFFT, buffer.getReadPointer(0),tempImportExportProfile.data()); // channel 0

    clearChunk(chunk); // clear this part of the profile

    // fill up the profile with the magnitude of each frequency spike
    for(int i = 2; i < std::min((double)4000, importSampleRate); i+=2){
        // Iterate through each point in the profile we want to fill.
        // For each one, find the bin of the input FFT buffer that
        // corresponds to this profile frequency point.
        
        int sourceBin1 = freqToBin(i, importSampleRate);
        int sourceBin2 = freqToBin(i + 1, importSampleRate);


        // magnitude is length of vector to the complex point from the origin
        auto magnitude =
            (float)sqrt(
                pow(tempImportExportProfile[sourceBin1].r, 2) +
                pow(tempImportExportProfile[sourceBin1].i, 2)) +
            (float)sqrt(
                pow(tempImportExportProfile[sourceBin2].r, 2) +
                pow(tempImportExportProfile[sourceBin2].i, 2)
        );

        if( magnitude > 5){
            setProfileRawPoint(chunk, i, magnitude);
        }
        
    }
}

void Converter::clearChunk(int chunk){
    kiss_fft_cpx zeroCpx{0,0};
    for(int i =0; i < SOUNDSHAPE_CHUNK_SIZE; i++){
        profile[SOUNDSHAPE_CHUNK_SIZE * chunk + i] = zeroCpx;
    }
}

float Converter::getPreviewSample(int chunk, int index)
{
    return previewChunks[chunk * SOUNDSHAPE_PREVIEW_CHUNK_SIZE + index];
}


void Converter::handleNoteOn(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{

    // When a note is played on the keyboard, we need to enter the attack state of
    // that note's associated ADSR envelope.

    noteStates[midiNoteNumber].adsrEnvelope.noteOn();
    noteStates[midiNoteNumber].sustained = false;

}

void Converter::handleNoteOff(MidiKeyboardState * source, int midiChannel, int midiNoteNumber, float velocity)
{
    if (sustainPressed == false) {
        noteStates[midiNoteNumber].adsrEnvelope.noteOff();
    }
    else {
        noteStates[midiNoteNumber].sustained = true;
    }
}

void Converter::setSustain(bool sustainState)
{
    sustainPressed = sustainState;
    if (sustainState == false) {
        // turn off all notes that aren't currently pressed,
        // and check if any notes are still pressed.
        for (int i = 0; i < 128; i++) {
            if (noteStates[i].sustained == true) {
                noteStates[i].sustained = false;
                noteStates[i].adsrEnvelope.noteOff();
            }
        }
    }
}

void Converter::envelopeListenTo(String paramName, AudioProcessorValueTreeState & valueTreeState)
{
    // make each key's envelope listen to parameter changes.
    for (int i = 0; i < 128; i++) {
        valueTreeState.addParameterListener(paramName, &noteStates[i]);
    }
    // make our extra envelope for rendering also listen
    valueTreeState.addParameterListener(paramName, &exportRenderEnvelope);

}

void Converter::panic()
{
    sustainPressed = false;
    for (int i = 0; i < 128; i++) {
        noteStates[i].adsrEnvelope.noteOff();
    }
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



void Converter::parameterChanged(const String & parameterID, float newValue)
{
    if (parameterID == "gain") {
        gain = newValue;
    }
    if (parameterID == "beginningChunk") {
        beginningChunk = (int)newValue;
        currentChunk = beginningChunk;
    }
    if (parameterID == "endingChunk") {
        endingChunk = (int)newValue;
    }
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

    // OLD : setProfileRawPoint(chunk, freqToBin(freq,sampleRate), value);
    setProfileRawPoint(chunk, freq, value);
}

float Converter::getFrequencyValue(int chunk, int freq) {
    // OLD: return getProfileRawPoint(chunk, freqToBin(freq,sampleRate)).r;
    return getProfileRawPoint(chunk, freq).r;
}

