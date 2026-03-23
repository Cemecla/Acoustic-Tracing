/*
  ==============================================================================

    SpatialAudioEngine.cpp
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#include "SpatialAudioEngine.h"
#include "SoundSource.h"
using namespace YutaSound;

SpatialAudioEngine::SpatialAudioEngine()
{
    juce::MessageManager::getInstance();
}
SpatialAudioEngine::~SpatialAudioEngine()
{
    juce::shutdownJuce_GUI();
}

void YutaSound::SpatialAudioEngine::prepare(double sampleRate, int bufferSize) {
    m_sampleRate = sampleRate;
    m_bufferSize = bufferSize;
}

void YutaSound::SpatialAudioEngine::addSource(const juce::String& filePath)
{
    SoundSource source;
    source.loadFromPath(filePath);
    m_sources.push_back(std::move(source));
}

void YutaSound::SpatialAudioEngine::runProcessingLoop()
{
    m_mixBuffer.clear();

    for (auto& source : m_sources) 
    {
        auto sampleAvaileble = source.getAudioBuffer().getNumSamples() - source.readPosition;
        auto samplesThisBlock = juce::jmin(m_bufferSize, sampleAvaileble);

        m_mixBuffer.addFrom(0,0,source.getAudioBuffer(),0,source.readPosition,samplesThisBlock);
        m_mixBuffer.addFrom(1,0,source.getAudioBuffer(),0,source.readPosition,samplesThisBlock);

        source.readPosition += samplesThisBlock;
        
        if (source.readPosition >= source.getAudioBuffer().getNumSamples())
            source.readPosition = 0;
    }
    m_circularBuffer.write(
        m_mixBuffer.getWritePointer(0),
        m_mixBuffer.getWritePointer(0),
        m_bufferSize
    );

}

void SpatialAudioEngine::start()
{
    m_deviceManager.initialise(0, 2, nullptr, true);
    m_deviceManager.addAudioCallback(&m_audioCallback);
}

void YutaSound::SpatialAudioEngine::stop()
{
}
