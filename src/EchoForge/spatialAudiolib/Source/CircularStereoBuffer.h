/*
  ==============================================================================

    CircularStereoBuffer.h
    Created: 6 Mar 2026 10:20:26am
    Author:  yutai

  ==============================================================================
*/
#pragma once
#include "JuceHeader.h"
#ifndef CIRCULARSTEREOBUFFER_H
#define CIRCULARSTEREOBUFFER_H

namespace YutaSound {

	class CircularStereoBuffer
	{
	public:
		void prepare(int bufferSize);
		void write(float* left, float* right, int numSamples); //for SpatialAudioENgine write into the circular buffer
		void read(float* left, float* right, int numSamples); // for audio callback to get the content inthe buffer

	int getFreeSpace() const {
			return m_fifo.getFreeSpace();
	}
	private:
		juce::AbstractFifo m_fifo{1}; //  handle the when  should write or read protection to 
		juce::AudioBuffer<float> m_buffer;
		//std::vector<float> m_bufferL;
		//std::vector<float> m_bufferR;
	};


}
#endif
