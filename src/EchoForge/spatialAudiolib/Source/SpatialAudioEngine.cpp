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

        // TODO: push matching activeChain, shadowChain
    }

    void SpatialAudioEngine::removeProcessingSlot(int index)
    {
        m_delayLines.erase(m_delayLines.begin() + index);
        m_readPositions.erase(m_readPositions.begin() + index);
        m_smoothedITD.erase(m_smoothedITD.begin() + index);
        // TODO: erase activeChain, shadowChain at index
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
                m_mixBuffer.clear();

                for (int i = 0; i < (int)m_audioManager.getSources().size(); i++)
                {
                    auto& source = m_audioManager.getSources()[i];

                    // skip paused or stopped emitters
                    if (source.state != EmitterState::Playing)
                        continue;

                    // get buffer from Librarian via AudioManager
                    const auto& buffer = m_audioManager.getBuffer(source.soundFileId);

                    auto& delayLine = m_delayLines[i];
                    int& readPos = m_readPositions[i];

                    auto samplesAvailable = buffer.getNumSamples() - readPos;
                    auto samplesThisBlock = juce::jmin(m_bufferSize, samplesAvailable);

                    float itd = source.itdSamples;
                    ILDGain ild = source.ildGain;

                    for (int n = 0; n < samplesThisBlock; n++)
                    {
                        float monoSample = buffer.getSample(0, readPos + n);
                        auto stereo = delayLine.process(monoSample, itd);

                        if (itd > 0.0f)
                            stereo.left *= ild.mid;
                        else if (itd < 0.0f)
                            stereo.right *= ild.mid;

                        *m_mixBuffer.getWritePointer(0, n) += stereo.left;
                        *m_mixBuffer.getWritePointer(1, n) += stereo.right;
                    }

                    readPos += samplesThisBlock;
                    if (readPos >= buffer.getNumSamples())
                        readPos = 0;
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
}