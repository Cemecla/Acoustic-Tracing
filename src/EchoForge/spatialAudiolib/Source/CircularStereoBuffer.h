/*
  ==============================================================================

    CircularStereoBuffer.h
    Created: 6 Mar 2026 10:20:26am
    Author:  yutai

  ==============================================================================
*/

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


	private:
		juce::AbstractFifo m_fifo{512}; //  handle the when  should write or read protection to 
		std::vector<float> m_bufferL;
		std::vector<float> m_bufferR;
	};


};
#endif
