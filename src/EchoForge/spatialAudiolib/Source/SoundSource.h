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
#include "EmitterState.h"
#include "ILDGain.h"

#ifndef SOUNDSOURCE_H
#define SOUNDSOURCE_H

namespace yutasound {
       
    class SoundSource {
    public:
        //TODO: move to librarian 
        void loadFromPath(const juce::String& filePath);
        // getter that send a refference to audiobuffer
        const juce::AudioBuffer<float>& getAudioBuffer() const;

        
        
        int emitterId = -1; 
        int soundFileId = -1; // which file in Librarian (TODO: used when Librarian built)
        
        float azimuth = 0.0f; // degrees, -180 to +180
        float elevation = 0.0f; // degrees,  -90 to +90
        float itdSamples = 0.0f;
        ILDGain ildGain;
        SoundRemaining soundRemaining;

        //position of starting to read sound  can be use to do rdelay calculs like in the session prject of state machine 
        int readPosition = 0;
        EmitterState state = EmitterState::Playing;
        
        bool isAmbient = false;
    private:
        
        //TODO: move to librarian 

        juce::AudioBuffer<float> m_audioBuffer;
    };

}

// have to separate declaration and definition 

#endif