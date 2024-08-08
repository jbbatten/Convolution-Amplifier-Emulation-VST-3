/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
using namespace std;

//==============================================================================
ConvolutionProjectAudioProcessor::ConvolutionProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		  .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		  .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	  ),
	  parameters(*this, nullptr, juce::Identifier("parameters"), paramLayout())


#endif
{
	//define waveshaper function,
	//asymptotic function
	auto& waveshaper = processorChain.get<waveshaperIndex>();
	waveshaper.functionToUse = [](float x)
	{
		//hyperbolic tangent function (Not in use)
		/*return std::tanh(x);*/

		//Asymptotic function
		return x / (std::abs(x) + 1);
	};
}

ConvolutionProjectAudioProcessor::~ConvolutionProjectAudioProcessor()
{
}


juce::AudioProcessorValueTreeState::ParameterLayout ConvolutionProjectAudioProcessor::paramLayout()
{
	//creates parameter layout for APVTS and returns vector of all parameters
	std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

	parameters.push_back(std::make_unique<juce::AudioParameterFloat>("PREGAIN", "PreGain", 0.0f, 60.0f, 15.0f));
	parameters.push_back(std::make_unique<juce::AudioParameterFloat>("TREBLE", "Treble", 0.5f, 2.0f, 0.5f));
	parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BASS", "Bass", 0.1f, 1.0f, 0.1f));
	parameters.push_back(std::make_unique<juce::AudioParameterFloat>("REVERBWET", "ReverbWet", 0.0f, 0.8f, 0.01f));
	parameters.push_back(std::make_unique<juce::AudioParameterFloat>("OUTPUTTRIM", "OutputTrim", -20.0f, 30.0f, 0.0f));

	return { parameters.begin(), parameters.end() };
}

//==============================================================================
const juce::String ConvolutionProjectAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool ConvolutionProjectAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
    return false;
#endif
}

bool ConvolutionProjectAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
    return false;
#endif
}

bool ConvolutionProjectAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
	return false;
#endif
}

double ConvolutionProjectAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ConvolutionProjectAudioProcessor::getNumPrograms()
{
	return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int ConvolutionProjectAudioProcessor::getCurrentProgram()
{
	return 0;
}

void ConvolutionProjectAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ConvolutionProjectAudioProcessor::getProgramName(int index)
{
	return {};
}

void ConvolutionProjectAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void ConvolutionProjectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	//process spec that is passed to processor chain
	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumOutputChannels();

	auto preGainValue = parameters.getRawParameterValue("PREGAIN");
	auto outputTrimValue = parameters.getRawParameterValue("OUTPUTTRIM");

	//sets input gain at initialization so volume isn't crazy when audio buffer is populated for first time.
	auto& preGain = processorChain.template get<preGainIndex>();
	preGain.setGainDecibels(preGainValue->load());

	//sets output gain at initialization so volume isn't crazy when audio buffer is populated for first time.
	auto& outputGain = processorChain.template get<postGainIndex>();
	outputGain.setGainDecibels(outputTrimValue->load());

	//processor chain prepared with spec information, sample rate, maximum block size and number of channels
	processorChain.prepare(spec);

	//update all of the impulse responses and coefficients
	updateImpulseResponse();
	updateCoefficients();

}

void ConvolutionProjectAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.

	processorChain.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ConvolutionProjectAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void ConvolutionProjectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;

	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	//clears buffer of any garbage before processing
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	auto preGainValue = parameters.getRawParameterValue("PREGAIN");
	auto outputTrimValue = parameters.getRawParameterValue("OUTPUTTRIM");

	//gets value of preGain (input gain) and loads it (conversion from atomic float to float)
	auto& preGain = processorChain.template get<preGainIndex>();
	preGain.setGainDecibels(preGainValue->load());

	//gets value of output gain and loads it (conversion from atomic float to float)
	auto& outputGain = processorChain.template get<postGainIndex>();
	outputGain.setGainDecibels(outputTrimValue->load());

	//calls update of impulse response and coefficients before processing
	updateImpulseResponse();
	updateCoefficients();

	//copy buffer into audio block
	juce::dsp::AudioBlock<float> block(buffer);

	//process the processor chain with audio block context
	juce::dsp::ProcessContextReplacing<float> context(block);
	processorChain.process(context);

}

