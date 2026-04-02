/*
  ==============================================================================

    AudioManager.h
    Created: 30 Mar 2026 10:33:26am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SoundSource.h"
//#include "SoundRemaining.h"

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

namespace YutaSound {

    class CircularStereoBuffer;

    class AudioManager
    {
    public:
        void prepare(double sampleRate, int bufferSize);
        void addSource(const juce::String& filePath);
        //void updateSource(const juce::String& md5Name, float azimuth, float elevation, SoundRemaining soundRemaining);
        void runProcessingLoop(CircularStereoBuffer& circularBuffer);

    private:
        std::vector<SoundSource> m_sources;
        juce::AudioBuffer<float> m_mixBuffer;
        double m_sampleRate = 0.0;
        int m_bufferSize = 0;

        float computeITD_(float azimuth, float elevation);
        float computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining);

    };
}
#endif

