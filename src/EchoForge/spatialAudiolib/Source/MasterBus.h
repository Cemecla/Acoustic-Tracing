/*
  ==============================================================================

    MasterBus.h
    Processing applied once to the whole stereo mix: room reverb + safety limiter.

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#ifndef MASTERBUS_H
#define MASTERBUS_H

namespace yutasound {

    // Shared room reverb settings (one reverb for the whole scene)
    struct ReverbSettings
    {
        float roomSize = 0.6f;   // 0..1, bigger room = longer tail
        float damping  = 0.4f;   // 0..1, more damping = darker tail
        float wetLevel = 0.3f;   // 0..1, overall loudness of the reverb
    };

    // Runs after every emitter has been rendered into the mix:
    //  1. Room reverb: each emitter adds its dry mono signal x its reverb send into the send
    //     buffer; one shared reverb turns it into a diffuse stereo tail added to the mix.
    //  2. Safety limiter: only acts when the mix would go over kLimiterCeiling (instant attack,
    //     ~100 ms release). Below that the mix passes through unchanged. (juce::dsp::Limiter is
    //     not used because it always compresses above -10 dBFS and adds ~+3.75 dB makeup gain.)
    class MasterBus
    {
    public:
        void prepare(double sampleRate, int bufferSize)
        {
            m_reverb.prepare({ sampleRate, (juce::uint32)bufferSize, 2 });
            setReverbSettings(ReverbSettings{});
            m_send.setSize(2, bufferSize);
            m_send.clear();
            m_releaseCoeff = 1.0f - std::exp(-1.0f / (0.1f * (float)sampleRate));
            m_limiterGain = 1.0f;
        }

        void setReverbSettings(const ReverbSettings& settings)
        {
            juce::dsp::Reverb::Parameters params;
            params.roomSize = juce::jlimit(0.0f, 1.0f, settings.roomSize);
            params.damping = juce::jlimit(0.0f, 1.0f, settings.damping);
            params.wetLevel = juce::jlimit(0.0f, 1.0f, settings.wetLevel);
            params.dryLevel = 0.0f; // the dry signal is already in the mix
            params.width = 1.0f;
            m_reverb.setParameters(params);
        }

        // Mono reverb send (one value per sample of the block). Emitters add into it,
        // process() consumes and clears it.
        float* getSendWritePointer() { return m_send.getWritePointer(0); }

        // Adds the reverb of the send to the mix, then limits the mix. mix must be stereo.
        void process(juce::AudioBuffer<float>& mix, int numSamples)
        {
            // same send on both reverb inputs: the reverb decorrelates left and right
            m_send.copyFrom(1, 0, m_send, 0, 0, numSamples);
            auto block = juce::dsp::AudioBlock<float>(m_send).getSubBlock(0, (size_t)numSamples);
            m_reverb.process(juce::dsp::ProcessContextReplacing<float>(block));
            mix.addFrom(0, 0, m_send, 0, 0, numSamples);
            mix.addFrom(1, 0, m_send, 1, 0, numSamples);
            m_send.clear();

            auto* left = mix.getWritePointer(0);
            auto* right = mix.getWritePointer(1);
            for (int n = 0; n < numSamples; ++n)
            {
                const float peak = std::max(std::abs(left[n]), std::abs(right[n]));
                const float target = peak > kLimiterCeiling ? kLimiterCeiling / peak : 1.0f;
                if (target < m_limiterGain)
                    m_limiterGain = target;                                   // instant attack
                else
                    m_limiterGain += (target - m_limiterGain) * m_releaseCoeff; // ~100 ms release
                if (m_limiterGain > 0.99999f)
                    m_limiterGain = 1.0f;                                     // exactly transparent again

                left[n] *= m_limiterGain;
                right[n] *= m_limiterGain;
            }
        }

        // clears the reverb tail (e.g. when the scene is reset)
        void reset()
        {
            m_reverb.reset();
            m_send.clear();
            m_limiterGain = 1.0f;
        }

    private:
        static constexpr float kLimiterCeiling = 0.95f; // about -0.45 dBFS

        juce::dsp::Reverb m_reverb;
        juce::AudioBuffer<float> m_send;   // channel 0 = mono send, channel 1 = copy for the reverb
        float m_limiterGain = 1.0f;
        float m_releaseCoeff = 0.0f;
    };
}

#endif // !MASTERBUS_H
