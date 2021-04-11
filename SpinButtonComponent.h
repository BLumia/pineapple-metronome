#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include juce module headers
// directly. If you need to remain compatible with Projucer-generated builds, and
// have called `juce_generate_juce_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <JuceHeader.h>` here instead, to make all your module headers visible.
#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class SpinButtonComponent : public juce::Component
{
public:
    //==============================================================================
    SpinButtonComponent();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    struct Trail
    {
        Trail (const juce::MouseInputSource& ms)
            : source (ms)
        {}

        void pushPoint (juce::Point<float> newPoint, juce::ModifierKeys newMods, float pressure)
        {
            currentPosition = newPoint;
            modifierKeys = newMods;

            if (lastPoint.getDistanceFrom (newPoint) > 5.0f)
            {
                if (lastPoint != juce::Point<float>())
                {
                    juce::Path newSegment;
                    newSegment.startNewSubPath (lastPoint);
                    newSegment.lineTo (newPoint);

                    auto diameter = 20.0f * (pressure > 0 && pressure < 1.0f ? pressure : 1.0f);

                    juce::PathStrokeType (diameter, juce::PathStrokeType::curved, juce::PathStrokeType::rounded).createStrokedPath (newSegment, newSegment);
                    path.addPath (newSegment);
                }

                lastPoint = newPoint;
            }
        }

        juce::MouseInputSource source;
        juce::Path path;
        juce::Colour colour  { juce::Colours::red.withAlpha (0.6f) };
        juce::Point<float> lastPoint, currentPosition;
        juce::ModifierKeys modifierKeys;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Trail)
    };

protected:
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;
    void mouseDown (const juce::MouseEvent& e) override;

private:
    void drawTrail(Trail& trail, juce::Graphics& g);
    Trail* getTrail(const juce::MouseInputSource& source);

    juce::OwnedArray<Trail> m_trails;
    juce::Point<float> m_centerPos;
    juce::Rectangle<float> m_innerCircleRect;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpinButtonComponent)
};
