/*
  ==============================================================================

    SpatialAudioEngine.h
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SoundSource.h"

class SpatialAudioEngine
{
public:
    void prepare(double sampleRate, int bufferSize);
    //void addSource(juce::AudioBuffer<float>& buffer);
     void addSource(const juce::String& filePath);

    //void runProcessingLoop();

private:
    juce::AudioBuffer<float> m_mixBufferL;
    juce::AudioBuffer<float> m_mixBufferR;
    std::vector<SoundSource> m_sources;

};


