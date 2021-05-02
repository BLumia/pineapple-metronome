#include "MainComponent.h"

#include "BinaryData.h"

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
        if (m_bpm + submit < 1 || m_bpm + submit > 2000) return;

        m_bpm += submit;
        m_bpmLabel.setText(juce::String(m_bpm), juce::NotificationType::dontSendNotification);
        m_bpmLabel.repaint();
        m_metronome.setBpm(m_bpm);
    };

    m_playToggleButton.m_onStateChange = [this](bool isOn) {
        if (isOn) {
            m_metronome.play();
        } else {
            m_metronome.stop();
        }
    };

    m_openGLContext.attachTo(*getTopLevelComponent());

    setAudioChannels(0, 2);

    juce::Desktop::setScreenSaverEnabled(false);

    if (juce::SystemStats::getUserLanguage() == "zh") {
#ifdef JUCE_WINDOWS
        juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Microsoft YaHei");
#endif // JUCE_WINDOWS
        juce::LocalisedStrings::setCurrentMappings(new juce::LocalisedStrings(
                juce::String::createStringFromData(BinaryData::translation_zh_CN_txt, BinaryData::translation_zh_CN_txtSize),
                false));
    }
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
