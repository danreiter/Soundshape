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

/**
Stores parameters like the envelope and performs common tasks for time domain <-> frequency domain transformations.

This class is used for performing DSP tasks such as FFTs (forward and inversed).
It is used by the Soundshape_pluginAudioProcessor to combine its frequency profile data
with information about the current state of all attached MIDI keyboards in order to synthesize
chunks of audio during the audio thread's callback.

This listens for changes in audio parameters and keyboard states. It uses one ADSR per key
to manage making each note on the keyboard have its own voice with individual attack, sustain, 
decay, and release.

Profile data is stored contiguously in memory. When at least one key's ADSR is active,
playback begins. Playback involves going through a range of chunks of the profile,
generating their inverse FFTs based on which keys are being pressed (and those keys' 
current envelope position). Playback stops once all 
ADSR envelopes become idle, and will later resume at the beginning of the range the next time it begins.

*/
class Converter : public MidiKeyboardStateListener, public AudioProcessorValueTreeState::Listener
{
public:
    Converter(AudioProcessorValueTreeState& _valueTreeState);
    ~Converter();


    void parameterChanged(const String &parameterID, float newValue) override;

    /**
    Update the sample rate that audio playback is at.
    <b> This MUST be called when the host changes its sampling rate or else output will be out-of-tune. </b>
    */
    void setSampleRate(double _sampleRate);

    double getSampleRate();

    /**
    Sets the frequency profile bin of the specified chunk corresponding to the specified frequency to a value.
    @param chunk The first index of the 2D profile array; which section of the profile you want to update.
    @param freq The frequency in Hz that you want to update.
    @param value the float value to change it to.
    */
    void updateFrequencyValue(int chunk, int freq, float value);

    /**
     * Sets all bins in this profile chunk to 0
     *
     * @param chunk The index of the chunk to clear.
     */
     void clearChunk(int chunk);

    /**
    Get the value of a bin in a profile chunk.
    @return the value of the bin.
    */
    float getFrequencyValue(int chunk, int bin);

    /** Fill the buffer with the processed inverse discrete fourier transform of
    the data in row currentChunk of the profile, according to which keys are pressed in the
    key state.
    @param buffer the AudioBuffer with channels to fill. If it has 2 channels, then the first is copied into the others.
    */
    void synthesize(AudioBuffer<float>& buffer);

    /** 
    Tell the converter of the sustain pedal is held down or not.
    */
    void setSustain(bool sustainState);

    /**
     * Draws the IFFT of one chunk of profile data into its buffers.
     *
     * Useful for drawing the beginning of waveforms in a GUI.
     * @param chunk The row in the data structure to update.
     */
    void renderPreview(int chunk);

    /**
     * Get a sample in the preview rendered with renderPreview.
     *
     * @param chunk The row of profile data.
     * @param index Index into this row.
     * @return The specified sample.
     */
    float getPreviewSample(int chunk, int index);

    /**
     * Writes the IFFT of a profile chunk to an AudioBuffer.
     *
     * Applies the Attack, Decay, and Sustain regions of the ADSR envelope.
     *
     * @param chunk Which part of the profile to render.
     * @param buffer A buffer of floats to fill. Should only have 1 channel (channel 0).
     * @param exportSampleRate The sample rate of the data being exported.
     */
    void renderExportChunkToBuffer(int chunk, AudioBuffer<float>& buffer, double exportSampleRate);


    /**
     * Updates the profile to the spectral content of an audio buffer.
     *
     * Perform an FFT on the input buffer and fill every odd-numbered frequency spike
     * in the profile (up to 4000 Hz because this is what we set in the GUI; this can be changed in
     * the implementation). Tries to shift the FFT data so that its strongest spike is at 440 Hz.
     *
     * @param chunk Which chunk to replace in the profile.
     * @param buffer The input audio data.
     * @param importSampleRate Sample rate in Hz of the data in the buffer.
     */
    void analyzeChunkIntoProfile(int chunk, AudioBuffer<float>& buffer, double importSampleRate);

    /**
    Sets up all the envelopes ( one per MIDI key) to listen to a certain envelope parameter in the AudioProcessorValueTreeState
    */
    void envelopeListenTo(String paramName, AudioProcessorValueTreeState& valueTreeState);

    /**
    Moves the sustain pedal up and makes all notes' envelopes be reset. This is used to cut off ALL current noise.
    */
    void panic();

private:

    int freqToBin(int freq, double rate);
    float binToFreq(int bin, double rate);
    kiss_fft_cpx getProfileRawPoint(int chunk, int i);
    void setProfileRawPoint(int chunk, int i, float value);
    void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

    /** 
    Prepares the shiftedProfiles buffer for an inverse FFT representing currently pressed keys.
    
    This tries to scale each shifted profile by an envelope sample of the key, so those envelopes need to know
    how many samples this is going to make.
    **/
    void addShiftedProfiles(int chunk, int numSamples, float gain);

