/*
  ==============================================================================

    SpatialAudioEngine.cpp
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#include "SpatialAudioEngine.h"
#include "SoundSource.h"

namespace YutaSound {

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
        m_circularBuffer.prepare(bufferSize + 1);
    }

    void SpatialAudioEngine::addSource(const juce::String& filePath)
    {
        SoundSource source;
        source.loadFromPath(filePath);
        m_sources.push_back(std::move(source));
        // TODO: push matching delayLine, activeChain, shadowChain
    }

    void SpatialAudioEngine::run()
    {
        DBG("SAE run started, bufferSize: " + juce::String(m_bufferSize));
        while (!threadShouldExit())
        {
            // only write when buffer needs more data
            while (m_circularBuffer.getFreeSpace() >= m_bufferSize)
            {
                m_mixBuffer.clear();

                for (auto& source : m_sources)
                {
                    auto samplesAvailable = source.getAudioBuffer().getNumSamples() - source.readPosition;
                    auto samplesThisBlock = juce::jmin(m_bufferSize, samplesAvailable);

                    // TODO: apply delayLine[i] for ITD
                    // TODO: apply activeChain[i] for ILD + pinna

                    m_mixBuffer.addFrom(0, 0, source.getAudioBuffer(), 0,
                        source.readPosition, samplesThisBlock, 0.5f);
                    m_mixBuffer.addFrom(1, 0, source.getAudioBuffer(), 0,
                        source.readPosition, samplesThisBlock, 0.5f);

                    source.readPosition += samplesThisBlock;
                    if (source.readPosition >= source.getAudioBuffer().getNumSamples())
                        source.readPosition = 0;
                }

                // TODO: swap shadow -> active chains here (block boundary)

                m_circularBuffer.write(
                    m_mixBuffer.getWritePointer(0),
                    m_mixBuffer.getWritePointer(1),
                    m_bufferSize
                );
            }
            wait(1);
        }
    }

    void SpatialAudioEngine::start()
    {   
        m_deviceManager.initialiseWithDefaultDevices(0, 2);
        m_deviceManager.addAudioCallback(&m_audioCallback);
        // 2. Start the background loop
        startThread();
    }

    void SpatialAudioEngine::stop()
    {
        stopThread(2000); // wait max 2 seconds for thread to finish
        m_deviceManager.removeAudioCallback(&m_audioCallback);
    }
}