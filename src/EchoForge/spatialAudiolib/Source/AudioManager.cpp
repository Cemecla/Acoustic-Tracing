/*
  ==============================================================================

    AudioManager.cpp
    Created: 30 Mar 2026 10:33:26am
    Author:  Administrateur

  ==============================================================================
*/

#include "AudioManager.h"

namespace yutasound {

    void AudioManager::prepare(double sampleRate, int bufferSize)
    {
        m_sampleRate = sampleRate;
    }

    void AudioManager::play() { m_isPlaying = true; }
    void AudioManager::stop() { m_isPlaying = false; }
    bool AudioManager::isPlaying() const { return m_isPlaying; }

    int AudioManager::registerSound(const juce::String& filePath)
    {
        return m_librarian.addItem(filePath);
    }

    void AudioManager::loadSoundRegistry(const juce::String& jsonPath)
    {
        m_librarian.loadSoundRegistry(jsonPath);
    }

    void AudioManager::saveSoundRegistry(const juce::String & jsonPath)
    {
        m_librarian.saveSoundRegistry(jsonPath);
    }

    void AudioManager::unloadSoundRegistry()
    {
        m_librarian.unloadSoundRegistry();
    }

    int AudioManager::playSound(int soundFileId, float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        SoundSource emitter;
        emitter.emitterId = m_nextEmitterId++;
        emitter.soundFileId = soundFileId;
        emitter.azimuth = azimuth;
        emitter.elevation = elevation;
        emitter.soundRemaining = soundRemaining;
        emitter.itdSamples = computeITD_(azimuth, elevation);
        emitter.ildGain = computeILD_(azimuth, elevation, soundRemaining);
        emitter.state = EmitterState::Playing;
           
        m_sources.push_back(std::move(emitter));

        if (onEmitterAdded) onEmitterAdded(m_nextEmitterId - 1);
        
        return m_nextEmitterId - 1;
    }

    void AudioManager::pauseSound(int emitterId)
    {
        for (auto& s : m_sources)
            if (s.emitterId == emitterId)
                s.state = EmitterState::Paused;
    }

    void AudioManager::resumeSound(int emitterId)
    {
        for (auto& s : m_sources)
            if (s.emitterId == emitterId)
                s.state = EmitterState::Playing;
    }

    void AudioManager::stopSound(int emitterId)
    {
        auto it = std::find_if(m_sources.begin(), m_sources.end(),
            [emitterId](const SoundSource& s) { return s.emitterId == emitterId; });

        if (it != m_sources.end())
        {
            int index = (int)std::distance(m_sources.begin(), it);
            m_sources.erase(it);
            if (onEmitterRemoved) onEmitterRemoved(index);
        }
    }

    void AudioManager::pauseAll()
    {
        for (auto& s : m_sources)
            s.state = EmitterState::Paused;
    }

    void AudioManager::resumeAll()
    {
        for (auto& s : m_sources)
            s.state = EmitterState::Playing;
    }

    void AudioManager::reset()
    {
        m_sources.clear();
        m_nextEmitterId = 0;
        DBG("AudioManager: reset all emitters");
    }

    void AudioManager::updateSoundSource(int emiterId, float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        // check if source exists
        for (auto& s : m_sources)
        {
            if (s.emitterId = emiterId)
            {
                s.azimuth = azimuth;
                s.elevation = elevation;
                s.soundRemaining = soundRemaining;
                s.itdSamples = computeITD_(azimuth, elevation);
                s.ildGain = computeILD_(azimuth, elevation, soundRemaining);
                break;
            }
        }
        // update shadowChain[id]
    }

    const std::vector<SoundSource>& AudioManager::getSources()const {
        return m_sources;
    }

    const juce::AudioBuffer<float>& AudioManager::getBuffer(int soundFileId) const
    {
        return m_librarian.getBuffer(soundFileId);
    }

    float AudioManager::computeITD_(float azimuth, float elevation)
    {
        constexpr float r = 0.0875f;
        constexpr float c = 343.0f;
        float azRad = azimuth * (juce::MathConstants<float>::pi / 180.0f);
        float elRad = elevation * (juce::MathConstants<float>::pi / 180.0f);

        float thetaEff = std::cos(elRad) * std::sin(azRad);
        float ITD = (r / c) * (std::asin(thetaEff) + thetaEff);
        return ITD * (float)m_sampleRate; // convertion into samples
    }

    ILDGain AudioManager::computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        float azRAD = azimuth * (juce::MathConstants<float>::pi / 180.0f);

        // aproximate calcule for testing - replace with Brown-Duda filter later
        // Reference: Brown & Duda 1998 "A Structural Model for Binaural Sound Synthesis"
        float baseILD = (1.0f + std::cos(azRAD)) / 2.0f;
        ILDGain gain;
        gain.low = baseILD * soundRemaining.lowRemaining;
        gain.mid = baseILD * soundRemaining.midRemaining;
        gain.high = baseILD * soundRemaining.highRemaining;

        return gain;
    }
}
