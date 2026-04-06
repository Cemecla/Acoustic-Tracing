/*
  ==============================================================================

    AudioManager.cpp
    Created: 30 Mar 2026 10:33:26am
    Author:  Administrateur

  ==============================================================================
*/

#include "AudioManager.h"
#include "CircularStereoBuffer.h"

namespace yutasound {

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
    //            compteITD_(source.azimuth, source.elevation);
    //            // TODO: call computeITD_ and computeILD_ here later
    //            break;
    //        }
    //    }
    //}

    float AudioManager::computeITD_(float azimuth, float elevation)
    {
        constexpr float r = 0.0875f;
        constexpr float c = 343.0f;
        float azRad = azimuth * (juce::MathConstants<float>::pi / 180.0f);
        float elRad = elevation * (juce::MathConstants<float>::pi / 180.0f);

        float thetaEff = std::cos(elRad) * std::sin(azRad);
        float ITD = (r / c) * (std::asin(thetaEff) + thetaEff);
        return ITD * (float)m_sampleRate; // convertion into samples
    }

    float AudioManager::computeILD_(float azimuth, float elevation, SoundRemaining soundRemaining)
    {
        // TODO: implement ILD
        return 1.0f;
    }
}
