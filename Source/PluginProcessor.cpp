
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "IOHandler.h"

#define CATCH_CONFIG_RUNNER
#include "catch.h"

//==============================================================================
Soundshape_pluginAudioProcessor::Soundshape_pluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    valueTreeState(*this, nullptr, Identifier("Soundshape"), {
                                std::make_unique<AudioParameterFloat>(
                                    "gain",
                                    "Gain",
                                    0.00f,
                                    0.99f,
                                    0.80f),
                                std::make_unique<AudioParameterFloat>(
                                    "attack",
                                    "Attack",
                                    0.01f,
                                    2.50f,
                                    0.25f),
                                std::make_unique<AudioParameterFloat>(
                                    "decay",
                                    "Decay",
                                    0.01f,
                                    2.50f,
                                    0.25f),
                                std::make_unique<AudioParameterFloat>(
                                    "sustain",
                                    "Sustain",
                                    0.00f,
                                    0.99f,
                                    0.75f),
                                std::make_unique<AudioParameterFloat>(
                                    "release",
                                    "Release",
                                    0.01f,
                                    2.50f,
                                    0.25f),
                                std::make_unique<AudioParameterInt>(
                                    "beginningChunk",
                                    "Beginning Section",
                                    0,
                                    50,
                                    0),
                                std::make_unique<AudioParameterInt>(
                                    "endingChunk",
                                    "Ending Section",
                                    0,
                                    50,
                                    50)
                                }
    ),
    converter(valueTreeState)
#endif
{

    // different parameters can have different objects handle the logic of them changing if necessary
    valueTreeState.addParameterListener("attack", &converter.getEnvelope());
    valueTreeState.addParameterListener("decay", &converter.getEnvelope());
    valueTreeState.addParameterListener("sustain", &converter.getEnvelope());
    valueTreeState.addParameterListener("release", &converter.getEnvelope());
    valueTreeState.addParameterListener("gain", &converter);

    // the beginningChunk and endingChunk parameters need 2 listeners : the converter and also the GUI.
    // Here, we set the converter as a listener to these. The GUI will set itself up to listen
    valueTreeState.addParameterListener("beginningChunk", &converter);
    valueTreeState.addParameterListener("endingChunk", &converter);

    // add the converter as a listener to the midi keyboard state
    keyState.addListener(&converter);


    // TODO : SHOULD THIS BE THE DEFAULT PROFILE?
    converter.setSampleRate(44100.0f);
    for (int i = 0; i < SOUNDSHAPE_PROFILE_ROWS; i++) {
        converter.updateFrequencyValue(i, 1 * 440, 500.0f);
        converter.updateFrequencyValue(i, 2 * 440, 300.0f);
        converter.updateFrequencyValue(i, 4 * 440, 200.0f);
        converter.updateFrequencyValue(i, 6 * 440, 100.0f);
        converter.updateFrequencyValue(i, 8 * 440, 50.0f);
        converter.renderPreview(i);
    }

    if (SOUNDSHAPE_RUN_TESTS) {
        int result = Catch::Session().run();
    }

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

void Soundshape_pluginAudioProcessor::panic()
{
    for (int i = 0; i < 16; i++) {
        keyState.allNotesOff(i);
    }

    // TEMPORARY testing xml
    // *********************
    auto state = valueTreeState.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    XmlElement *profileXML = IOHandler::createProfileXML(converter);
    xml->addChildElement(profileXML);
    String docString = xml->createDocument("");
    File f(File::getCurrentWorkingDirectory().getChildFile("test.xml"));
    FileOutputStream ostream(f);
    if (ostream.openedOk()) {
        ostream.setPosition(0);
        ostream.truncate();
        ostream.writeText(docString,false,false,nullptr);
    }
    // ****************************
}

void Soundshape_pluginAudioProcessor::playFreq(float freq)
{
    keyState.noteOn(1, freqToMidiNote( freq ,440.0f), 1.0f);
}
bool Soundshape_pluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}




//==============================================================================
void Soundshape_pluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // The Converter needs to know about the sample rate in order to convert
    // between frequency values and indexes for its internal structure
    converter.setSampleRate(sampleRate);

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
    MidiBuffer::Iterator messagesIter(midiMessages);
    MidiMessage currentMessage;
    int sampleNum;
    while (messagesIter.getNextEvent(currentMessage, sampleNum)) {
        if (currentMessage.isSustainPedalOn()) {
            converter.setSustain(true);
        }
        else if (currentMessage.isSustainPedalOff()) {
            converter.setSustain(false);
        }
    }

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
    Soundshape_pluginAudioProcessorEditor* editor = new Soundshape_pluginAudioProcessorEditor(*this, valueTreeState);
    return editor;
}

//==============================================================================
void Soundshape_pluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    
}

void Soundshape_pluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

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

AudioProcessorValueTreeState & Soundshape_pluginAudioProcessor::getTreeState()
{
    return valueTreeState;
}

int Soundshape_pluginAudioProcessor::freqToMidiNote(float freq, float freqOfA)
{
    return (int)std::round( 12 * std::log2(freq/freqOfA) + 69 );
}
