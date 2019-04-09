#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"

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


    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;

    // these are called from the host for basic information about this plugin
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

    void panic();
    void playFreq(float freq);


    // serializing parameters
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    // accessors
    Converter &getConverter();

    AudioProcessorValueTreeState &getTreeState();

private:

    int freqToMidiNote(float freq, float freqOfA);
    MidiKeyboardState keyState; // tracks which MIDI keys are down
    Converter converter; // performs DSP tasks, manages some parameters.
    int currentChunk = 0;
    AudioProcessorValueTreeState valueTreeState;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Soundshape_pluginAudioProcessor)
};
