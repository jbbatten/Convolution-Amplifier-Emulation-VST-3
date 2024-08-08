/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ConvolutionProjectAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    ConvolutionProjectAudioProcessor();
    ~ConvolutionProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //Processor Chain enum, position in the processor chain
    enum
    {
        preGainIndex = 0,
        bassFilter,
        midIndex,
        trebleIndex,
        waveshaperIndex,
        convolutionIndex,
        reverbIndex,
        postGainIndex,
    };

    //current microphone selected for convolution engine
    int microphoneSelection;

    //update functions
    void updateCoefficients();
    void updateImpulseResponse();

    //Aliases
    using StereoEQ = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    using Gain = juce::dsp::Gain<float>;
    using WaveShaper = juce::dsp::WaveShaper<float>;
    using Reverb = juce::dsp::Reverb;
    using ConvolutionEngine = juce::dsp::Convolution;

    //Processor chain with all objects
       juce::dsp::ProcessorChain
        <
        Gain,
        StereoEQ,
        StereoEQ,
        StereoEQ,
        WaveShaper,
        ConvolutionEngine,
        Reverb,
        Gain
        >
        processorChain;

    //APVTS parameters
       juce::AudioProcessorValueTreeState parameters;

private:

    //create parameter layout function
    juce::AudioProcessorValueTreeState::ParameterLayout paramLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConvolutionProjectAudioProcessor)

};
