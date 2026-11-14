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
        const float azRad = azimuthDegrees * (juce::MathConstants<float>::pi / 180.0f);

        // Head shadow on the far ear: its low-pass cutoff goes from fcNear (source in front or
        // behind, no shadow) down to fcFarMin (source fully to the side), log-interpolated with
        // |sin(azimuth)|. Continuous through 0 and 180 deg: no jump when the source crosses them,
        // and a source straight behind is centred.
        constexpr float fcNear = 10000.0f;
        constexpr float fcFarMin = 1500.0f;
        const float lateral = std::sin(azRad); // > 0: source on the right
        const float fcFar = fcNear * std::pow(fcFarMin / fcNear, std::abs(lateral));

        if (lateral > 0.0f)
        {
            // source on the RIGHT: left ear is far -> shadow on the left
            m_cutoffL.setTargetValue(fcFar);
            m_cutoffR.setTargetValue(fcNear);
        }
        else
        {
            // source on the LEFT: right ear is far (in front / behind fcFar == fcNear)
            m_cutoffL.setTargetValue(fcNear);
            m_cutoffR.setTargetValue(fcFar);
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
        // clear the filter memory only; keep the azimuth-based cutoff (jump straight to it)
        m_filterL.reset();
        m_filterR.reset();
        m_cutoffL.setCurrentAndTargetValue(m_cutoffL.getTargetValue());
        m_cutoffR.setCurrentAndTargetValue(m_cutoffR.getTargetValue());
    }
}