void ConvolutionProjectAudioProcessor::updateImpulseResponse()
{
	//updates the convolution engine with new impulse response if new one has been selected
	auto& convolution = processorChain.template get<convolutionIndex>();
	
	if (microphoneSelection == 1)
	{
		convolution.loadImpulseResponse
		(
			BinaryData::FEVLUX65_FOCUS_CAB_57_wav,
			BinaryData::FEVLUX65_FOCUS_CAB_57_wavSize,
			juce::dsp::Convolution::Stereo::yes,
			juce::dsp::Convolution::Trim::no, 0
		);
	}

	if (microphoneSelection == 2)
	{
		convolution.loadImpulseResponse
		(
			BinaryData::FEVLUX65_FOCUS_CAB_121_wav,
			BinaryData::FEVLUX65_FOCUS_CAB_121_wavSize,
			juce::dsp::Convolution::Stereo::yes,
			juce::dsp::Convolution::Trim::no, 0
		);
	}

	if (microphoneSelection == 3)
	{
		convolution.loadImpulseResponse
		(
			BinaryData::FEVLUX65_FOCUS_CAB_184_wav,
			BinaryData::FEVLUX65_FOCUS_CAB_184_wavSize,
			juce::dsp::Convolution::Stereo::yes,
			juce::dsp::Convolution::Trim::no, 0
		);
	}

	if (microphoneSelection == 4)
	{
		convolution.loadImpulseResponse
		(
			BinaryData::FEVLUX65_FOCUS_CAB_414_wav,
			BinaryData::FEVLUX65_FOCUS_CAB_414_wavSize,
			juce::dsp::Convolution::Stereo::yes,
			juce::dsp::Convolution::Trim::no, 0
		);
	}

	if (microphoneSelection == 5)
	{
		convolution.loadImpulseResponse
		(
			BinaryData::FEVLUX65_FOCUS_CAB_421_wav,
			BinaryData::FEVLUX65_FOCUS_CAB_421_wavSize,
			juce::dsp::Convolution::Stereo::yes,
			juce::dsp::Convolution::Trim::no, 0
		);
	}

	if (microphoneSelection == 6)
	{
		convolution.loadImpulseResponse
		(
			BinaryData::FEVLUX65_FOCUS_CAB_906_wav,
			BinaryData::FEVLUX65_FOCUS_CAB_906_wavSize,
			juce::dsp::Convolution::Stereo::yes,
			juce::dsp::Convolution::Trim::no, 0
		);
	}

	if (microphoneSelection == 7)
	{
		convolution.loadImpulseResponse
		(
			BinaryData::FEVLUX65_FOCUS_CAB_U67_wav,
			BinaryData::FEVLUX65_FOCUS_CAB_U67_wavSize,
			juce::dsp::Convolution::Stereo::yes,
			juce::dsp::Convolution::Trim::no, 0
		);
	}
}

void ConvolutionProjectAudioProcessor::updateCoefficients()
{
	//updates coefficients for reverb and IIR filters, changing filter values based on slider values in the APVTS

	auto rverbWet = parameters.getRawParameterValue("REVERBWET");
	auto bassVal = parameters.getRawParameterValue("BASS");
	auto trebleVal = parameters.getRawParameterValue("TREBLE");

	auto& rverb = processorChain.template get<reverbIndex>();
	juce::dsp::Reverb::Parameters reverbParams;
	reverbParams.wetLevel = rverbWet->load();

	rverb.setParameters(reverbParams);

	//generates new coefficients with gain factor from APVTS
	auto bassCoeff = juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), (double)300, (double)1.6,bassVal->load());
	//sets the state of the processorchain filters with new coefficients
	*processorChain.get<1>().state = *bassCoeff;

	auto midCoeff = juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), (double)500, (double)2.56,0.005f);
	*processorChain.get<2>().state = *midCoeff;

	auto trebleCoeff = juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), (double)1500, (double)1.6, trebleVal->load());
	*processorChain.get<3>().state = *trebleCoeff;
}

//==============================================================================
bool ConvolutionProjectAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ConvolutionProjectAudioProcessor::createEditor()
{
	return new ConvolutionProjectAudioProcessorEditor(*this);
}

//==============================================================================
void ConvolutionProjectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	//memory object that retains apvts state
	juce::MemoryOutputStream memory(destData, true);
	parameters.state.writeToStream(memory);
}

void ConvolutionProjectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	//sets the apvts values if a state exist.
	auto valtree = juce::ValueTree::readFromData(data, sizeInBytes);
	if(valtree.isValid())
	{
		parameters.replaceState(valtree);
		updateCoefficients();
		updateImpulseResponse();

	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ConvolutionProjectAudioProcessor();
}
