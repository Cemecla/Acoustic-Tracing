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
        m_lowpass3000L.prepare(spec);
        m_lowpass3000R.prepare(spec);

        m_highpassL.prepare(spec);
        m_highpassR.prepare(spec);
        m_highpass300L.prepare(spec);
        m_highpass300R.prepare(spec);

        m_lowpassL.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        m_lowpassR.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        m_lowpass3000L.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        m_lowpass3000R.setType(juce::dsp::StateVariableTPTFilterType::lowpass);

        m_highpassL.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        m_highpassR.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        m_highpass300L.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        m_highpass300R.setType(juce::dsp::StateVariableTPTFilterType::highpass);

        m_lowpassL.setCutoffFrequency(300.0f);
        m_lowpassR.setCutoffFrequency(300.0f);
        m_lowpass3000L.setCutoffFrequency(3000.0f);
        m_lowpass3000R.setCutoffFrequency(3000.0f);

        m_highpassL.setCutoffFrequency(3000.0f);
        m_highpassR.setCutoffFrequency(3000.0f);
        m_highpass300L.setCutoffFrequency(300.0f);
        m_highpass300R.setCutoffFrequency(300.0f);

        m_gains = ILDGain();
    }
    void CrossoverFilter::setGains(ILDGain gains)
    {
        m_gains = gains;
    }
    StereoSample CrossoverFilter::processSample(float left, float right)
    {
        float lowL = m_lowpassL.processSample(0, left);
        float lowR = m_lowpassR.processSample(0, right);
        float restL = m_highpass300L.processSample(0, left);
        float restR = m_highpass300R.processSample(0, right);

        float midL = m_lowpass3000L.processSample(0, restL);
        float midR = m_lowpass3000R.processSample(0, restR);

        float highL = m_highpassL.processSample(0, restL);
        float highR = m_highpassR.processSample(0, restR);

        StereoSample result;

        result.left = lowL * m_gains.low + midL * m_gains.mid + highL * m_gains.high;
        result.right = lowR * m_gains.low + midR * m_gains.mid + highR * m_gains.high;
        return result;
    }
    void CrossoverFilter::reset()
    {
        m_lowpassL.reset();
        m_lowpassR.reset();
        m_lowpass3000L.reset();
        m_lowpass3000R.reset();

        m_highpassL.reset();
        m_highpassR.reset();
        m_highpass300L.reset();
        m_highpass300R.reset();
        
        m_gains = ILDGain();
    }
}
