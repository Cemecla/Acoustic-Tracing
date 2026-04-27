/*
  ==============================================================================

    FilterChain.cpp
    Created: 27 Apr 2026 10:22:41am
    Author:  Administrateur

  ==============================================================================
*/

#include "FilterChain.h"
namespace yutasound {
    void FilterChain::prepare(double sampleRate, int bufferSize)
    {
    }
    void FilterChain::setParameters(float azimuth, float elevation, PinnaPreset presest)
    {
    }
    StereoSample FilterChain::processSample(float left, float right)
    {
        return StereoSample();
    }
    bool FilterChain::isDirty()
    {
        return false;
    }
    void FilterChain::clearDirty()
    {
    }
}