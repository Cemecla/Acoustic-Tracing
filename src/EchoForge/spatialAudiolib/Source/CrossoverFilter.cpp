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
        spec.numChannels = 2; // 0 = left ear, 1 = right ear
        spec.maximumBlockSize = (juce::uint32)bufferSize;

        m_lowpass300.prepare(spec);
        m_lowpass3000.prepare(spec);

        m_lowpass300.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
        m_lowpass3000.setType(juce::dsp::FirstOrderTPTFilterType::lowpass);

        m_lowpass300.setCutoffFrequency(300.0f);
        m_lowpass3000.setCutoffFrequency(3000.0f);

        // gains start at 0 (silent) like before; the first setGains() fades in over 50 ms
        for (auto* gain : allGains_())
        {
            gain->reset(sampleRate, 0.05);
            gain->setCurrentAndTargetValue(0.0f);
        }
    }
    void CrossoverFilter::setGains(ILDGain gains)
    {
        m_lowGainL.setTargetValue(gains.lowL);
        m_midGainL.setTargetValue(gains.midL);
        m_highGainL.setTargetValue(gains.highL);
        m_lowGainR.setTargetValue(gains.lowR);
        m_midGainR.setTargetValue(gains.midR);
        m_highGainR.setTargetValue(gains.highR);
    }
    StereoSample CrossoverFilter::processSample(float left, float right)
    {
        const float below300L  = m_lowpass300.processSample(0, left);
        const float below3000L = m_lowpass3000.processSample(0, left);
        const float below300R  = m_lowpass300.processSample(1, right);
        const float below3000R = m_lowpass3000.processSample(1, right);

        const float lowL = m_lowGainL.getNextValue();
        const float midL = m_midGainL.getNextValue();
        const float highL = m_highGainL.getNextValue();
        const float lowR = m_lowGainR.getNextValue();
        const float midR = m_midGainR.getNextValue();
        const float highR = m_highGainR.getNextValue();

        StereoSample result;
        result.left = highL * left + (midL - highL) * below3000L + (lowL - midL) * below300L;
        result.right = highR * right + (midR - highR) * below3000R + (lowR - midR) * below300R;
        return result;
    }
    void CrossoverFilter::reset()
    {
        m_lowpass300.reset();
        m_lowpass3000.reset();

        // reset() only clears the filter memory, like JUCE's reset(): the gains keep their target
        // and jump straight to it. Zeroing them would silence a chain right after a shadow swap.
        for (auto* gain : allGains_())
            gain->setCurrentAndTargetValue(gain->getTargetValue());
    }

    std::array<juce::SmoothedValue<float>*, 6> CrossoverFilter::allGains_()
    {
        return { &m_lowGainL, &m_midGainL, &m_highGainL, &m_lowGainR, &m_midGainR, &m_highGainR };
    }
}
