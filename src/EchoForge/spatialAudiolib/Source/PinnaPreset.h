/*
  ==============================================================================

    PinnaPreset.h
    Created: 27 Apr 2026 10:15:49am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


#ifndef PINNAPRESET_H
#define PINNAPRESET_H
namespace yutasound {
    struct PinnaPreset
    {
        int id = -1;
        int cipicSubject = -1;
        juce::String category;
        float earSizeCm = 0.0f;
        juce::String description;

        // 7 frequency bands → gain in dB
        float gain4000 = 0.0f;
        float gain6000 = 0.0f;
        float gain8000 = 0.0f;
        float gain10000 = 0.0f;
        float gain12000 = 0.0f;
        float gain14000 = 0.0f;
        float gain16000 = 0.0f;
    };
}

#endif // !PINNAPRESET_H
