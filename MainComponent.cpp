#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (400, 600);

    m_bpmLabel.setText("120", juce::NotificationType::dontSendNotification);
    m_bpmLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(m_bpmLabel);

//    m_playToggleButton.setSize(getWidth(), getWidth());
    addAndMakeVisible(m_playToggleButton);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

//    g.setFont (juce::Font (16.0f));
//    g.setColour (juce::Colours::white);
//    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    const juce::Font & bpmFont = juce::Font(getWidth() / 4.5);
    m_bpmLabel.setFont(bpmFont);

    juce::FlexBox fb;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    fb.flexDirection = juce::FlexBox::Direction::column;

    fb.items.add(juce::FlexItem(m_bpmLabel)
                        .withMinWidth(getWidth())
                        .withMinHeight(1.4 * bpmFont.getHeight()));
    fb.items.add(juce::FlexItem(m_playToggleButton)
                        .withMinWidth(getWidth())
                        .withMinHeight(getWidth()));

    fb.performLayout(getLocalBounds());
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    juce::ignoreUnused(samplesPerBlockExpected);
    juce::ignoreUnused(sampleRate);
}

void MainComponent::releaseResources()
{
    //
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    juce::ignoreUnused(bufferToFill);
    DBG("A");
}
