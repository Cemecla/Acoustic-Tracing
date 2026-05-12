/*
  ==============================================================================

    SVFILDFilter.h
    Created: 27 Apr 2026 10:22:26am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "StereoDelayLine.h"

#ifndef SVFILDFILTER_H
#define SVFILDFILTER_H

namespace yutasound {
    class SVFILDFilter {
    public:
        void prepare(double sampleRate, int bufferSize);
        void setAzimuth(float azimuthDegrees);
        StereoSample processSample(float left, float right);
        void resetState();
    private:
        juce::dsp::StateVariableTPTFilter<float> m_filterL;
        juce::dsp::StateVariableTPTFilter<float> m_filterR;

        juce::SmoothedValue<float> m_cutoffL;
        juce::SmoothedValue<float> m_cutoffR;

        double m_sampleRate = 0.0;
    };
}

#endif // !SVFILDFILTER_H