/*
  ==============================================================================

    SoundFileEntry.h
    Created: 24 Apr 2026 10:02:24am
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

#ifndef SOUNDFILEENTRY_H
#define SOUNDFILEENTRY_H

namespace yutasound {
    struct SoundFileEntry
    {
        int id = -1;
        juce::String md5Name;
        juce::String filePath;
        juce::AudioBuffer<float> buffer; //mono normalized
    };
}

#endif // !SOUNDFILEENTRY_H
