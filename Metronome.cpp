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

void Metronome::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    if (m_metronomeSample != nullptr) {
        m_metronomeSample->prepareToPlay(samplesPerBlockExpected, sampleRate);
        DBG("loaded");
    }
    DBG("prepare");
    m_metronomeSample->setNextReadPosition(0);
}

void Metronome::getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill)
{
    if (m_metronomeSample.get() == nullptr) return;

    DBG(m_metronomeSample->getTotalLength() << " " << bufferToFill.numSamples << " " << m_sampleRemain);
    if (m_sampleRemain < bufferToFill.numSamples) {
        m_metronomeSample->setNextReadPosition(0);
        m_metronomeSample->getNextAudioBlock(bufferToFill);
        m_sampleRemain += 30000;
    } else if (m_metronomeSample->getNextReadPosition() != 0) {
        m_metronomeSample->getNextAudioBlock(bufferToFill);
    }

    m_sampleRemain -= bufferToFill.numSamples;
}
