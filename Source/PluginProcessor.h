#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Converter.h"

//==============================================================================
/**
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

    // serializing parameters
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // accessors
    Converter& getConverter();


private:
    MidiKeyboardState keyState; // tracks which MIDI keys are down
    Converter converter; // performs DSP tasks, manages some parameters.
    int currentChunk = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Soundshape_pluginAudioProcessor)
};
