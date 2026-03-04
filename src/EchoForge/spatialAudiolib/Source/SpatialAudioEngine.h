/*
  ==============================================================================

    SpatialAudioEngine.h
    Created: 2 Mar 2026 11:09:36am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
class SpatialAudioEngine
{
public:
	//SpatialAudioEngine();
	//~SpatialAudioEngine();

    void prepare(double sampleRate, int bufferSize);
    //void addSource(juce::AudioBuffer<float>& buffer);
    void runProcessingLoop();

private:

};


