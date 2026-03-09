/*
  ==============================================================================

    SpatialAudioEngine.h
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "SoundSource.h"
#ifndef SPATIALAUDIOENGINE_H
#define SPATIALAUDIOENGINE_H

namespace YutaSound {

    class SpatialAudioEngine
    {
    public:
        void prepare(double sampleRate, int bufferSize);
        void addSource(const juce::String& filePath);
        //void runProcessingLoop();

    private:
        double m_sampleRate = 0.0;
        int m_bufferSize = 0;
        juce::AudioBuffer<float> m_mixBufferL;
        juce::AudioBuffer<float> m_mixBufferR;
        std::vector<SoundSource> m_sources;

    };

};

#endif

