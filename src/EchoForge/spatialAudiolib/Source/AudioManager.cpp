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
    const std::vector<SoundSource>& AudioManager::getSources()const {
        return m_sources; // TODO: thread safety - main thread writes, SAE thread reads
    }

    void AudioManager::loadSource_(int id)
    {
        SoundSource source;
        source.id = id;
        source.loadFromPath("../../../../Acoustic-Tracing/assets/mooo.wav");
        m_sources.push_back(std::move(source));

    }

    void AudioManager::updateSoundSource(int id, float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        // check if source exists
        

        auto it = std::find_if(m_sources.begin(), m_sources.end(),
            [id](const SoundSource& s) { return s.id == id; });
        // if not → loadSource_(id)
        
        if (it == m_sources.end())
        {
            // NEW source → lazy load
            loadSource_(id);

            if(onSourceAdded) onSourceAdded(id);
            
            it = std::find_if(m_sources.begin(), m_sources.end(),
                [id](const SoundSource& s) { return s.id == id; });
        }

        it->azimuth = azimuth;
        it->elevation = elevation;
        it->soundRemaining = soundRemaining;
        it->itdSamples = computeITD_(azimuth, elevation);
        // then compute ITD/ILD
        // update shadowChain[id]
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

    float AudioManager::computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        // TODO: implement ILD
        return 1.0f;
    }
}
