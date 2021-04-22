#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

using PointF = juce::Point<float>;

//==============================================================================

double radius(const PointF &origin, const PointF &start, const PointF &end);

double angle(const PointF &origin, const PointF &start, const PointF &end);

class SpinButtonComponent : public juce::Component
{
public:
    //==============================================================================
    SpinButtonComponent();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    std::function<void(int)> m_onValueChange = [](int){};

    struct Trail
    {
        Trail (const juce::MouseInputSource& ms)
            : source (ms)
        {}

        void startPoint(const PointF & startPoint, const PointF & originPoint)
        {
            m_lastPoint = startPoint;
            m_originPoint = originPoint;

            path.startNewSubPath(startPoint);
        }

        int pushPoint (juce::Point<float> newPoint, juce::ModifierKeys newMods, float pressure)
        {
            // --- start ---
            int submit = 0;

            float a = angle(m_originPoint, m_lastPoint, newPoint);
            a += m_angleRemaining;
            if (fabs(a) > 10) {
                m_lastPoint = newPoint;
                submit = a / 10;
                m_angleRemaining = a - submit * 10;
                DBG("submit " << submit << " remaining " << m_angleRemaining);
            }
            // ---  end  ---

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

            // --- ***** ---
            return submit;
        }

        juce::MouseInputSource source;
        juce::Path path;
        juce::Colour colour  { juce::Colours::red.withAlpha (0.6f) };
        juce::Point<float> lastPoint, currentPosition;
        juce::ModifierKeys modifierKeys;

        float m_angleRemaining = 0;
        juce::Point<float> m_lastPoint;
        juce::Point<float> m_originPoint;

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
