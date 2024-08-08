
#pragma once

#include <JuceHeader.h>

class VibroluxLAF : public juce::LookAndFeel_V4
{
public:
    VibroluxLAF()
    {
        //initialization for colours of components
        setColour(juce::ComboBox::backgroundColourId, juce::Colour::fromRGB(208, 184, 157));
        setColour(juce::ComboBox::outlineColourId, juce::Colour::fromRGB(29, 24, 20));
        setColour(juce::PopupMenu::backgroundColourId, juce::Colour::fromRGB(208, 184, 157));
     
        setColour(juce::ComboBox::textColourId, juce::Colours::black);
        setColour(juce::PopupMenu::textColourId, juce::Colours::black);

        setColour(juce::Slider::thumbColourId, juce::Colours::red);
        //loads images for knobs and bypass buttons
        knob = juce::ImageCache::getFromMemory(BinaryData::vibrolux_faceplate_knob_updated_png, BinaryData::vibrolux_faceplate_knob_updated_pngSize);
        onButton = juce::ImageCache::getFromMemory(BinaryData::on_button_png, BinaryData::on_button_pngSize);
        offButton = juce::ImageCache::getFromMemory(BinaryData::off_button_png, BinaryData::off_button_pngSize);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override 
    {
    	//works out rotation for sliders
        float rotation = juce::MathConstants<float>::twoPi * sliderPos * 0.83;
        auto transform = juce::AffineTransform::rotation(rotation, knob.getWidth()/2, knob.getHeight()/2);
        g.drawImageTransformed(knob, transform, false);

    };

   void drawTickBox(juce::Graphics& g, juce::Component&, float x, float y, float w, float h, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
   {
		//changes graphics for bypass button logo
		if(ticked)
		g.drawImageWithin(onButton, 0,12.5,20,20,juce::RectanglePlacement::stretchToFit,false);

		if(!ticked)
            g.drawImageWithin(offButton, 0,12.5,20,20,juce::RectanglePlacement::stretchToFit,false);
			
   }

private:
    juce::Image knob;
    juce::Image onButton;
    juce::Image offButton;

};
