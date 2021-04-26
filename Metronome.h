#pragma once

#include <juce_audio_formats/juce_audio_formats.h>

class Metronome
{
public:
    Metronome();

    void setBpm(int bpm);
    void play();
    void stop();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill);

private:
    juce::AudioFormatManager m_formatManager;
    std::unique_ptr <juce::AudioFormatReaderSource> m_metronomeSample { nullptr };

    bool tryEnsureSamplePerBeat();

    int64_t m_sampleRemain = 0;
    int32_t m_bpm = 120;
    int64_t m_samplePerBeat = -1;
    double m_sampleRate = -1;
    bool m_isPlaying = false;
};
