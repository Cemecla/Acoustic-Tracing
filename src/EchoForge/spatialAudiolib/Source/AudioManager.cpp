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

        // send the current (default) preset to the engine so the EQ is active from the start
        if (m_pinnaCalibration.getCount() > 0)
            setPinnaPreset(m_currentPinnaPresetId);
    }

    void AudioManager::setPinnaPreset(int presetId)
    {
        m_currentPinnaPresetId = presetId;

        // the SAE applies it to the shadow chains, which get swapped in at the next block boundary
        const auto scope = m_commandFifo.write(1);
        if (scope.blockSize1 > 0)
        {
            m_commandBuffer[scope.startIndex1].type = EmitterCommand::Type::SetPinnaPreset;
            m_commandBuffer[scope.startIndex1].pinnaPreset = m_pinnaCalibration.getPreset(presetId);
        }
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
        return ITD * (float)m_sampleRate;
    }

    namespace {
        // Extra attenuation of the far ear (head shadow) in dB when the source is fully to the side.
        // Scaled by |sin(azimuth) * cos(elevation)|: 0 in front, behind and above, full at +-90 deg.
        // Rough values for a real head, tune by ear. The high band needs little extra because
        // SVFILDFilter already low-passes the far ear.
        constexpr float kFarEarLowDb  = 3.0f;   // < 300 Hz
        constexpr float kFarEarMidDb  = 7.0f;   // 300 Hz - 3 kHz
        constexpr float kFarEarHighDb = 0.0f;   // > 3 kHz
    }

    ILDGain AudioManager::computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        const float azRad = azimuth * (juce::MathConstants<float>::pi / 180.0f);
        const float elRad = elevation * (juce::MathConstants<float>::pi / 180.0f);

        // front/back level, same on both ears
        // Reference: Brown & Duda 1998 "A Structural Model for Binaural Sound Synthesis"
        // Scale to [0.2, 1.0] so rear sources (~180 deg) stay audible at ~-14 dB rather than going silent
        const float frontBack = 0.2f + 0.8f * (1.0f + std::cos(azRad)) / 2.0f;

        ILDGain gain;
        gain.lowL  = gain.lowR  = frontBack * soundRemaining.lowRemaining;
        gain.midL  = gain.midR  = frontBack * soundRemaining.midRemaining;
        gain.highL = gain.highR = frontBack * soundRemaining.highRemaining;

        // interaural level difference: only the far ear is turned down (the near ear is never
        // boosted, the mix has no limiter). lateral > 0: source on the right, the left ear is far
        const float lateral = std::cos(elRad) * std::sin(azRad);
        const float amount = std::abs(lateral);
        const float farLow  = juce::Decibels::decibelsToGain(-kFarEarLowDb  * amount);
        const float farMid  = juce::Decibels::decibelsToGain(-kFarEarMidDb  * amount);
        const float farHigh = juce::Decibels::decibelsToGain(-kFarEarHighDb * amount);

        if (lateral > 0.0f)
        {
            gain.lowL  *= farLow;
            gain.midL  *= farMid;
            gain.highL *= farHigh;
        }
        else
        {
            gain.lowR  *= farLow;
            gain.midR  *= farMid;
            gain.highR *= farHigh;
        }
        return gain;
    }
}
