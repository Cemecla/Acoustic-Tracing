/*
  ==============================================================================

    FileScanner.h
    Created: 20 Apr 2026 12:17:59pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


#ifndef FILESCANNER_H
#define FILESCANNER_H

namespace yutasound {
    class FileScanner {
    public:
        juce::File rootFolder;
        std::vector<juce::File> scanDirectory();
    private:
        bool isValidAudio_(const juce::File& file);
    };
}

#endif // !FILESCANNER_H
