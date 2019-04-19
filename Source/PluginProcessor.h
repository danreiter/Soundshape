#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"

class Soundshape_pluginAudioProcessorEditor;
/**
The AudioProcessor for Soundshape. This is where the host's audio buffers are filled in a real-time callback.

Soundshape's AudioProcessor facilitates communication between the host and the rest of Soundshape.
It defines methods for responding to host queries, serializing and deserializing its
state when the host asks, and most importantly fills audio buffers.
It's important that any code that happens in the audio thread finishes quickly and doesn't
do things like acquiring locks or memory. The AudioProcessor also owns
some AudioParameters managed by an AudioProcessorValueTreeState object.
These parameters are controllable from the DAW and are attached to components of
Soundshape_pluginAudioProcessorEditor that this object creates when necessary.

Its signal processing and data are mostly handled by the Converter class.

*/
class Soundshape_pluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Soundshape_pluginAudioProcessor();
    ~Soundshape_pluginAudioProcessor();

    /**
    Called by the host whenever the plugin needs to know about the sample rate changing or
    the number of samples per block of audio.

    The Converter needs to know the sample rate for synthesis as well as for managing its 
    envelopes.
    */
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    /**
    The real-time audio synthesization callback.

    Gets information about the requested block of audio and then asks the Converter
    to fill this block with audio data. <b>No code that has unpredicatable completion
    time (like acquiring a mutex lock or allocating data in the free store) should happen
    in the thread that calls this.</b>
    */
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

    bool hasEditor() const override;
    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;


    /**
    Shuts off all MIDI notes in the converter.
    */
    void panic();

    /**
    Simulates the pressing of a MIDI note with a particular frequency
    */
    void playFreq(float freq);


    /**
    * Serializes the state of Soundshape into a block of binary data.

    This makes an XML file of the state of the plugin and converts it into a binary
    blob. This can be called at any time by the host and is not only used for when
    the user makes the DAW save a preset; the DAW may also call this whenever you save your
    project or any other circumstance.

    @param destData The block of data to write the state to.
    */
    void getStateInformation (MemoryBlock& destData) override;

    /**
    Restore the state of the plugin from a binary data block.

    This restores all the audio parameters and profile information to the Converter and
    makes the GUI repaint its profiles.

    @param data The data block containing the binary state
    @param sizeInBytes Size of the data block
    */
    void setStateInformation (const void* data, int sizeInBytes) override;


    Converter &getConverter();

    AudioProcessorValueTreeState &getTreeState();

    /**
    Resets the state of the audio parameters (ADSR, gain, play region) 
    to the state they were in when the processor was constructed.
    
    This can be called from the GUI whenever a default sound preset has been selected.
    */
    void restoreNewSoundParams();

    /**
    Sets the profile spikes to some default values.

    This can be called whjen the processor gets constructed or when the editor
    wants to reset its state. (It should also call Soundshape_plugin::restoreNewSoundParams
    to reset the audio parameters.)
    */
    void restoreDefaultProfile();

private:

    int freqToMidiNote(float freq, float freqOfA);
    MidiKeyboardState keyState; // tracks which MIDI keys are down
    Converter converter; // performs DSP tasks, manages some parameters.
    AudioProcessorValueTreeState valueTreeState;
    Soundshape_pluginAudioProcessorEditor* editor;

    // this MUST be decalred after the value tree state
    std::unique_ptr<XmlElement> newSoundParamState;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Soundshape_pluginAudioProcessor)
};
