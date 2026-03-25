/*
  ==============================================================================

    AudioCallback.h
    Created: 23 Mar 2026 10:52:08am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "CircularStereoBuffer.h"

#ifndef AUDIOCALLBACK_H
#define AUDIOCALLBACK_H

namespace YutaSound {
	class AudioCallback : public juce::AudioIODeviceCallback
	{
	public:
		AudioCallback(CircularStereoBuffer& circularBuffer);

		void audioDeviceIOCallbackWithContext(
			const float* const* inputChannelData,
			int	numInputChannels,
			float* const* outputChannelData,
			int	numOutputChannels,
			int	numSamples,
			const juce::AudioIODeviceCallbackContext& context
		)override;

		//no need beacose my code not need to prepare it only read when nessesary --------------
		void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
		void audioDeviceStopped() override;

	private:
		CircularStereoBuffer& m_circularBuffer;
	};

}

#endif