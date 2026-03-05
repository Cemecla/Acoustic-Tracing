/*
  ==============================================================================

    SoundSource.cpp
    Created: 2 Mar 2026 1:49:52pm
    Author:  yutai

  ==============================================================================
*/

#include "SoundSource.h"

void SoundSource::loadFromPath(const juce::String& filePath) 
{   
    // allow juce to read WAV to the list of accecpted audio file format 
    m_formatManager.registerBasicFormats();
    juce::File file(filePath);
    //not sure redundency maybe its creating itself 2 time  in h?
    auto* _reader(m_formatManager.createReaderFor(filePath));

    if (_reader != nullptr)
    {
        //create buffer of size of the audio file  (number of channel,size)
        m_audioBuffer.setSize((int)1, (int)_reader->lengthInSamples);
        //filing up the buffer 
        _reader->read (&m_audioBuffer, 0, _reader->lengthInSamples, m_readPosition,true,false);
        //delete the reader no more need but again i beleave it get self deleat because of the unique_ptr in the h
        delete _reader;
    }
}

const juce::AudioBuffer<float>& SoundSource::getAudioBuffer() const
{
    return m_audioBuffer;
}
