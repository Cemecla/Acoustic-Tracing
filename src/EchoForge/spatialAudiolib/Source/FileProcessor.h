/*
  ==============================================================================

    FileProcessor.h
    Created: 20 Apr 2026 12:17:23pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SoundFileEntry.h"

#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H

namespace yutasound {
    class FileProcessor {

    public:
        FileProcessor(const juce::File& file);
        SoundFileEntry process();

        
    
    private:
        juce::File m_sourceFile;
        juce::String createMd5_();
        bool existsAsFile_();
        void unifyToMono_(juce::AudioBuffer<float>& buffer);
        void applyGain_(juce::AudioBuffer<float>& buffer);

    };
}
#endif // !FILEPROCESSOR_H
