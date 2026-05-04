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
#include "Librarian.h"
#include "EmitterState.h"
#include "EmitterCommand.h"
#include <functional>


#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

namespace yutasound {

    class AudioManager
    {
    public:
        void prepare(double sampleRate, int bufferSize);
        void play();
        void stop();
        bool isPlaying() const;

        // Librarian — sound registration
        const std::vector<SoundFileEntry>& getEntries() const;
        int  registerSound(const juce::String& filePath);
        void loadSoundRegistry(const juce::String& jsonPath);
        void saveSoundRegistry(const juce::String& jsonPath);
        void unloadSoundRegistry();

        // emitter management
        int  playSound(int soundFileId, float azimuth, float elevation, SoundRemaining soundRemaining);
        void pauseSound(int emitterId);
        void resumeSound(int emitterId);
        void stopSound(int emitterId);
        void pauseAll();
        void resumeAll();
        void reset();

        // emitter race condition to protect 
        int           hasCommands() const;
        EmitterCommand popCommand();

        void updateSoundSource(int emitterId, float azimuth, float elevation, SoundRemaining soundRemaining);
        
        // TODO: void compute_(Listener, SoundSource, SpatialFilterChain& shadow);
        // TODO: void updateListener(position, azimuth, elevation);
        //std::function<void(int)> onSourceAdded;
         
        //SAE access
        const std::vector<SoundSource>& getSources() const;
        const juce::AudioBuffer<float>& getBuffer(int soundFileId) const;

        //callback for the SAE
        std::function<void(int)> onEmitterAdded;
        std::function<void(int)> onEmitterRemoved;

    private:
        //void loadSource_(int id);
        
        Librarian m_librarian;
        std::vector<SoundSource>  m_sources; // emitters
        double m_sampleRate = 0.0;
        int m_nextEmitterId = 0;
        std::atomic<bool> m_isPlaying {false};

        float computeITD_(float azimuth, float elevation);
        ILDGain computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining);

        juce::AbstractFifo                     m_commandFifo{ 64 };
        std::array<EmitterCommand, 64>         m_commandBuffer;


    };
}
#endif

