#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (400, 600);

    m_bpmLabel.setText(juce::String(m_bpm), juce::NotificationType::dontSendNotification);
    m_bpmLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(m_bpmLabel);

//    m_playToggleButton.setSize(getWidth(), getWidth());
    addAndMakeVisible(m_playToggleButton);

    m_playToggleButton.m_onValueChange = [this](int submit) {
        m_bpm += submit;
        m_bpmLabel.setText(juce::String(m_bpm), juce::NotificationType::dontSendNotification);
        m_bpmLabel.repaint();
        m_metronome.setBpm(m_bpm);
    };

    setAudioChannels(2, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
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
    m_metronome.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::releaseResources()
{
    //
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    m_metronome.getNextAudioBlock(bufferToFill);
}
