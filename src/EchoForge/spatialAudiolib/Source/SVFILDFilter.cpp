/*
  ==============================================================================

    SVFILDFilter.cpp
    Created: 27 Apr 2026 10:22:26am
    Author:  Administrateur

  ==============================================================================
*/

#include "SVFILDFilter.h"

namespace yutasound {
    void SVFILDFilter::prepare(double sampleRate, int bufferSize)
    {
        m_sampleRate = sampleRate;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.numChannels = (juce::uint32)bufferSize;
        spec.numChannels = 1;

        m_filterL.prepare(spec);
        m_filterR.prepare(spec);

        //default both filters tp lowpass
        m_filterL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        m_filterR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

        //default cutoff in front, minimal shadow
        m_filterL.setCutoffFrequency(10000.0f);
        m_filterR.setCutoffFrequency(10000.0f);
    }
    void SVFILDFilter::setAzimuth(float azimuthDegrees)
    {
        float azRad = azimuthDegrees * (juce::MathConstants<float>::pi / 180.0f);
        // Brown-Duda cutoff formula
        float fc = 1500.0f + 8500.0f * (1.0f + std::cos(azRad)) / 2.0f;

        if (azimuthDegrees > 0.0f)
        {
            //source is on the RIGHT  so left is far  so attenuation on the high freq on left 
            m_filterL.setCutoffFrequency(fc); // far ear → shadow
            m_filterR.setCutoffFrequency(10000.0f); // near ear → minimal attenuation
        }
        else if (azimuthDegrees > 0.0f)
        {
            //source is on the LEFT so right is far  
            m_filterL.setCutoffFrequency(10000.0f); // near ear → minimal attenuation
            m_filterR.setCutoffFrequency(fc); // far ear → shadow
        }
        else 
        {
            // front equal on both ear 
            m_filterL.setCutoffFrequency(10000.0f);
            m_filterR.setCutoffFrequency(10000.0f);
        }

    }
    StereoSample SVFILDFilter::processSample(float left, float right)
    {
        StereoSample result;
        result.left = m_filterL.processSample(0, left);
        result.right = m_filterR.processSample(0, right);
        return StereoSample();
    }
}