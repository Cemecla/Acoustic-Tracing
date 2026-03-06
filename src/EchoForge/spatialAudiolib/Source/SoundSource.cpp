/*
  ==============================================================================

    SoundSource.cpp
    Created: 2 Mar 2026 1:49:52pm
    Author:  yutai

  ==============================================================================
*/

#include "SoundSource.h"
using namespace YutaSound;

void SoundSource::loadFromPath(const juce::String& filePath) 
{   
    // allow juce to read WAV to the list of accecpted audio file format 
    juce::AudioFormatManager formatManager; 
    formatManager.registerBasicFormats();

    juce::File file(filePath);
    
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader != nullptr)
    {
        //create buffer of size of the audio file  (number of channel,size)
        m_audioBuffer.setSize(1, (int)reader->lengthInSamples);
        //filing up the buffer 
        reader->read (&m_audioBuffer, 0, (int)reader->lengthInSamples, readPosition,true,false);
        
    }
}

const juce::AudioBuffer<float>& SoundSource::getAudioBuffer() const
{
    return m_audioBuffer;
}
