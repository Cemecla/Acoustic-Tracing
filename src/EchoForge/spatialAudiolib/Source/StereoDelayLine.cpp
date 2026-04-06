/*
  ==============================================================================

    StereoDelayLine.cpp
    Created: 4 Apr 2026 12:07:58pm
    Author:  yutai

  ==============================================================================
*/

#include "StereoDelayLine.h"


namespace yutasound {

    void StereoDelayLine::prepare(double sampleRate, int bufferSize) {
        m_sampleRate = sampleRate;
        m_bufferSize = bufferSize;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = (juce::uint32) bufferSize;
        spec.numChannels = 1;

        m_delayL.prepare(spec);
        m_delayR.prepare(spec);
    }

    StereoSample StereoDelayLine::process(float monoSample, float delaySamples)
    {
        // step 1: push the mono sample into BOTH delay lines
        // pushSample(channel, sample) — channel is always 0 since each line is mono
        m_delayL.pushSample(0 , monoSample);
        m_delayR.pushSample(0 , monoSample);

        float left = 0.0f;
        float right = 0.0f;

      
        if (delaySamples > 0.0f) 
        {
            // source is on the RIGHT → left ear is far
            left = m_delayL.popSample(0, delaySamples, true); // far ear  → delayed
            right = m_delayR.popSample(0, 0.0f, true); // near ear → no delay
        }
        else if (delaySamples < 0.0f)
        {
            // source is on the LEFT → right ear is far
            left = m_delayL.popSample(0,0.0f,true);  // near ear → no delay
            right = m_delayR.popSample(0, std::abs(delaySamples),true);  // far ear  → delayed
        }
        else
        {
            // source is in front → no delay on either ear
            left = m_delayL.popSample(0, 0.0f, true);
            right = m_delayR.popSample(0, 0.0f, true);
        }

        return { left, right };
    }
}