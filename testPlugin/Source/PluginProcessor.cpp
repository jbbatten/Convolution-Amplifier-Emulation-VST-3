/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace juce;

//==============================================================================
TestPluginAudioProcessor::TestPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
          .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
          .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

TestPluginAudioProcessor::~TestPluginAudioProcessor()
{
}

//==============================================================================
const juce::String TestPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TestPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool TestPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool TestPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double TestPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TestPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int TestPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TestPluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String TestPluginAudioProcessor::getProgramName(int index)
{
    return {};
}

void TestPluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void TestPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    chorus.prepare(spec);
    chorus.reset();
}

void TestPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif


void TestPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto gain = apvts.getRawParameterValue("GAIN");
    float g = gain->load();

    auto cRate = apvts.getRawParameterValue("CHORUS_RATE");
    float chorusRate = cRate->load();

    auto cDepth = apvts.getRawParameterValue("CHORUS_DEPTH");
    float chorusDepth = cDepth->load();

    auto cCentreDelay = apvts.getRawParameterValue("CHORUS_CENTREDELAY");
    float chorusCentreDelay = cCentreDelay->load();

    auto cFeedback = apvts.getRawParameterValue("CHORUS_FEEDBACK");
    float chorusFeedback = cFeedback->load();

    auto cMix = apvts.getRawParameterValue("CHORUS_MIX");
    float chorusMix = cMix->load();

    chorus.setRate(chorusRate);
    chorus.setDepth(chorusDepth);
    chorus.setCentreDelay(chorusCentreDelay);
    chorus.setFeedback(chorusFeedback);
    chorus.setMix(chorusMix);

    juce::dsp::AudioBlock<float> sampleBlock(buffer);
    chorus.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));

    for (int channel = 0; channel < numChannels; channel++)
    {
        auto* writePointer = buffer.getWritePointer(channel);

        for (int samples = 0; samples < numSamples; samples++)
        {
            writePointer[samples] = writePointer[samples] * juce::Decibels::decibelsToGain(g);
        }

    }
}

//==============================================================================
bool TestPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TestPluginAudioProcessor::createEditor()
{
    return new TestPluginAudioProcessorEditor(*this);
}

//==============================================================================
void TestPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TestPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TestPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout TestPluginAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>("GAIN", "Gain", -60.0f, 10.0f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("CHORUS_RATE", "cRate", 0, 10.0f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("CHORUS_DEPTH", "cDepth", 0, 0.10f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("CHORUS_CENTREDELAY", "cCentreDelay", 0, 50.0f, 25.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("CHORUS_FEEDBACK", "cFeedback", -0.60f, 0.60f, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("CHORUS_MIX", "cMix", 0, 0.99f, 0.0f));

    return {params.begin(), params.end()};
}

