#include "SpinButtonComponent.h"
#include <juce_graphics/juce_graphics.h>

#include <math.h>

double radius(const PointF &origin, const PointF &start, const PointF &end)
{
    return atan2(end.getY() - origin.getY(), end.getX() - origin.getX()) -
           atan2(start.getY() - origin.getY(), start.getX() - origin.getX());
}

double angle(const PointF &origin, const PointF &start, const PointF &end)
{
    double a = radius(origin, start, end) / M_PI * 180.0f;
    int move = (a + (a < 0 ? -180 : 180)) / 360;
    return (fabs(a) > 180) ? (a - move * 360) : a;
//    return a > 180 ? a - 360 : a;
}

//==============================================================================
SpinButtonComponent::SpinButtonComponent()
{
}

//==============================================================================
void SpinButtonComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (juce::Colours::grey);

    g.setColour(juce::Colours::white);
    g.fillEllipse(getLocalBounds().toFloat());

    g.setColour(juce::Colours::lightgrey);
    g.fillEllipse(m_innerCircleRect);

    g.setFont(juce::Font(m_innerCircleRect.getWidth() / 3.2, juce::Font::bold));
    g.setColour(juce::Colours::black);
    g.drawText(m_isOn ? "OFF" : "ON", getLocalBounds(), juce::Justification::centred, true);

    for (auto* trail : m_trails)
        drawTrail (*trail, g);
}

void SpinButtonComponent::resized()
{
    m_centerPos = getLocalBounds().toFloat().getCentre();
    m_innerCircleRect = getLocalBounds().toFloat();
    m_innerCircleRect.reduce(getWidth() / 3.2, getHeight() / 3.2);
}

bool SpinButtonComponent::isOn()
{
    return m_isOn;
}

void SpinButtonComponent::mouseDrag(const juce::MouseEvent &e)
{
    Trail * t = getTrail (e.source);

    if (t) {
        int submit = t->pushPoint (e.position, e.mods, e.pressure);
        if (m_trails[0] == t) {
            m_onValueChange(submit);
        }
        repaint();
    }
}

void SpinButtonComponent::mouseUp(const juce::MouseEvent &e)
{
    m_trails.removeObject (getTrail (e.source));
    repaint();
}

void SpinButtonComponent::mouseDown(const juce::MouseEvent &e)
{
    if (e.position.getDistanceFrom(m_innerCircleRect.getCentre()) <= m_innerCircleRect.getWidth() / 2) {
        m_isOn = !m_isOn;
        m_onStateChange(m_isOn);
        repaint();
    } else {
        Trail * t = new Trail (e.source);
        t->startPoint(e.position, getLocalBounds().getCentre().toFloat());
        m_trails.add(t);
    }
}

void SpinButtonComponent::drawTrail(SpinButtonComponent::Trail &trail, juce::Graphics &g)
{
    g.setColour (trail.colour);
    g.fillPath (trail.path);

    auto radius = 40.0f;

    g.setColour (juce::Colours::black);
    g.drawEllipse (trail.currentPosition.x - radius,
                   trail.currentPosition.y - radius,
                   radius * 2.0f, radius * 2.0f, 2.0f);

    g.setFont (14.0f);

    juce::String desc ("Mouse #");
    desc << trail.source.getIndex();

    auto pressure = trail.source.getCurrentPressure();

    if (pressure > 0.0f && pressure < 1.0f)
        desc << "  (pressure: " << (int) (pressure * 100.0f) << "%)";

    if (trail.modifierKeys.isCommandDown()) desc << " (CMD)";
    if (trail.modifierKeys.isShiftDown())   desc << " (SHIFT)";
    if (trail.modifierKeys.isCtrlDown())    desc << " (CTRL)";
    if (trail.modifierKeys.isAltDown())     desc << " (ALT)";

    g.drawText (desc,
                juce::Rectangle<int> ((int) trail.currentPosition.x - 200,
                                (int) trail.currentPosition.y - 60,
                                400, 20),
                juce::Justification::centredTop, false);
}

SpinButtonComponent::Trail *SpinButtonComponent::getTrail(const juce::MouseInputSource &source)
{
    for (auto* trail : m_trails)
    {
        if (trail->source == source)
            return trail;
    }

    return nullptr;
}
