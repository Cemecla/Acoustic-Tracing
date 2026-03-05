/*
  ==============================================================================

    SpatialAudioEngine.cpp
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#include "SpatialAudioEngine.h"


void SpatialAudioEngine::prepare(double sampleRate, int bufferSize) {
    double _sampleRate = sampleRate;
    int _bufferSize = bufferSize;
}

void SpatialAudioEngine::addSource(const juce::String& filePath)
{
    SoundSource source;
    source.loadFromPath(filePath);
    m_sources.push_back(source);
}
