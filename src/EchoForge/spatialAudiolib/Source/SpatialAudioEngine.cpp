/*
  ==============================================================================

    SpatialAudioEngine.cpp
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#include "SpatialAudioEngine.h"
#include "SoundSource.h"
using namespace YutaSound;

void SpatialAudioEngine::prepare(double sampleRate, int bufferSize) {
    m_sampleRate = sampleRate;
    m_bufferSize = bufferSize;
}

void SpatialAudioEngine::addSource(const juce::String& filePath)
{
    SoundSource source;
    source.loadFromPath(filePath);
    m_sources.push_back(std::move(source));
}

void YutaSound::SpatialAudioEngine::runProcessingLoop()
{
    m_mixBuffer.clear();

    for (auto& source : m_sources) 
    {
        
    }
}
