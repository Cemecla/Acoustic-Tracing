/*
  ==============================================================================

    AmbientEntry.h
    Created: 12 May 2026 11:52:02am
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#ifndef AMBIENTENTRY_H
#define AMBIENTENTRY_H

namespace yutasound {
    struct AmbientEntry
    {
        int id = -1;
        juce::String role;
        juce::String filePath;
        float gain = 0.0f;
        juce::AudioBuffer<float> buffer; 
    };
}

#endif // !AMBIENTENTRY_H