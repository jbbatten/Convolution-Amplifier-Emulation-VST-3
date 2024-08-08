#pragma once

#include <JuceHeader.h>

// NOT IN USE ANYMORE //

class OutputTrimLAF : public juce::LookAndFeel_V4
{
public:
    OutputTrimLAF()
    {
        knob = juce::ImageCache::getFromMemory(BinaryData::trim_output_png, BinaryData::trim_output_pngSize);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {

        float rotation = juce::MathConstants<float>::twoPi * sliderPos * 0.83;

        auto transform = juce::AffineTransform::rotation(rotation, knob.getWidth() / 2, knob.getHeight() / 2);
        g.drawImageTransformed(knob, transform, false);

    };


private:
    juce::Image knob;

};
