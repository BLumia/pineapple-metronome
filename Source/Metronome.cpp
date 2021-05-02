#include "Metronome.h"

#include "BinaryData.h"

#include <memory>

Metronome::Metronome()
{
    m_formatManager.registerBasicFormats();

    std::unique_ptr<juce::MemoryInputStream> inputStream {
        new juce::MemoryInputStream(BinaryData::Snare_wav, BinaryData::Snare_wavSize, true)
    };

//    std::unique_ptr<juce::AudioFormatReader> reader {
//        m_formatManager.createReaderFor(std::move(inputStream))
//    };
    juce::AudioFormatReader * reader = m_formatManager.createReaderFor(std::move(inputStream));
    m_metronomeSample.reset(new juce::AudioFormatReaderSource(reader, true));

    m_metronomeSample->setNextReadPosition(0);
}

void Metronome::setBpm(int bpm)
{
    m_bpm = bpm;
    tryEnsureSamplePerBeat();
}

void Metronome::play()
{
    m_isPlaying = true;
    tryEnsureSamplePerBeat();
}

void Metronome::stop()
{
    m_isPlaying = false;
    m_metronomeSample->setNextReadPosition(0);
    m_sampleRemain = 0;
}

void Metronome::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    m_sampleRate = sampleRate;
    if (m_metronomeSample != nullptr) {
        m_metronomeSample->prepareToPlay(samplesPerBlockExpected, sampleRate);
        DBG("loaded");
    }
    DBG("prepare");
    m_metronomeSample->setNextReadPosition(0);
    tryEnsureSamplePerBeat();
}

void Metronome::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    if (!m_isPlaying) return;
//    DBG(m_metronomeSample->getTotalLength() << " " << bufferToFill.numSamples << " " << m_sampleRemain);
    if (m_sampleRemain > m_samplePerBeat) {
//        DBG("should only happen after adjust bpm from slow to fast");
        m_sampleRemain = m_samplePerBeat;
    }

    if (m_sampleRemain < bufferToFill.numSamples) {
        m_metronomeSample->setNextReadPosition(-m_sampleRemain);
        m_metronomeSample->getNextAudioBlock(bufferToFill);
        m_sampleRemain += m_samplePerBeat;
    } else if (m_metronomeSample->getNextReadPosition() != 0) {
        m_metronomeSample->getNextAudioBlock(bufferToFill);
    }

    m_sampleRemain -= bufferToFill.numSamples;
}

bool Metronome::tryEnsureSamplePerBeat()
{
    if (m_sampleRate < 0 || m_bpm <= 0) return false;
    m_samplePerBeat = 60.f / m_bpm * m_sampleRate;
    return true;
}
