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

    int AudioManager::registerAmbient(const juce::String& filePath)
    {
        // find next available ambient id (0-9)
        int nextId = -1;
        const auto& entries = m_librarian.getAmbientEntries();
        for (int i = ReservedIds::AMBIENT_MIN; i <= ReservedIds::AMBIENT_MAX; i++)
        {
            bool taken = false;
            for (const auto& e : entries)
                if (e.id == i) { taken = true; break; }
            if (!taken) { nextId = i; break; }
        }

        if (nextId == -1)
        {
            DBG("AudioManager: no ambient slots available (max 10)");
            return -1;
        }

        m_librarian.addAmbientItem(nextId, "ambient", filePath, 1.0f);
        DBG("AudioManager: registered ambient id=" + juce::String(nextId));
        return nextId;
    }
    
    void AudioManager::removeSound(int id)
    {
        m_librarian.removeItem(id);
    }

    void AudioManager::loadSoundRegistry(const juce::String& jsonPath)
    {
        m_librarian.loadSoundRegistry(jsonPath);

        for (const auto& ambient : m_librarian.getAmbientEntries())
        {
            SoundSource emitter;
            emitter.emitterId = ambient.id;
            emitter.soundFileId = ambient.id;
            emitter.state = EmitterState::Playing;
            emitter.isAmbient = true;
            emitter.itdSamples = 0.0f;
            emitter.ildGain = ILDGain();

            const auto scope = m_commandFifo.write(1);
            if (scope.blockSize1 > 0)
            {
                m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::Add;
                m_commandBuffer[scope.startIndex1].emitter = emitter;
                m_commandBuffer[scope.startIndex1].emitterId = emitter.emitterId;
            }
        }
    }

    void AudioManager::saveSoundRegistry(const juce::String & jsonPath)
    {
        m_librarian.saveSoundRegistry(jsonPath);
    }

    void AudioManager::unloadSoundRegistry()
    {
        for (int id = ReservedIds::AMBIENT_MIN; id <= ReservedIds::AMBIENT_MAX; id++)
            stopSound(id);
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
        emitter.state = EmitterState::Paused;
        emitter.isAmbient = ReservedIds::isAmbient(soundFileId);
           
        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
        {
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::Add;
            m_commandBuffer[scope.startIndex1].emitter = emitter;
            m_commandBuffer[scope.startIndex1].emitterId = emitter.emitterId;
        }

        return  emitter.emitterId;
    }

    void AudioManager::pauseSound(int emitterId)
    {
        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
        {
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetState;
            m_commandBuffer[scope.startIndex1].emitterId = emitterId;
            m_commandBuffer[scope.startIndex1].newState = EmitterState::Paused;
        }
    }

    void AudioManager::resumeSound(int emitterId)
    {
        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
        {
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetState;
            m_commandBuffer[scope.startIndex1].emitterId = emitterId;
            m_commandBuffer[scope.startIndex1].newState = EmitterState::Playing;
        }
    }

    void AudioManager::loopSound(int emitterId)
    {
        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
        {
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetState;
            m_commandBuffer[scope.startIndex1].emitterId = emitterId;
            m_commandBuffer[scope.startIndex1].newState = EmitterState::Looping;
        }
    }

    void AudioManager::stopSound(int emitterId)
    {
        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
        {
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::Remove;
            m_commandBuffer[scope.startIndex1].emitterId = emitterId;
        }
    }

    void AudioManager::pauseAll()
    {
        DBG("AudioManager: pauseAll sending " + juce::String(m_nextEmitterId) + " commands");
        // pause ambient emitters (ids 0 - 9)
        for (int i = ReservedIds::AMBIENT_MIN; i <= ReservedIds::AMBIENT_MAX; i++)
        {
            const auto scope = m_commandFifo.write(1);
            if (scope.blockSize1 > 0)
            {
                m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetState;
                m_commandBuffer[scope.startIndex1].emitterId = i;
                m_commandBuffer[scope.startIndex1].newState = EmitterState::Paused;
            }
        }

        // pause dynamic emitters (ids 10+)
        for (int i = ReservedIds::DYNAMIC_START; i < m_nextEmitterId; i++)
        {
            const auto scope = m_commandFifo.write(1);
            if (scope.blockSize1 > 0)
            {
                m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetState;
                m_commandBuffer[scope.startIndex1].emitterId = i;
                m_commandBuffer[scope.startIndex1].newState = EmitterState::Paused;
            }
        }
    }

    void AudioManager::resumeAll()
    {
        // resume ambient emitters (ids 0 - 9)
        for (int i = ReservedIds::AMBIENT_MIN; i <= ReservedIds::AMBIENT_MAX; i++)
        {
            const auto scope = m_commandFifo.write(1);
            if (scope.blockSize1 > 0)
            {
                m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetState;
                m_commandBuffer[scope.startIndex1].emitterId = i;
                m_commandBuffer[scope.startIndex1].newState = EmitterState::Playing;
            }
        }
        // resume dynamic emitters (ids 10+)
        for (int i = ReservedIds::DYNAMIC_START; i < m_nextEmitterId; i++)
        {
            const auto scope = m_commandFifo.write(1);
            if (scope.blockSize1 > 0)
            {
                m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetState;
                m_commandBuffer[scope.startIndex1].emitterId = i;
                m_commandBuffer[scope.startIndex1].newState = EmitterState::Playing;
            }
        }
    }

    void AudioManager::reset()
    {
        m_nextEmitterId = ReservedIds::DYNAMIC_START;

        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::Reset;

        DBG("AudioManager: reset all emitters");
    }

    bool AudioManager::hasCommands() const
    {
        return m_commandFifo.getNumReady() > 0;
    }

    EmitterCommand AudioManager::popCommand()
    {
        const auto scope = m_commandFifo.read(1);
        return m_commandBuffer[scope.startIndex1];
    }

    void AudioManager::updateSoundSource(int emitterId, float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
        {
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::UpdateSpatial;
            m_commandBuffer[scope.startIndex1].emitterId = emitterId;
            m_commandBuffer[scope.startIndex1].azimuth = azimuth;
            m_commandBuffer[scope.startIndex1].elevation = elevation;
            m_commandBuffer[scope.startIndex1].itdSamples = computeITD_(azimuth, elevation);
            m_commandBuffer[scope.startIndex1].ildGain = computeILD_(azimuth, elevation, soundRemaining);
        }
    }

    void AudioManager::loadPinnaPresets(const juce::String& path)
    {
        m_pinnaCalibration.loadFromJson(path);
        DBG("AudioManager: pinna presets loaded");
    }

    void AudioManager::setPinnaPreset(int presetId)
    {
        m_currentPinnaPresetId = presetId;
        DBG("AudioManager: pinna preset set to " + juce::String(presetId));
    }

    int AudioManager::getPinnaPreset() const
    {
        return m_currentPinnaPresetId;
    }

    const PinnaPreset& AudioManager::getCurrentPinnaPreset() const
    {
        return m_pinnaCalibration.getPreset(m_currentPinnaPresetId);
    }

    int AudioManager::getPinnaPresetCount() const
    {
        return m_pinnaCalibration.getCount();
    }

    const std::vector<SoundFileEntry>& AudioManager::getEntries() const
    {
        return m_librarian.getEntries();
    }

    //const std::vector<SoundSource>& AudioManager::getSources()const {
    //    return m_sources;
    //}

    const juce::AudioBuffer<float>& AudioManager::getBuffer(int soundFileId) const
    {
        if (ReservedIds::isAmbient(soundFileId))
        {
            for (const auto& ambient : m_librarian.getAmbientEntries())
                if (ambient.id == soundFileId)
                    return ambient.buffer;
        }
        return m_librarian.getBuffer(soundFileId);
    }

    std::vector<AmbientEntry>& AudioManager::getAmbientEntries() 
    {
        return m_librarian.getAmbientEntries();
    }

    float AudioManager::getAmbientGain(int ambientId) const
    {
        for (const auto& ambient : m_librarian.getAmbientEntries())
            if (ambient.id == ambientId)
                return ambient.gain;
        return 1.0f;
    }

    void AudioManager::setAmbientGain(int ambientId, float gain)
    {
        for (auto& ambient : m_librarian.getAmbientEntries())
            if (ambient.id == ambientId)
            {
                ambient.gain = gain;
                return;
            }
        DBG("AudioManager: ambient id=" + juce::String(ambientId) + " not found");
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
