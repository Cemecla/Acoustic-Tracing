/*
  ==============================================================================

    StereoDelayLine.h
    Created: 4 Apr 2026 12:07:58pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#ifndef STEREODELAYLINE_H
#define STEREODELAYLINE_H

namespace yutasound {
    struct StereoSample
    {
        float left = 0.0f;
        float right = 0.0f;
    };
    class StereoDelayLine{
    public:
        void prepare(double sampleRate, int bufferSize);
        StereoSample process(float monoSample, float delaySamples);
    private:
        int m_bufferSize = 0;
        double m_sampleRate = 0.0;
        // the following 2 line of code to create the a delayline per ear was given by AI 
        juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Thiran> m_delayL {64};
        juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Thiran> m_delayR {64};
    };

}
#endif // !STEREODELAYLINE_H
