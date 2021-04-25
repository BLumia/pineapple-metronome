#pragma once

#include <juce_audio_formats/juce_audio_formats.h>

class Metronome
{
public:
    Metronome();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill);

private:
    juce::AudioFormatManager m_formatManager;
    std::unique_ptr <juce::AudioFormatReaderSource> m_metronomeSample { nullptr };

    int64_t m_sampleRemain = 0;
};
