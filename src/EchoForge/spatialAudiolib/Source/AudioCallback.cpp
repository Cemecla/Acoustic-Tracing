/*
  ==============================================================================

    AudioCallback.cpp
    Created: 23 Mar 2026 10:52:08am
    Author:  Administrateur

  ==============================================================================
*/

#include "AudioCallback.h"
namespace YutaSound {

    AudioCallback::AudioCallback(CircularStereoBuffer& circularBuffer)
        :m_circularBuffer(circularBuffer)
    {}

    void AudioCallback::audioDeviceIOCallbackWithContext(
        const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const juce::AudioIODeviceCallbackContext& context)
    {
        m_circularBuffer.read(
            outputChannelData[0],
            outputChannelData[1],
            numSamples
        );
    }

    void AudioCallback::audioDeviceAboutToStart(juce::AudioIODevice* device)
    {}
    void AudioCallback::audioDeviceStopped()
    {}
}