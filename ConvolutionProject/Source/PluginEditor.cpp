/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ConvolutionProjectAudioProcessorEditor::ConvolutionProjectAudioProcessorEditor(ConvolutionProjectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{

    //attaching sliders to APVTS
    inputGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "PREGAIN", inputGainSlider);
    trebleSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "TREBLE", trebleSlider);
    bassSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "BASS", bassSlider);
    reverbSliderAttachhment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "REVERBWET", reverbSlider);
    outputTrimSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.parameters, "OUTPUTTRIM", outputTrimSlider);

    //setting look and feel of sliders with vibrolux LAF
    inputGainSlider.setLookAndFeel(&vibroluxLAF);
    trebleSlider.setLookAndFeel(&vibroluxLAF);
    bassSlider.setLookAndFeel(&vibroluxLAF);
    reverbSlider.setLookAndFeel(&vibroluxLAF);
    outputTrimSlider.setLookAndFeel(&vibroluxLAF);
    micMenu.setLookAndFeel(&vibroluxLAF);
      //setting look nad feel for bypass buttons
	bassEQBypass.setLookAndFeel(&vibroluxLAF);
	midEQBypass.setLookAndFeel(&vibroluxLAF);
	trebleEQBypass.setLookAndFeel(&vibroluxLAF);
	waveshaperBypass.setLookAndFeel(&vibroluxLAF);
	convolutionBypass.setLookAndFeel(&vibroluxLAF);
	reverbBypass.setLookAndFeel(&vibroluxLAF);

	addAndMakeVisible(bassEQBypass);
    addAndMakeVisible(midEQBypass);
    addAndMakeVisible(trebleEQBypass);
    addAndMakeVisible(waveshaperBypass);
    addAndMakeVisible(convolutionBypass);
    addAndMakeVisible(reverbBypass);
    //microphone menu setup
    addAndMakeVisible(micLabel);
    micLabel.setFont(textFont);
    micLabel.attachToComponent(&micMenu, false);
    addAndMakeVisible(micMenu);

    //adding all of the microphones to the microphone menu
    micMenu.addItem("Shure SM57", SM57);
    micMenu.addItem("Royer 121", ROYER121);
    micMenu.addItem("Neumann KM 184", NEUMANN184);
    micMenu.addItem("AKG C414 XLS", AKG414);
    micMenu.addItem("Sennheiser MD421", SENNHEISER421);
    micMenu.addItem("Sennheiser e906", SENNHEISER906);
    micMenu.addItem("Neumann U67", NEUMANN67);

    //calling mic menu changed function when selection made
    micMenu.onChange = [this] { micMenuChanged(); };
    micMenu.setSelectedId(1);

    //setup for all sliders
    //input slider
    inputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
    addAndMakeVisible(inputGainSlider);
    //treble slider
    trebleSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    trebleSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
    addAndMakeVisible(trebleSlider);
    //bass slider
    bassSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    bassSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
    addAndMakeVisible(bassSlider);
    //reverb slider
    reverbSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    reverbSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
    addAndMakeVisible(reverbSlider);  
    //output trim slider
    outputTrimSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    outputTrimSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 50, 20);
    addAndMakeVisible(outputTrimSlider);

    //setup for bypass buttons, setting all bypass buttons to on (true)
    bassEQBypass.setToggleState(true, juce::NotificationType::dontSendNotification);
    midEQBypass.setToggleState(true, juce::NotificationType::dontSendNotification);
    trebleEQBypass.setToggleState(true, juce::NotificationType::dontSendNotification);
    waveshaperBypass.setToggleState(true, juce::NotificationType::dontSendNotification);
    convolutionBypass.setToggleState(true, juce::NotificationType::dontSendNotification);
    reverbBypass.setToggleState(true, juce::NotificationType::dontSendNotification);

    //on clicking bypass button send which button was clicked to updateToggleState function
    bassEQBypass.onClick = [this]{updateToggleState(&bassEQBypass, "Bass");};
	midEQBypass.onClick = [this]{updateToggleState(&midEQBypass, "Mid");};
    trebleEQBypass.onClick = [this]{updateToggleState(&trebleEQBypass, "Treble");};
    waveshaperBypass.onClick = [this]{updateToggleState(&waveshaperBypass, "Waveshaper");};
    convolutionBypass.onClick = [this]{updateToggleState(&convolutionBypass, "Convolution");};
    reverbBypass.onClick = [this]{updateToggleState(&reverbBypass, "Reverb");};

    //sets size of plugin
    setSize(800, 400);
}

