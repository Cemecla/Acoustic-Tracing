/*
  ==============================================================================

    CrossoverFilter.h
    Created: 21 May 2026 9:54:46am
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "ILDGain.h"
#include "StereoDelayLine.h"

#ifndef CROSSOVERFILTER_H
#define CROSSOVERFILTER_H

namespace yutasound {
    // Applies the per-ear ILDGain as 3 smooth frequency regions (low ~< 300 Hz, mid, high ~> 3 kHz):
    //     out = high * x + (mid - high) * LP3k(x) + (low - mid) * LP300(x)
    // with one-pole low-passes. When the 3 gains are equal the output is exactly gain * x: no
    // filtering and no phase change, so a sound in front passes through untouched. Transitions
    // are gentle (6 dB/oct) and never overshoot.
    // History: the old SVF low-pass + high-pass split cancelled itself at its cutoffs (-20 dB at
    // 300 Hz, -40 dB at 3 kHz). A Linkwitz-Riley split is flat but its phase shift raised peaks
    // by up to 1.7x (clipping on Knocking.mp3).
    class CrossoverFilter {
    public:
        void prepare(double sampleRate, int bufferSize);
        void setGains(ILDGain gains);
        StereoSample processSample(float left, float right);
        void reset();
    private:
        // channel 0 = left ear, channel 1 = right ear
        juce::dsp::FirstOrderTPTFilter<float> m_lowpass300;
        juce::dsp::FirstOrderTPTFilter<float> m_lowpass3000;

        // per-ear band gains, smoothed over 50 ms so position updates don't click
        juce::SmoothedValue<float> m_lowGainL, m_midGainL, m_highGainL;
        juce::SmoothedValue<float> m_lowGainR, m_midGainR, m_highGainR;
        std::array<juce::SmoothedValue<float>*, 6> allGains_();

        double m_sampleRate = 0.0;
    };
}


#endif
