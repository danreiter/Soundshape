
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Soundshape_pluginAudioProcessor::Soundshape_pluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
    , converter()
#endif
{
    // register audio parameters. Their pointers are stored in the converter object and its envelope object.
    addParameter(converter.getEnvelope().setAttackParamPtr(new AudioParameterInt("attack",
        "Attack",0,88200,11025)));
    // TODO register the rest of the parameters here, like this.
}

Soundshape_pluginAudioProcessor::~Soundshape_pluginAudioProcessor()
{
}

//==============================================================================
// Used by host to get basic info
//==============================================================================
const String Soundshape_pluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}
bool Soundshape_pluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}
bool Soundshape_pluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}
bool Soundshape_pluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}
double Soundshape_pluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}
int Soundshape_pluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}
int Soundshape_pluginAudioProcessor::getCurrentProgram()
{
    return 0;
}
void Soundshape_pluginAudioProcessor::setCurrentProgram (int index)
{
}
const String Soundshape_pluginAudioProcessor::getProgramName (int index)
{
    return {};
}
void Soundshape_pluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}
bool Soundshape_pluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}




//==============================================================================
void Soundshape_pluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Soundshape_pluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Soundshape_pluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif



//  real time audio callback function
void Soundshape_pluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numSamples = buffer.getNumSamples();

    // update the keyboard state with new messages in MIDI buffer
    keyState.processNextMidiBuffer(midiMessages, 0, numSamples, true);


    // clear all input channels (because they will be used as output, even if we dont write to them)
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
    // To determine the index of the appropriate chunk, we keep track of the
    // beginning of the UI chunk range and the end of it. Once notes start being pressed,
    // we start keeping track of how many samples we process. Once this number exceeds
    // the number of samples that each chunk represents, we increment the index (accounting
    // for things like looping). If there are no notes down at the moment, then
    // we should reset the index to the beginning of the range in the UI slider.
    // We should also skip all DSP if there are no notes down or the number of samples to process
    // happens to be 0.
    converter.synthesize(currentChunk, buffer, keyState);
    

}


AudioProcessorEditor* Soundshape_pluginAudioProcessor::createEditor()
{
    return new Soundshape_pluginAudioProcessorEditor (*this);
}

//==============================================================================
void Soundshape_pluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Soundshape_pluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Soundshape_pluginAudioProcessor();
}

// accessor for the converter (for changing parameters that the converter manages)
Converter& Soundshape_pluginAudioProcessor::getConverter() {
    return converter;
}
