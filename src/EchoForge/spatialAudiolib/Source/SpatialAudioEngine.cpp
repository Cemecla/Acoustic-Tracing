/*
  ==============================================================================

    SpatialAudioEngine.cpp
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#include "SpatialAudioEngine.h"
#include "SoundSource.h"

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

    void SpatialAudioEngine::addSource(const juce::String& filePath)
    {
        SoundSource source;
        source.loadFromPath(filePath);
        m_sources.push_back(std::move(source));

        StereoDelayLine dl;
        dl.prepare(m_sampleRate, m_bufferSize);
        m_delayLines.push_back(std::move(dl));
        // TODO: push matching activeChain, shadowChain
    }

    void SpatialAudioEngine::run()
    {
        while (!threadShouldExit())
        {
            if (m_circularBuffer.getFreeSpace() >= m_bufferSize)
            {
                DBG("SAE writing block...");
                m_mixBuffer.clear();

                for (int i = 0; i < m_sources.size(); i++)
                {
                    auto& source = m_sources[i];
                    auto& delayLine = m_delayLines[i];
                    auto samplesAvailable = source.getAudioBuffer().getNumSamples() - source.readPosition;
                    auto samplesThisBlock = juce::jmin(m_bufferSize, samplesAvailable);
                    
                    for (int n = 0; n < samplesThisBlock; n++)
                    {
                        float monoSample = source.getAudioBuffer().getSample(0, source.readPosition + n);
                        //float itd = m_audioManager.computeITD_(45.0f, 0.0f);
                        auto stereo = delayLine.process(monoSample, -20.0f);
                        //dereference operator
                        *m_mixBuffer.getWritePointer(0, n) += stereo.left;
                        *m_mixBuffer.getWritePointer(1, n) += stereo.right;
                    }

                    source.readPosition += samplesThisBlock;
                    if (source.readPosition >= source.getAudioBuffer().getNumSamples())
                        source.readPosition = 0;
                }

                m_circularBuffer.write(
                    m_mixBuffer.getWritePointer(0),
                    m_mixBuffer.getWritePointer(1),
                    m_bufferSize
                );
            }
            else
            {
                wait(1); // only sleep when buffer is actually full
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
}