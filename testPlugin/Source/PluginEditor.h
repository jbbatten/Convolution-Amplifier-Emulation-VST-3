/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OtherLookAndFeel.h"


//==============================================================================
/**
*/
class TestPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    TestPluginAudioProcessorEditor (TestPluginAudioProcessor&);
    ~TestPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    OtherLookAndFeel design;

    juce::Slider gainSlider;
    juce::Slider chorusRate;
    juce::Slider chorusDepth;
    juce::Slider chorusCentreDelay;
    juce::Slider chorusFeedback;
    juce::Slider chorusMix;

    juce::Label gainLabel;
    juce::Label chorusRateLabel;
    juce::Label chorusDepthLabel;
    juce::Label chorusCentreDelayLabel;
    juce::Label chorusFeedbackLabel;
    juce::Label chorusMixLabel;

    TestPluginAudioProcessor& audioProcessor;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusCentreDelayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusFeedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusMixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestPluginAudioProcessorEditor)


 
};
