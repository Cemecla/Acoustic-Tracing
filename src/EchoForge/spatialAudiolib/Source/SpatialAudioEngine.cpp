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

    SpatialAudioEngine::SpatialAudioEngine() : juce::Thread("ProcessingThread") 
    {}

    SpatialAudioEngine::~SpatialAudioEngine()
    {
        // 2. Safely stop the thread before the engine is destroyed!
        stop(); 
    }

    void SpatialAudioEngine::run()
    {
        while (!threadShouldExit())
        {
            // only write when buffer needs more data
            while (m_circularBuffer.getFreeSpace() >= m_bufferSize)
            {
                runProcessingLoop();
            }
            // sleep a tiny amount then check again
            wait(1);
        }
    }

    void SpatialAudioEngine::prepare(double sampleRate, int bufferSize) {
        std::cout << "i am preaparing " << std::endl;

        m_sampleRate = sampleRate;
        m_bufferSize = bufferSize;
        m_mixBuffer.setSize(2, bufferSize);
        m_circularBuffer.prepare(bufferSize * 2 + 1);
    }

    void SpatialAudioEngine::addSource(const juce::String& filePath)
    {
        std::cout << "i am adding source "<< std::endl;
        SoundSource source;
        source.loadFromPath(filePath);
        m_sources.push_back(std::move(source));

    }

    void SpatialAudioEngine::runProcessingLoop()
    {
        DBG("Thread is running! Processing audio...");

        m_mixBuffer.clear();

        for (auto& source : m_sources)
        {
            auto sampleAvaileble = source.getAudioBuffer().getNumSamples() - source.readPosition;
            auto samplesThisBlock = juce::jmin(m_bufferSize, sampleAvaileble);

            m_mixBuffer.addFrom(0, 0, source.getAudioBuffer(), 0, source.readPosition, samplesThisBlock, 0.5f);
            m_mixBuffer.addFrom(1, 0, source.getAudioBuffer(), 0, source.readPosition, samplesThisBlock, 0.5f);

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

    void SpatialAudioEngine::start()
    {
        // The 'false' here prevents JUCE from scanning MIDI, 
        // avoiding the thread crash in your Qt app.
        // 1. Put the hardware connection in the mailbox
        juce::MessageManager::callAsync([this]() {
            m_deviceManager.initialiseWithDefaultDevices(0, 2);
            m_deviceManager.addAudioCallback(&m_audioCallback);
            });

        // 2. Start the background loop
        startThread();
    }

    void SpatialAudioEngine::stop()
    {
        stopThread(2000); // wait max 2 seconds for thread to finish
        m_deviceManager.removeAudioCallback(&m_audioCallback);
    }
}