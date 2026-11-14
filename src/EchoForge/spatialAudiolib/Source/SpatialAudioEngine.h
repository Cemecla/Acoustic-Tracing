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
#include "FilterChain.h"
    
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
        void removeProcessingSlot(int index);
        void start();
        void stop();

    private:
        void drainCommandQueue_();

        int m_bufferSize = 0;
        double m_sampleRate = 0.0;
        
        AudioManager& m_audioManager;

        std::vector<SoundSource>     m_sources;
        juce::AudioBuffer<float>     m_mixBuffer;
        std::vector<StereoDelayLine>  m_delayLines;
        std::vector<int> m_readPositions;
        // to atenuate the pop when azimuth jump from 180 deg to -180 deg
        std::vector<juce::SmoothedValue<float>> m_smoothedITD;

        std::vector<FilterChain>      m_activeChains;
        std::vector<FilterChain>      m_shadowChains;
        // pinna preset currently applied to every chain (set by EmitterCommand::Type::SetPinnaPreset)
        PinnaPreset                   m_pinnaPreset;

        CircularStereoBuffer m_circularBuffer;
        AudioCallback m_audioCallback { m_circularBuffer };
        juce::AudioDeviceManager m_deviceManager;
    };

}
#endif