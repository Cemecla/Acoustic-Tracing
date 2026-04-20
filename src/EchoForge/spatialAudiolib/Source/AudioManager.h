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

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

namespace yutasound {

    class AudioManager
    {
    public:
        void prepare(double sampleRate, int bufferSize);
        void updateSoundSource(int id, float azimuth, float elevation, SoundRemaining soundRemaining);
        
        // TODO: void compute_(Listener, SoundSource, SpatialFilterChain& shadow);
        // TODO: void updateListener(position, azimuth, elevation);
        void play();
        void stop();
        bool isPlaying() const;

        // test callback strategy
        std::function<void(int)> onSourceAdded;
        const std::vector<SoundSource>& getSources() const;
    private:
        void loadSource_(int id);
        
        std::vector<SoundSource>  m_sources;
        double m_sampleRate = 0.0;

        std::atomic<bool> m_isPlaying {false};

        float computeITD_(float azimuth, float elevation);
        ILDGain computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining);


    };
}
#endif

