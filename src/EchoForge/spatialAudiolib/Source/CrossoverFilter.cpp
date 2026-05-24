/*
  ==============================================================================

    CrossoverFilter.cpp
    Created: 21 May 2026 9:54:46am
    Author:  yutai

  ==============================================================================
*/

#include "CrossoverFilter.h"

namespace yutasound {
    void CrossoverFilter::prepare(double sampleRate, int bufferSize)
    {
        m_sampleRate = sampleRate;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.numChannels = 1;
        spec.maximumBlockSize = (juce::uint32)bufferSize;

        m_lowpassL.prepare(spec);
        m_lowpassR.prepare(spec);
        m_highpassL.prepare(spec);
        m_highpassR.prepare(spec);

        //default both filters tp lowpass
        m_lowpassL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        m_lowpassR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        m_highpassL.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        m_highpassR.setType(juce::dsp::StateVariableTPTFilterType::highpass);

        //default cutoff in front, minimal shadow
        m_lowpassL.setCutoffFrequency(300.0f);
        m_lowpassR.setCutoffFrequency(300.0f);
        m_highpassL.setCutoffFrequency(3000.0f);
        m_highpassR.setCutoffFrequency(3000.0f);

        m_gains = ILDGain();
    }
    void CrossoverFilter::setGains(ILDGain gains)
    {
        m_gains = gains;
    }
    StereoSample CrossoverFilter::processSample(float left, float right)
    {
        //low band
        float lowL = m_lowpassL.processSample(0, left);
        float lowR = m_lowpassR.processSample(0, right);

        // high band
        float highL = m_highpassL.processSample(0, left);
        float highR = m_highpassR.processSample(0, right);

        // mid bands
        float midL = left - lowL - highL;
        float midR = right - lowR - highR;
        StereoSample result;

        result.left = lowL * m_gains.low + midL * m_gains.mid + highL * m_gains.high;
        result.right = lowR * m_gains.low + midR * m_gains.mid + highR * m_gains.high;

        return result;
    }
    void CrossoverFilter::reset()
    {
        m_lowpassL.reset();
        m_lowpassR.reset();
        m_highpassL.reset();
        m_highpassR.reset();
        m_gains = ILDGain();
    }
}
