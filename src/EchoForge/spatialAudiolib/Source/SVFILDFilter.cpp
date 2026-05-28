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
        spec.numChannels = 1;

        m_filterL.prepare(spec);
        m_filterR.prepare(spec);

        m_filterL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        m_filterR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

        m_cutoffL.reset(sampleRate, 0.05);
        m_cutoffR.reset(sampleRate, 0.05);
        m_cutoffL.setCurrentAndTargetValue(10000.0f);
        m_cutoffR.setCurrentAndTargetValue(10000.0f);

        m_filterL.setCutoffFrequency(10000.0f);
        m_filterR.setCutoffFrequency(10000.0f);
    }
    void SVFILDFilter::setAzimuth(float azimuthDegrees)
    {
        float azRad = azimuthDegrees * (juce::MathConstants<float>::pi / 180.0f);
        // Brown-Duda cutoff formula
        float fcFar = 1500.0f + 8500.0f * (1.0f - std::abs(std::sin(azRad))) / 2.0f;
        float fcNear = 10000.0f;

        if (azimuthDegrees > 0.0f)
        {
            //source is on the RIGHT  so left is far  so attenuation on the high freq on left 
            m_cutoffL.setTargetValue(fcFar); // far ear → shadow
            m_cutoffR.setTargetValue(fcNear); // near ear → minimal attenuation
        }
        else if (azimuthDegrees < 0.0f)
        {
            //source is on the LEFT so right is far  
            m_cutoffL.setTargetValue(fcNear); // near ear → minimal attenuation
            m_cutoffR.setTargetValue(fcFar); // far ear → shadow
        }
        else 
        {
            // front equal on both ear 
            m_cutoffL.setTargetValue(fcNear);
            m_cutoffR.setTargetValue(fcNear);
        }

    }
    StereoSample SVFILDFilter::processSample(float left, float right)
    {
        m_filterL.setCutoffFrequency(m_cutoffL.getNextValue());
        m_filterR.setCutoffFrequency(m_cutoffR.getNextValue());

        StereoSample result;
        result.left = m_filterL.processSample(0, left);
        result.right = m_filterR.processSample(0, right);

        return result;
    }
    void SVFILDFilter::resetState()
    {
        m_filterL.reset();
        m_filterR.reset();
        m_cutoffL.setCurrentAndTargetValue(10000.0f);
        m_cutoffR.setCurrentAndTargetValue(10000.0f);
    }
}