/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VibroluxLAF.h"
#include "OutputTrimLAF.h"
#include "RMSmeter.h"

//==============================================================================
/**
*/
class ConvolutionProjectAudioProcessorEditor : public juce::AudioProcessorEditor
                                                 

{
public:
    ConvolutionProjectAudioProcessorEditor(ConvolutionProjectAudioProcessor&);
    ~ConvolutionProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
   

private:

    //Not in use anymore
    juce::TextButton header;
    juce::TextButton footer;
    juce::TextButton convolutionButton {"Convolution"};
    juce::TextButton filterButton {"Filter"};
    juce::TextButton preGainButton {"Pre Gain"};
    juce::TextButton waveShaperButton {"WaveShaper"};
    juce::TextButton postGainButton {"Post Gain"};
    juce::Label path;

    //microphone selection menu
    juce::ComboBox micMenu;
    juce::Label micLabel{ {}, "Microphones:" };
    juce::Font textFont{ 12.0f };

    //function for switching microphones
    void micMenuChanged();

    //Enum for microphone selection
    enum MicrophoneSelection {

        SM57 = 1,
        ROYER121,
        NEUMANN184,
        AKG414,
        SENNHEISER421,
        SENNHEISER906,
        NEUMANN67
    };

    //look and feel classes
    VibroluxLAF vibroluxLAF;
    //OutputTrimLAF NOT IN USE
    OutputTrimLAF outputTrimLAF;

    //Sliders and slider attachments
    juce::Slider inputGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainSliderAttachment;

    juce::Slider trebleSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleSliderAttachment;

    juce::Slider bassSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassSliderAttachment;

    juce::Slider reverbSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> reverbSliderAttachhment;

    juce::Slider outputTrimSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputTrimSliderAttachment;

    // void updateToggleState(juce::Button* button, juce::String name);
    void updateToggleState (juce::Button* button, juce::String name);

    //Bypass buttons
    juce::ToggleButton bassEQBypass;
    juce::ToggleButton midEQBypass{"Mid Scoop"};
    juce::ToggleButton trebleEQBypass;
    juce::ToggleButton waveshaperBypass{"Waveshaper"};
    juce::ToggleButton convolutionBypass{"Convolution"};
    juce::ToggleButton reverbBypass;

    //reference to the audio processor
    ConvolutionProjectAudioProcessor& audioProcessor;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConvolutionProjectAudioProcessorEditor)
};