    AudioProcessorValueTreeState& valueTreeState;
    // updated whenever the actual audioparameter changes
    float gain = 0.99;

    //*===================================
    //* 

    /**
    The frequency domain profile of the current preset.
    Each row is the spectrum of the desired sound for a single note at the specified time.
    Implemented 1-dimensionally. This class's accessors still treats this 2-dimensionally.
    Data is complex.
    <b> THIS STRUCTURES SHOULD NEVER BE ALLOWED TO RESIZE AS THIS CAN CAUSE
    MEMORY ALLOCATION TO OCCUR DURING THE AUDIO CALLBACK. </b>
    */
    std::vector<kiss_fft_cpx> profile;

    /**
    Temporary chunk for copying/intermediate work during synthesis.
    Twice as long as the actual chunks because the IFFT might need the extra samples to work
    <b> THIS STRUCTURES SHOULD NEVER BE ALLOWED TO RESIZE AS THIS CAN CAUSE
    MEMORY ALLOCATION TO OCCUR DURING THE AUDIO CALLBACK. </b>
    */
    std::vector<kiss_fft_cpx> shiftedProfile;

    /**
    Temporary chunk for copying/intermediate work during synthesis.
    Twice as long as the actual chunks because the IFFT might need the extra samples to work
    <b> THIS STRUCTURES SHOULD NEVER BE ALLOWED TO RESIZE AS THIS CAN CAUSE
    MEMORY ALLOCATION TO OCCUR DURING THE AUDIO CALLBACK. </b>
    */
    std::vector<float> tempChunk;

    /**
    Temporary storage of last rendered DFT for crossfading.
    */
    std::vector<float> previousDFT;
    
    //*======================================

    /** Stores previews of synthesized data. This can be used to draw parts of the wave in the GUI.
    It's size is determined in a SOUNDSHAPE_PREVIEW_CHUNK_SIZE. */
    std::vector<float> previewChunks;

    /** Stores the IFFT of profiles when rendering previews for the GUI */
    std::vector<float> tempRenderbuffer;

    /** temporary data for the profile before its rendered for previews in the GUI */
    std::vector<kiss_fft_cpx> tempRenderProfile;

    /** Stores an ADSR envelope for each note. In Converter::addShiftedProfiles, this lets
    us giuve each note on a keyboard its own "voice."
    */
    std::array<EnvelopeParams, 128> noteStates;

    /** An extra envelope for rendering exports.
     * Gets reset whenever a exporting starts happening at chunk 0.
     */
     EnvelopeParams exportRenderEnvelope;

     /** data storage for the profile before its rendered for exporting IFFT data
      * or for an FFT of imported data before its shifted and written to the profile*/
     std::vector<kiss_fft_cpx> tempImportExportProfile;



    float referenceFrequency = 440.0f; // Hz. This is what the lowest spike is assumed to represent.
    float referenceSampleRate = 44100.0f; // Hz. This is what the profile assumes it was derived from. Converts to actual sampler rate during synthesis
    double sampleRate;

    int currentChunk = 0;

    AudioFormatManager formatManager;

    /** COnfiguration object for inverse FFT during real-time synthesis. */
    kiss_fftr_cfg inverseFFT;

    /** object for doing the inverse FFT when rendering previews */
    kiss_fftr_cfg previewInverseFFT;

    /** object for inverse FFT when exporting data */
    kiss_fftr_cfg exportInverseFFT;

    /** Object for <b>forward</b> transforms for importing and anlyzing audio data. */
    kiss_fftr_cfg importFFT;

    /**This keeps track of where we are in copying a DFT into the buffer.
    
    This index is incremented until reaching the size of a soundshape chunk,
    at which point it wraps back to zero. This is to prevent a discontinuity
    from resetting audio output to the beginning of the DFT while the last sample
    was at a point that would cause discontinuity.
    */
    int currentIndex = 0;

    /**
    \see currentIndex
    
    This does a similar job as currentIndex but is used for rednering file IO instead of realtime IO.
    */
    int currentExportIndex = 0;

    /** If this exceeds the samples per chunk, were done with that chunk.
    
    This is incremented by the output buffer size and shouldn't be used to calculate
    the current position in a loop wirting to the buffer.*/
    int samplesWritten = 0;

    /** keep track of where to start and end a sound's playback. This are manged by AudioParameters */
    int beginningChunk = 0;
    int endingChunk = 1;

    bool sustainPressed = false;

    /**
    Keeps track of if there are any active keys.
    
    Useful for corssfading.*/
    bool anyKeysPressed = false;

    /**
    Keeps track of if we need crossfade the previous chunk with the current chunk.

    This gets set true once we've held a a key during a chunk and set back to false 
    once all keys are released.
    */
    bool shouldCrossfadeChunk = false;

};