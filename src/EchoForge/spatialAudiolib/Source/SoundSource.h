/*
  ==============================================================================

    SoundSource.h
    Created: 2 Mar 2026 1:49:52pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SoundRemaining.h"

#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

namespace yutasound {

    class SoundSource {
    public:
        //function to load a audio file from a path 
        void loadFromPath(const juce::String& filePath);
        // getter that send a refference to audiobuffer
        const juce::AudioBuffer<float>& getAudioBuffer() const;

        float azimuth = 0.0f;
        float elevation = 0.0f;
        SoundRemaining soundRemaining;

        //position of starting to read sound  can be use to do rdelay calculs like in the session prject of state machine 
        int readPosition = 0;

    private:
        //int m_id = 0;
        //specific buffer to save audio data in ram 
        juce::AudioBuffer<float> m_audioBuffer;
    };

}

// have to separate declaration and definition 

#endif