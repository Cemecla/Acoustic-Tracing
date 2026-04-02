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

    SpatialAudioEngine::SpatialAudioEngine(AudioManager& audiomanager) : juce::Thread("ProcessingThread"), m_audioManager(audiomanager)
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
                m_audioManager.runProcessingLoop(m_circularBuffer);
            }
            // sleep a tiny amount then check again
            wait(1);
        }
    }

    void SpatialAudioEngine::prepare(double sampleRate, int bufferSize) {
        std::cout << "i am preaparing " << std::endl;

        m_bufferSize = bufferSize;
        m_circularBuffer.prepare(bufferSize * 2 + 1);
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