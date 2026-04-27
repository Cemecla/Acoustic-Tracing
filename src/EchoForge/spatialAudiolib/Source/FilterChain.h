/*
  ==============================================================================

    FilterChain.h
    Created: 27 Apr 2026 10:22:41am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SVFILDFilter.h"
#include "PinnaPreset.h"
#include "StereoDelayLine.h"

#ifndef FILTERCHAIN_H
#define FILTERCHAIN_H

namespace yutasound {
    class FilterChain {
    public:
        void prepare(double sampleRate, int bufferSize);
        void setParameters(float azimuth, float elevation, PinnaPreset preset);
        StereoSample processSample(float left, float right);
        // in audio world Dirty mean "this data has been updated and needs to be applied"
        bool isDirty() const;
        void clearDirty();

    private:
        SVFILDFilter m_svfILD;
        std::array<juce::dsp::IIR::Filter<float>, 7> m_pinnaFilters;
        std::atomic<bool> m_isDirty;
        float m_azimuth;
        float m_elevation;
        double m_sampleRate = 0.0;

    };
}
#endif // !FILTERCHAIN_H
