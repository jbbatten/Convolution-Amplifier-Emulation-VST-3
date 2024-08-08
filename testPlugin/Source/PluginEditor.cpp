/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TestPluginAudioProcessorEditor::TestPluginAudioProcessorEditor(TestPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    gainSlider.setLookAndFeel(&design);
    chorusRate.setLookAndFeel(&design);
    chorusDepth.setLookAndFeel(&design);
    chorusCentreDelay.setLookAndFeel(&design);
    chorusFeedback.setLookAndFeel(&design);
    chorusMix.setLookAndFeel(&design);
  
    //gain slider
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    //labels
    gainLabel.setText("Gain", dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainSlider, false);

    //CHORUS
    //chorus rate
    chorusRate.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chorusRate.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    //labels
    chorusRateLabel.setText("Rate", dontSendNotification);
    chorusRateLabel.setJustificationType(juce::Justification::centred);
    chorusRateLabel.attachToComponent(&chorusRate, false);

    //chorus depth
    chorusDepth.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chorusDepth.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    //labels
    chorusDepthLabel.setText("Depth", dontSendNotification);
    chorusDepthLabel.setJustificationType(juce::Justification::centred);
    chorusDepthLabel.attachToComponent(&chorusDepth, false);

    //chorus centre delay
    chorusCentreDelay.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chorusCentreDelay.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    //labels
    chorusCentreDelayLabel.setText("Centre Delay", dontSendNotification);
    chorusCentreDelayLabel.setJustificationType(juce::Justification::centred);
    chorusCentreDelayLabel.attachToComponent(&chorusCentreDelay, false);

    //chorus feedback
    chorusFeedback.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chorusFeedback.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    //labels
    chorusFeedbackLabel.setText("Feedback", dontSendNotification);
    chorusFeedbackLabel.setJustificationType(juce::Justification::centred);
    chorusFeedbackLabel.attachToComponent(&chorusFeedback, false);

    //chorus mix
    chorusMix.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    chorusMix.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    //labels
    chorusMixLabel.setText("Mix", dontSendNotification);
    chorusMixLabel.setJustificationType(juce::Justification::centred);
    chorusMixLabel.attachToComponent(&chorusMix, false);


    //sliders
    addAndMakeVisible(chorusRate);
    addAndMakeVisible(chorusDepth);
    addAndMakeVisible(chorusCentreDelay);
    addAndMakeVisible(chorusFeedback);
    addAndMakeVisible(chorusMix);
    addAndMakeVisible(gainSlider);

    //labels
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(chorusRateLabel);
    addAndMakeVisible(chorusDepthLabel);
    addAndMakeVisible(chorusCentreDelayLabel);
    addAndMakeVisible(chorusFeedbackLabel);
    addAndMakeVisible(chorusMixLabel);

    //attachments
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "GAIN", gainSlider);
    chorusRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "CHORUS_RATE", chorusRate);
    chorusDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "CHORUS_DEPTH", chorusDepth);
    chorusCentreDelayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "CHORUS_CENTREDELAY", chorusCentreDelay);
    chorusFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "CHORUS_FEEDBACK", chorusFeedback);
    chorusMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "CHORUS_MIX", chorusMix);


    setSize(1200, 500);
}

TestPluginAudioProcessorEditor::~TestPluginAudioProcessorEditor()
{
}

//==============================================================================
void TestPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void TestPluginAudioProcessorEditor::resized()
{
    auto rowW = getWidth() / 10;
    gainSlider.setBounds(rowW + 75, getHeight() / 2 - 100, 150, 150);
    chorusRate.setBounds((rowW * 2) + 75, getHeight() / 2 - 100, 150, 150);
    chorusDepth.setBounds((rowW * 3) + 75, getHeight() / 2 - 100, 150, 150);
    chorusCentreDelay.setBounds((rowW * 4) + 75, getHeight() / 2 - 100, 150, 150);
    chorusFeedback.setBounds((rowW * 5) + 75, getHeight() / 2 - 100, 150, 150);
    chorusMix.setBounds((rowW * 6) + 75, getHeight() / 2 - 100, 150, 150);
}
