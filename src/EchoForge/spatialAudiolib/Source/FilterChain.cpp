/*
  ==============================================================================

    FilterChain.cpp
    Created: 27 Apr 2026 10:22:41am
    Author:  Administrateur

  ==============================================================================
*/

#include "FilterChain.h"
namespace yutasound {
    void FilterChain::prepare(double sampleRate, int bufferSize)
    {
        m_sampleRate = sampleRate;

        // prepare SVF 
        m_svfILD.prepare(sampleRate, bufferSize);

        // prepare 7 pinna EQ filters
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.numChannels = (juce::uint32)bufferSize;
        spec.numChannels = 1;

        for (auto& filter : m_pinnaFilters)
            filter.prepare(spec);

        m_isDirty = false;
    }

    void FilterChain::setParameters(float azimuth, float elevation, PinnaPreset preset)
    {
        m_azimuth = azimuth;
        m_elevation = elevation;

        // update SVF ILD cutoff
        m_svfILD.setAzimuth(azimuth);

        // update 7 pinna EQ bands
        float gains[7] = {
            preset.gain4000,  preset.gain6000,  preset.gain8000,
            preset.gain10000, preset.gain12000, preset.gain14000,
            preset.gain16000
        };

        float freqs[7] = { 4000.0f, 6000.0f, 8000.0f,
                           10000.0f, 12000.0f, 14000.0f, 16000.0f };

        for (int b = 0; b < 7; b++)
        {
            float linearGain = std::pow(10.0f, gains[b] / 20.0f);
            *m_pinnaFilters[b].coefficients =
                *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                    m_sampleRate, freqs[b], 1.0f, linearGain);
        }
        m_isDirty = true; // signal SAE to swap at next block boundary
    }

    StereoSample FilterChain::processSample(float left, float right)
    {
        // SVF ILD - head shadow
        auto stereo = m_svfILD.processSample(left, right);

        // 2. Pinna EQ — 7 bands both ears
        for (auto& filter : m_pinnaFilters)
        {
            stereo.left = filter.processSample(stereo.left);
            stereo.right = filter.processSample(stereo.right);
        }

        return stereo;
    }

    bool FilterChain::isDirty()const
    {
        return m_isDirty;
    }
   
    void FilterChain::clearDirty()
    {
        m_isDirty = false;
    }
}