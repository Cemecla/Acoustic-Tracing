/*
  ==============================================================================

    SoundSource.h
    Created: 2 Mar 2026 1:49:52pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SoundSource {
public:
    //function to load a audio file from a path 
    void loadFromPath(const juce::String& filePath);
    // getter that send a refference to audiobuffer
    const juce::AudioBuffer<float>& getAudioBuffer() const;

    //TODOO 

    //void changeistnerState();
    //bool isPlaying = true;


    
private:
    // class that contain list of audio format to be able to read the file in the right format 
    // may have to change later if we allready know that they are in 32bit float .wav 
    juce::AudioFormatManager m_formatManager; 
    // by using unique pointer on to the reader juce will delete itself after the loading of the sound donne 
    std::unique_ptr<juce::AudioFormatReader> _reader;
    //position of starting to read sound  can be use to do rdelay calculs like in the session prject of state machine 
    int m_readPosition = 0;
    int m_id;
    //specific buffer to save audio data in ram 
    juce::AudioBuffer<float> m_audioBuffer;
};