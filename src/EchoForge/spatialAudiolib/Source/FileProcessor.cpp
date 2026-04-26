/*
  ==============================================================================

    FileProcessor.cpp
    Created: 20 Apr 2026 12:17:23pm
    Author:  yutai

  ==============================================================================
*/

#include "FileProcessor.h"
namespace yutasound {
    yutasound::FileProcessor::FileProcessor(const juce::File& file)
        : m_sourceFile(file)
    {}

    SoundFileEntry FileProcessor::process() 
    {
        if (!existsAsFile_())
        {
            DBG("FileProcessor: file not found: " + m_sourceFile.getFullPathName());
            return SoundFileEntry();
        }

        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(m_sourceFile));

        if (reader == nullptr)
        {
            DBG("FileProcessor: failed to read: " + m_sourceFile.getFullPathName());
            return SoundFileEntry();
        }

        juce::AudioBuffer<float> buffer;
        buffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);

        unifyToMono_(buffer);

        applyGain_(buffer);

        juce::String md5 = createMd5_();

        SoundFileEntry entry;
        entry.md5Name = md5;
        entry.filePath = m_sourceFile.getFullPathName();
        entry.buffer = std::move(buffer);

        return entry;
    }

    juce::String FileProcessor::createMd5_()
    {
        juce::MD5 md5(m_sourceFile);
        return md5.toHexString();
    }

    bool FileProcessor::existsAsFile_()
    {
        return m_sourceFile.existsAsFile();
    }

    void FileProcessor::unifyToMono_(juce::AudioBuffer<float>& buffer)
    {
        if (buffer.getNumChannels() == 1)
            return; // its already mono as an audio file

        //mix les channels into channel 0
        for (int ch = 1; ch < buffer.getNumChannels(); ch++)
            buffer.addFrom(0, 0, buffer,ch,0, buffer.getNumSamples(), 1.0f / buffer.getNumChannels());
        
        //resize to mono (1 channel
        buffer.setSize(1, buffer.getNumSamples(), true);
    }

    void FileProcessor::applyGain_(juce::AudioBuffer<float>&buffer)
    {
        float peak = buffer.getMagnitude(0, buffer.getNumSamples());
        if (peak > 0.0f)
            buffer.applyGain(1.0f / peak);
    }

}

