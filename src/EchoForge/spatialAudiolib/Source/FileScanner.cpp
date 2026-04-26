/*
  ==============================================================================

    FileScanner.cpp
    Created: 20 Apr 2026 12:17:59pm
    Author:  yutai

  ==============================================================================
*/

#include "FileScanner.h"

namespace yutasound {
    std::vector<juce::File> FileScanner::scanDirectory()
    {
        std::vector<juce::File> results;

        if (!rootFolder.isDirectory())
        {
            DBG("FileScanner: not a valid directory: " + rootFolder.getFullPathName());
            return results;
        }
        
        for (const auto& entry : juce::RangedDirectoryIterator(rootFolder, false))
        {
            juce::File file = entry.getFile();
            if (isValidAudio_(file))
                results.push_back(file);
        }

        DBG("FileScanner: found " + juce::String(results.size()) + " audio files");
        return results;
    }
    bool FileScanner::isValidAudio_(const juce::File& file)
    {
        auto extention = file.getFileExtension().toLowerCase();

        return extention == ".wav" 
            || extention == ".mp3" 
            || extention == ".aiff" 
            || extention == ".flac";
    }
}