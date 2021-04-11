#include "SpinButtonComponent.h"
#include <juce_graphics/juce_graphics.h>

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
    g.drawText("ON", getLocalBounds(), juce::Justification::centred, true);

    for (auto* trail : m_trails)
        drawTrail (*trail, g);
}

void SpinButtonComponent::resized()
{
    m_centerPos = getLocalBounds().toFloat().getCentre();
    m_innerCircleRect = getLocalBounds().toFloat();
    m_innerCircleRect.reduce(getWidth() / 3.2, getHeight() / 3.2);
}

void SpinButtonComponent::mouseDrag(const juce::MouseEvent &e)
{
    Trail * t = getTrail (e.source);

    if (t) {
        t->pushPoint (e.position, e.mods, e.pressure);
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
        DBG("button pressed");
    } else {
        Trail * t = new Trail (e.source);
        t->path.startNewSubPath (e.position);
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
