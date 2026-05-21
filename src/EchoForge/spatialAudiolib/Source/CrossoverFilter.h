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
    class CrossoverFilter {
    public:
        void prepare(double sampleRate, int bufferSize);
        void setGains(ILDGain gains);
        StereoSample processSample(float left, float right);
        void reset();
    private:
        juce::dsp::StateVariableTPTFilter<float> m_lowpassL;
        juce::dsp::StateVariableTPTFilter<float> m_lowpassR;
        juce::dsp::StateVariableTPTFilter<float> m_highpassL;
        juce::dsp::StateVariableTPTFilter<float> m_highpassR;
        
        ILDGain m_gains;
        double m_sampleRate = 0.0;
    };
}


#endif