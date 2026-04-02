/*
  ==============================================================================

    AudioManager.cpp
    Created: 30 Mar 2026 10:33:26am
    Author:  Administrateur

  ==============================================================================
*/

#include "AudioManager.h"
#include "CircularStereoBuffer.h"

namespace YutaSound {
    
    void AudioManager::prepare(double sampleRate, int bufferSize) {
        std::cout << "i am preaparing " << std::endl;

        m_sampleRate = sampleRate;
        m_bufferSize = bufferSize;
        m_mixBuffer.setSize(2, bufferSize);
    }

    void AudioManager::addSource(const juce::String& filePath)
    {
        std::cout << "i am adding source " << std::endl;
        SoundSource source;
        source.loadFromPath(filePath); 
        m_sources.push_back(std::move(source));

    }

    //void AudioManager::updateSource(const juce::String& md5Name, float azimuth, float elevation,
    //    SoundRemaining soundRemaining)
    //{
    //    for (auto& source : m_sources)
    //    {
    //        if (source.md5Name == md5Name)
    //        {
    //            source.azimuth = azimuth;
    //            source.elevation = elevation;
    //            source.soundRemaining = soundRemaining;
    //            // TODO: call computeITD_ and computeILD_ here later
    //            break;
    //        }
    //    }
    //}

    void AudioManager::runProcessingLoop(CircularStereoBuffer& circularBuffer)
    {
        DBG("Thread is running! Processing audio...");

        m_mixBuffer.clear();
        {
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
        }
        circularBuffer.write(
            m_mixBuffer.getWritePointer(0),
            m_mixBuffer.getWritePointer(1),
            m_bufferSize
        );
    }
    float AudioManager::computeITD_(float azimuth, float elevation)
    {
        // TODO: implement Woodworth formula
        return 0.0f;
    }

    float AudioManager::computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        // TODO: implement ILD
        return 1.0f;
    }
}
