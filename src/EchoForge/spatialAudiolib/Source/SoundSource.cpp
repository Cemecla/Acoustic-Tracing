/*
  ==============================================================================

    SoundSource.cpp
    Created: 2 Mar 2026 1:49:52pm
    Author:  yutai

  ==============================================================================
*/

#include "SoundSource.h"

namespace yutasound {

    void SoundSource::loadFromPath(const juce::String& filePath)
    {
        
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        juce::File file = juce::File::getCurrentWorkingDirectory().getChildFile(filePath);
        DBG("Trying to load: " + file.getFullPathName());
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

        if (reader != nullptr)
        {
            m_audioBuffer.setSize(1, (int)reader->lengthInSamples);
            reader->read(&m_audioBuffer, 0, (int)reader->lengthInSamples, 0, true, false);
            DBG("SUCCESS: Loaded WAV file! Samples: " + juce::String(reader->lengthInSamples));
        }
        else
        {
            DBG("Failed to load: " + file.getFullPathName());
        }
    }

    const juce::AudioBuffer<float>& SoundSource::getAudioBuffer() const
    {
        return m_audioBuffer;
    }
}