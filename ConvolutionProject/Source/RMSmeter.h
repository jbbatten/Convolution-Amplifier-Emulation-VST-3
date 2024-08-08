
#pragma once

#include <JuceHeader.h>

//NOT IN USE
namespace Gui {

	class RMSMeter : public juce::Component
	{
	public:
		void paint(juce::Graphics& g) override
		{
			auto bounds = getLocalBounds().toFloat();

			g.setColour(juce::Colours::brown);
			g.fillRoundedRectangle(bounds, 4.0f);

			g.setColour(juce::Colours::white);
			const auto scaledX = juce::jmap(level, -60.0f, +6.f, 0.f, static_cast<float>(getWidth()));
			g.fillRoundedRectangle(bounds.removeFromLeft(scaledX), 5.f);
		}

		//setter function
		void setLevel(const float value) { level = value; };

	private:
		float level = -60.0f;


	};
}
