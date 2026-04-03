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

    void AudioManager::prepare(double sampleRate, int bufferSize)
    {
        m_sampleRate = sampleRate;
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
