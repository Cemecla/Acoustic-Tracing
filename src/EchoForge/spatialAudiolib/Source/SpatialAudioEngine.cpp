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
        // 2. Safely stop the thread before the engine is destroyed!
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

        // active filter chain 
        FilterChain active;
        active.prepare(m_sampleRate, m_bufferSize);
        m_activeChains.push_back(std::move(active));

        // chadow filter chain 
        FilterChain shadow;
        shadow.prepare(m_sampleRate, m_bufferSize);
        m_shadowChains.push_back(std::move(shadow));
    }

    void SpatialAudioEngine::removeProcessingSlot(int index)
    {
        m_delayLines.erase(m_delayLines.begin() + index);
        m_readPositions.erase(m_readPositions.begin() + index);
        m_smoothedITD.erase(m_smoothedITD.begin() + index);
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
                // drain command queue every block
                drainCommandQueue_();

                for (int i = 0; i < (int)m_sources.size(); i++)
                {
                    // shadow to active swap
                    if (m_shadowChains[i].isDirty())
                    {
                        m_shadowChains[i].resetState();
                        m_shadowChains[i].warmUp();
                        std::swap(m_activeChains[i], m_shadowChains[i]);
                        m_activeChains[i].clearDirty();
                    }
                }

                //const auto& currentPreset = m_audioManager.getCurrentPinnaPreset();
                //for (int i = 0; i < (int)m_sources.size(); i++)
                //{
                //    if (!m_sources[i].isAmbient)
                //        m_shadowChains[i].setParameters(
                //            m_sources[i].azimuth,
                //            m_sources[i].elevation,
                //            currentPreset);
                //}
                
                m_mixBuffer.clear();

                for (int i = 0; i < (int)m_sources.size(); i++)
                {
                    auto& source = m_sources[i];

                    // skip paused or stopped emitters
                    if (source.state != EmitterState::Playing &&
                        source.state != EmitterState::Looping)
                        continue;

                    // get buffer from Librarian via AudioManager
                    const auto& buffer = m_audioManager.getBuffer(source.soundFileId);

                    auto& delayLine = m_delayLines[i];
                    int& readPos = m_readPositions[i];

                    auto samplesAvailable = buffer.getNumSamples() - readPos;
                    auto samplesThisBlock = juce::jmin(m_bufferSize, samplesAvailable);

                    m_smoothedITD[i].setTargetValue(source.itdSamples);

                    for (int n = 0; n < samplesThisBlock; n++)
                    {
                        float monoSample = buffer.getSample(0, readPos + n);
                        float smoothed = m_smoothedITD[i].getNextValue();
                        auto stereo = delayLine.process(monoSample, smoothed);
                        auto filtered = m_activeChains[i].processSample(stereo.left, stereo.right);

                        float gain = source.isAmbient
                            ? m_audioManager.getAmbientGain(source.soundFileId)
                            : 1.0f;

                        *m_mixBuffer.getWritePointer(0, n) += filtered.left * gain;
                        *m_mixBuffer.getWritePointer(1, n) += filtered.right * gain;
                    }

                    readPos += samplesThisBlock;
                    if (readPos >= buffer.getNumSamples())
                    {
                        readPos = 0;
                        if (source.state == EmitterState::Looping || source.isAmbient)
                        {
                            // keep playing — readPos already reset
                        }
                        else
                        {
                            source.state = EmitterState::Paused;
                        }
                    }
                }

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
        // 2. Start the background loop
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
                        // SVF — direct update on active chain, no swap
                        m_activeChains[i].setAzimuth(cmd.azimuth);
                        // crossover gains - direct update on on active chain
                        m_activeChains[i].setGains(cmd.ildGain);
                        break;
                    }
            }
            else if (cmd.type == EmitterCommand::Type::Reset)
            {
                m_sources.clear();
                m_delayLines.clear();
                m_readPositions.clear();
                m_smoothedITD.clear();
                m_activeChains.clear();
                m_shadowChains.clear();
                DBG("SAE: reset all emitters");
            }
        }
    }
}