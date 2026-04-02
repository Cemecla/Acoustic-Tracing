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
//#include "SoundSource.h"
#include "AudioCallback.h"
#include "AudioManager.h"
#include "CircularStereoBuffer.h"

#ifndef SPATIALAUDIOENGINE_H
#define SPATIALAUDIOENGINE_H

namespace YutaSound {

    class SpatialAudioEngine : public juce::Thread
    {
    public:
        SpatialAudioEngine(AudioManager& audioManager);
        ~SpatialAudioEngine() override;

        void run() override;
        void prepare(double sampleRate, int bufferSize);
        void start();
        void stop();

    private:
        int m_bufferSize = 0;
        
        AudioManager m_audioManager;
        CircularStereoBuffer m_circularBuffer;
        AudioCallback m_audioCallback { m_circularBuffer };
        juce::AudioDeviceManager m_deviceManager;

    };

}

#endif

