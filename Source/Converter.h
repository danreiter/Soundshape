#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeParams.h"
#include "kissfft/kiss_fftr.h"

// A DFT of size 2**16 gives us 1Hz precision. Output is real-valued so we only need 2**15 samples per row
// 2**15 is 32768.
// We decided to give a maximum of 50 chunks.
#define SOUNDSHAPE_FFT_ORDER 15
#define SOUNDSHAPE_CHUNK_SIZE (1 << SOUNDSHAPE_FFT_ORDER)
#define SOUNDSHAPE_PROFILE_ROWS 50

#define SOUNDSHAPE_PREVIEW_CHUNK_SIZE 128 // how many samples of each rendered chunk to save for drawing

// Stores parameters like the envelope and performs common tasks for time domain <-> frequency domain transformations.
class Converter : public MidiKeyboardStateListener
{
public:
    Converter();
    ~Converter();

    // getters for AudioParameter stuff
    EnvelopeParams& getEnvelope();
    AudioParameterFloat *getGain();

    // getters/setters for managin the profile
    void setSampleRate(double _sampleRate);
    void updateFrequencyValue(int chunk, int freq, float value);
    float getFrequencyValue(int chunk, int bin);

    AudioThumbnail * getThumbnail();

    // Fill the buffer with the processed inverse discrete fourier transform of
    // the data in row currentChunk of the profile, according to which keys are pressed in the
    // key state
    void synthesize(int currentChunk, AudioBuffer<float>& buffer, MidiKeyboardState& keyState);

    void setSustain(bool sustainState);

    void renderPreview(int chunk);
    float getPreviewSample(int chunk, int index);


private:

    int freqToBin(int freq, double rate);
    float binToFreq(int bin, double rate);
    kiss_fft_cpx getProfileRawPoint(int chunk, int i);
    void setProfileRawPoint(int chunk, int i, float value);
    void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity);
    void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity);
    void midiPanic(); // turn off all notes

    // Prepares that buffer for an inverse FFT representing currently pressed keys
    void addShiftedProfiles(int chunk);

    //*===================================
    //* THESE STRUCTURES SHOULD NEVER BE ALLOWED TO RESIZE AS THIS CAN CAUSE 
    //* MEMORY ALLOCATION TO OCCUR DURING THE AUDIO CALLBACK

    // The frequency domain profile of the current preset.
    // Each row is the spectrum of the desired sound for a single note at the specified time.
    // Implemented 1-dimensionally. This class's interface still treats this 2-dimensionally
    // Data is complex
    std::vector<kiss_fft_cpx> profile;

    // Temporary chunks for copying/intermediate work during synthesis.
    // These are twice as long as the actual chunks because the IFFT might need the extra samples to work
    std::vector<kiss_fft_cpx> shiftedProfile;
    std::vector<float> tempChunk;

    // temporary storage of last rendered DFT for crossfading
    std::vector<float> previousDFT;

    // temporary storage for data rearrangement when sample rate changes
    std::vector<kiss_fft_cpx> tempProfile;
    
    //*======================================

    std::vector<float> previewChunks;
    std::vector<float> tempRenderbuffer;

    // stores velocity information for each note, 0 to 127 (128 possible notes)
    // these are the notes that are currently pressed
    std::array<float, 128> noteVelocities;
    // and these are the ones that arent pressed but are being sustained by the pedal
    std::array<float, 128> sustainedNoteVelocities;
    
    AudioThumbnail thumbnail;
    AudioThumbnailCache thumbnailCache;

    float referenceFrequency = 440.0f; // Hz. This is what the lowest spike is assumed to represent.
    double sampleRate;

    // the converter logically stores the audio parameters for better abstraction,
    // but these actually need to be setup in the audio processor with addParameter().
    AudioParameterFloat *gain;
    // the envelope contains the pointers to the ADSR values and the envelope size (which is not and audio parameter)
    EnvelopeParams envelope;

    void UpdateThumbnail();

    AudioFormatManager formatManager;

    // object for inverse FFT during synthesis
    //dsp::FFT inverseTransform;
    kiss_fftr_cfg inverseFFT;

    // TODO : This keeps track of where we are in copying a DFT into the buffer (need to rethink this once we add crosfading)
    int currentIndex = 0;

    bool sustainPressed = false;

};