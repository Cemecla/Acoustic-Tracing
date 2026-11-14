/*
  ==============================================================================

    SpatialAudioEngine.cpp
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#include "SpatialAudioEngine.h"

namespace yutasound {

    SpatialAudioEngine::SpatialAudioEngine(AudioManager& audiomanager) 
        : juce::Thread("ProcessingThread"), m_audioManager(audiomanager)
    {}

    SpatialAudioEngine::~SpatialAudioEngine()
    {
        stop();
    }

    void SpatialAudioEngine::prepare(double sampleRate, int bufferSize)
    {
        m_sampleRate = sampleRate;
        m_bufferSize = bufferSize;
        m_mixBuffer.setSize(2, bufferSize);
        m_circularBuffer.prepare(bufferSize * 4 + 1);
        for (auto& delayline : m_delayLines)
            delayline.prepare(m_sampleRate, m_bufferSize);
        m_masterBus.prepare(sampleRate, bufferSize);
    }

    void SpatialAudioEngine::addProcessingSlot()
    {
        StereoDelayLine dl;
        dl.prepare(m_sampleRate, m_bufferSize);
        m_delayLines.push_back(std::move(dl));

        m_readPositions.push_back(0);
        
        juce::SmoothedValue<float> smoothed;
        smoothed.reset(m_sampleRate, 0.05);
        m_smoothedITD.push_back(std::move(smoothed));

        juce::SmoothedValue<float> reverbSend;
        reverbSend.reset(m_sampleRate, 0.2);
        m_smoothedReverbSend.push_back(std::move(reverbSend));

        // new emitters start with the current pinna preset on both chains (no swap needed)
        FilterChain active;
        active.prepare(m_sampleRate, m_bufferSize);
        active.setPinnaEQ(m_pinnaPreset);
        active.clearDirty();
        m_activeChains.push_back(std::move(active));

        FilterChain shadow;
        shadow.prepare(m_sampleRate, m_bufferSize);
        shadow.setPinnaEQ(m_pinnaPreset);
        shadow.clearDirty();
        m_shadowChains.push_back(std::move(shadow));
    }

    void SpatialAudioEngine::removeProcessingSlot(int index)
    {
        m_delayLines.erase(m_delayLines.begin() + index);
        m_readPositions.erase(m_readPositions.begin() + index);
        m_smoothedITD.erase(m_smoothedITD.begin() + index);
        m_smoothedReverbSend.erase(m_smoothedReverbSend.begin() + index);
        m_activeChains.erase(m_activeChains.begin() + index);
        m_shadowChains.erase(m_shadowChains.begin() + index);
    }

    void SpatialAudioEngine::run()
    {
        while (!threadShouldExit())
        {
            if (!m_audioManager.isPlaying())
            {
                wait(1);
                continue;
            }

            if (m_circularBuffer.getFreeSpace() >= m_bufferSize)
            {
                drainCommandQueue_();

                for (int i = 0; i < (int)m_sources.size(); i++)
                {
                    // shadow to active swap
                    // the shadow already has the same azimuth and gains as the active chain
                    // (UpdateSpatial writes to both), only its filter memory is stale
                    if (m_shadowChains[i].isDirty())
                    {
                        m_shadowChains[i].resetState();
                        m_shadowChains[i].warmUp();
                        std::swap(m_activeChains[i], m_shadowChains[i]);
                        m_activeChains[i].clearDirty();

                        // the old active chain is now the shadow: give it the new preset too,
                        // so both chains stay identical until the next change
                        m_shadowChains[i].setPinnaEQ(m_pinnaPreset);
                        m_shadowChains[i].clearDirty();
                    }
                }

                m_mixBuffer.clear();

                for (int i = 0; i < (int)m_sources.size(); i++)
                {
                    auto& source = m_sources[i];

                    if (source.state != EmitterState::Playing &&
                        source.state != EmitterState::Looping)
                        continue;

                    const auto& buffer = m_audioManager.getBuffer(source.soundFileId);

                    auto& delayLine = m_delayLines[i];
                    int& readPos = m_readPositions[i];

                    auto samplesAvailable = buffer.getNumSamples() - readPos;
                    auto samplesThisBlock = juce::jmin(m_bufferSize, samplesAvailable);

                    m_smoothedITD[i].setTargetValue(source.itdSamples);
                    m_smoothedReverbSend[i].setTargetValue(source.reverbSend);
                    float* reverbSend = m_masterBus.getSendWritePointer();

                    for (int n = 0; n < samplesThisBlock; n++)
                    {
                        float monoSample = buffer.getSample(0, readPos + n);
                        float smoothed = m_smoothedITD[i].getNextValue();
                        auto stereo = delayLine.process(monoSample, smoothed);
                        auto filtered = m_activeChains[i].processSample(stereo.left, stereo.right);

                        *m_mixBuffer.getWritePointer(0, n) += filtered.left;
                        *m_mixBuffer.getWritePointer(1, n) += filtered.right;

                        // dry mono signal to the room reverb (diffuse: no direction)
                        reverbSend[n] += monoSample * m_smoothedReverbSend[i].getNextValue();
                    }

                    readPos += samplesThisBlock;
                    if (readPos >= buffer.getNumSamples())
                    {
                        readPos = 0;
                        if (source.state == EmitterState::Looping || source.isAmbient)
                        {
                        }
                        else
                        {
                            source.state = EmitterState::Paused;
                        }
                    }
                }

                // room reverb + safety limiter on the whole mix
                m_masterBus.process(m_mixBuffer, m_bufferSize);

                m_circularBuffer.write(
                    m_mixBuffer.getWritePointer(0),
                    m_mixBuffer.getWritePointer(1),
                    m_bufferSize
                );
            }
            else
            {
                wait(1);
            }
        }
    }

    void SpatialAudioEngine::start()
    {   
        auto error = m_deviceManager.initialiseWithDefaultDevices(0, 2);
        if (error.isNotEmpty())
            DBG("AudioDeviceManager error: " + error);
        else
            DBG("AudioDeviceManager initialized OK");
        m_deviceManager.addAudioCallback(&m_audioCallback);
        DBG("AudioCallback added");

        startThread();
    }

    void SpatialAudioEngine::stop()
    {
        stopThread(2000); // wait max 2 seconds for thread to finish
        m_deviceManager.removeAudioCallback(&m_audioCallback);
    }
    void SpatialAudioEngine::drainCommandQueue_()
    {
        while(m_audioManager.hasCommands())
        {
            auto cmd = m_audioManager.popCommand();
            if (cmd.type == EmitterCommand::Type::Add)
            {
                m_sources.push_back(cmd.emitter);
                addProcessingSlot();
            }
            else if (cmd.type == EmitterCommand::Type::Remove)
            {
                for (int i = 0; i < (int)m_sources.size(); i++)
                {
                    if (m_sources[i].emitterId == cmd.emitterId)
                    {
                        removeProcessingSlot(i);
                        m_sources.erase(m_sources.begin() + i);
                        break;
                    }
                }
            }
            else if (cmd.type == EmitterCommand::Type::SetState)
            {
                for (auto& s : m_sources)
                    if (s.emitterId == cmd.emitterId)
                        s.state = cmd.newState;
            }
            else if (cmd.type == EmitterCommand::Type::UpdateSpatial)
            {
                for (int i = 0; i < (int)m_sources.size(); i++)
                    if (m_sources[i].emitterId == cmd.emitterId)
                    {
                        m_sources[i].itdSamples = cmd.itdSamples;
                        m_sources[i].ildGain = cmd.ildGain;
                        m_sources[i].azimuth = cmd.azimuth;
                        m_sources[i].elevation = cmd.elevation;
                        m_sources[i].reverbSend = cmd.reverbSend;
                        // SVF + crossover gains: direct update, no swap. The shadow is kept in
                        // sync so it is correct when it gets swapped in after a pinna change
                        m_activeChains[i].setAzimuth(cmd.azimuth);
                        m_shadowChains[i].setAzimuth(cmd.azimuth);
                        m_activeChains[i].setGains(cmd.ildGain);
                        m_shadowChains[i].setGains(cmd.ildGain);
                        break;
                    }
            }
            else if (cmd.type == EmitterCommand::Type::SetPinnaPreset)
            {
                // coefficients go to the shadow chains (marks them dirty);
                // run() swaps them in at the start of the block
                m_pinnaPreset = cmd.pinnaPreset;
                for (auto& shadow : m_shadowChains)
                    shadow.setPinnaEQ(m_pinnaPreset);
            }
            else if (cmd.type == EmitterCommand::Type::SetReverbSettings)
            {
                m_masterBus.setReverbSettings(cmd.reverbSettings);
            }
            else if (cmd.type == EmitterCommand::Type::Reset)
            {
                m_sources.clear();
                m_delayLines.clear();
                m_readPositions.clear();
                m_smoothedITD.clear();
                m_smoothedReverbSend.clear();
                m_masterBus.reset(); // drop the reverb tail of the old scene
                m_activeChains.clear();
                m_shadowChains.clear();
                DBG("SAE: reset all emitters");
            }
        }
    }
}