void ConvolutionProjectAudioProcessorEditor::micMenuChanged(){

    //selects the microphone from the menu and passes it to the microphone selection member variable in the audio processor
    switch (micMenu.getSelectedId())
    {
        case SM57: 
             audioProcessor.microphoneSelection = 1; break;
        case ROYER121: 
             audioProcessor.microphoneSelection = 2; break;
        case NEUMANN184: 
            audioProcessor.microphoneSelection = 3; break;
        case AKG414: 
            audioProcessor.microphoneSelection = 4; break;
        case SENNHEISER421: 
            audioProcessor.microphoneSelection = 5; break;
        case SENNHEISER906: 
            audioProcessor.microphoneSelection = 6; break;
        case NEUMANN67: 
            audioProcessor.microphoneSelection = 7; break;
        default: break;
    }
};


ConvolutionProjectAudioProcessorEditor::~ConvolutionProjectAudioProcessorEditor()
{
}

//==============================================================================
void ConvolutionProjectAudioProcessorEditor::paint(juce::Graphics& g)
{
    //sets background image
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::vibrolux_faceplate_updated_png, BinaryData::vibrolux_faceplate_updated_pngSize);
    g.drawImageAt(background, 0, 0);
}

void ConvolutionProjectAudioProcessorEditor::resized()
{
    //setting bounds for components
    auto area = getLocalBounds();
    auto headerFooterHeight = 40;
    auto contentItemHeight = 200;

    trebleEQBypass.setBounds(183,240,50,50);
    bassEQBypass.setBounds(273,240,50,50);
    reverbBypass.setBounds(363,240,50,50);
	midEQBypass.setBounds(578,170,100,40);
	convolutionBypass.setBounds(578,205,100,40);
	waveshaperBypass.setBounds(578,240,100,40);

    micMenu.setBounds(580, 150, 150, 20);
    inputGainSlider.setBounds(63, 120, 110, 110);
    trebleSlider.setBounds(153, 120, 110, 110);
    bassSlider.setBounds(243, 120, 110, 110);
    reverbSlider.setBounds(333, 120, 110, 110);
    outputTrimSlider.setBounds(423, 120, 110, 110);
}


void ConvolutionProjectAudioProcessorEditor::updateToggleState (juce::Button* button, juce::String name)
{

    //functionality to bypass processor chain component when clicked
	auto state = button->getToggleState();
	juce::String stateString = state ? "ON" : "OFF";

   if(name == "Bass")
   {
	   if(stateString == "ON")
	   {
		   audioProcessor.processorChain.setBypassed<1>(false);
	   }
       else
       {
	       audioProcessor.processorChain.setBypassed<1>(true);
       }
   }

    if(name == "Mid")
   {
	   if(stateString == "ON")
	   {
		   audioProcessor.processorChain.setBypassed<2>(false);
	   }
       else
       {
	       audioProcessor.processorChain.setBypassed<2>(true);
       }
   }

	if(name == "Treble")
   {
	   if(stateString == "ON")
	   {
		   audioProcessor.processorChain.setBypassed<3>(false);
	   }
       else
       {
	       audioProcessor.processorChain.setBypassed<3>(true);
       }
   }

    if(name == "Waveshaper")
   {
	   if(stateString == "ON")
	   {
		   audioProcessor.processorChain.setBypassed<4>(false);
	   }
       else
       {
	       audioProcessor.processorChain.setBypassed<4>(true);
       }
   }

	if(name == "Convolution")
   {
	   if(stateString == "ON")
	   {
		   audioProcessor.processorChain.setBypassed<5>(false);
	   }
       else
       {
	       audioProcessor.processorChain.setBypassed<5>(true);
       }
   }

	if(name == "Reverb")
   {
	   if(stateString == "ON")
	   {
		   audioProcessor.processorChain.setBypassed<6>(false);
	   }
       else
       {
	       audioProcessor.processorChain.setBypassed<6>(true);
       }
   }

}


