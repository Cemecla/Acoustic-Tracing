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
#include "SoundSource.h"
#include "AudioCallback.h"
#include "CircularStereoBuffer.h"

#ifndef SPATIALAUDIOENGINE_H
#define SPATIALAUDIOENGINE_H

namespace YutaSound {

    class SpatialAudioEngine : public juce::Thread
    {
    public:
        SpatialAudioEngine();
        ~SpatialAudioEngine() override;

        void run() override;

        void prepare(double sampleRate, int bufferSize);
        void addSource(const juce::String& filePath);
        void runProcessingLoop();
        void start();
        void stop();

    private:
        double m_sampleRate = 0.0;
        int m_bufferSize = 0;
        juce::AudioBuffer<float> m_mixBuffer;
        std::vector<SoundSource> m_sources;
        CircularStereoBuffer m_circularBuffer;
        AudioCallback m_audioCallback { m_circularBuffer };
        juce::AudioDeviceManager m_deviceManager;

    };

}

#endif

