/*
  ==============================================================================

    SoundSource.cpp
    Created: 2 Mar 2026 1:49:52pm
    Author:  yutai

  ==============================================================================
*/

#include "SoundSource.h"
namespace YutaSound {

    void SoundSource::loadFromPath(const juce::String& filePath)
    {
        // allow juce to read WAV to the list of accecpted audio file format 
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        juce::File file = juce::File::getCurrentWorkingDirectory().getChildFile(filePath);

        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

        if (reader != nullptr)
        {
            //create buffer of size of the audio file  (number of channel,size)
            m_audioBuffer.setSize(1, (int)reader->lengthInSamples);
            //filing up the buffer 
            reader->read(&m_audioBuffer, 0, (int)reader->lengthInSamples, 0, true, false);
        }
        else
        {
            // Optional: Print a warning if the file isn't found so you aren't left guessing!
            std::cout << "Failed to load audio file: " << file.getFullPathName().toStdString() << std::endl;
        }
    }

    const juce::AudioBuffer<float>& SoundSource::getAudioBuffer() const
    {
        return m_audioBuffer;
    }
}