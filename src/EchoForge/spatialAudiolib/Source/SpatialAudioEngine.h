/*
  ==============================================================================

    SpatialAudioEngine.h
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once
#define JUCE_MIDI_ENABLED 0
#include "JuceHeader.h"
#include "AudioCallback.h"
#include "AudioManager.h"
#include "CircularStereoBuffer.h"
#include "StereoDelayLine.h"
    
#ifndef SPATIALAUDIOENGINE_H
#define SPATIALAUDIOENGINE_H

namespace yutasound {

    class SpatialAudioEngine : public juce::Thread
    {
    public:
        SpatialAudioEngine(AudioManager& audioManager);
        ~SpatialAudioEngine() override;

        void run() override;
        void prepare(double sampleRate, int bufferSize);
        void addProcessingSlot();
        void start();
        void stop();

    private:
        int m_bufferSize = 0;
        double m_sampleRate = 0.0;
        
        AudioManager& m_audioManager;

        //std::vector<SoundSource>     m_sources;
        juce::AudioBuffer<float>     m_mixBuffer;
        std::vector<StereoDelayLine>  m_delayLines;
        std::vector<int> m_readPositions;

        // TODO: std::vector<FilterChain>      m_activeChains;
        // TODO: std::vector<FilterChain>      m_shadowChains;

        CircularStereoBuffer m_circularBuffer;
        AudioCallback m_audioCallback { m_circularBuffer };
        juce::AudioDeviceManager m_deviceManager;
    };

}
#endif