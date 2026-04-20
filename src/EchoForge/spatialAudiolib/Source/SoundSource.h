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
    
    // struct that will hold ild gain after calcule to be used an apply in the filters
    struct ILDGain
    {
        float low  = 1.0f;
        float mid  = 1.0f;
        float high = 1.0f;
    };

    class SoundSource {
    public:
        //function to load a audio file from a path 
        void loadFromPath(const juce::String& filePath);
        // getter that send a refference to audiobuffer
        const juce::AudioBuffer<float>& getAudioBuffer() const;

        float azimuth = 0.0f; // degrees, -180 to +180
        float elevation = 0.0f; // degrees,  -90 to +90
        float itdSamples = 0.0f;
        ILDGain ildGain;
        SoundRemaining soundRemaining;

        //position of starting to read sound  can be use to do rdelay calculs like in the session prject of state machine 
        int readPosition = 0;

        int id = -1;
        

    private:
        
        //specific buffer to save audio data in ram 

        juce::AudioBuffer<float> m_audioBuffer;
    };

}

// have to separate declaration and definition 

#endif