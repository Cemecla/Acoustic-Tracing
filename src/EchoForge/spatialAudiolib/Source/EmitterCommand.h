/*
  ==============================================================================

    EmitterCommand.h
    Created: 30 Apr 2026 11:53:11am
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "SoundSource.h"
#include "PinnaPreset.h"
#include "MasterBus.h"

#ifndef EMITTERCOMMAND_H
#define EMITTERCOMMAND_H

namespace yutasound {
    struct EmitterCommand
    {
        enum class Type {Add, Remove, SetState, UpdateSpatial, Reset, SetPinnaPreset, SetReverbSettings};
        Type type;
        SoundSource emitter; // used for Add
        int emitterId; // used for Remove
        int index; // used for Remove (slot index in SAE)
        EmitterState newState;  // used for SetState
        float        itdSamples;
        ILDGain      ildGain;
        float azimuth = 0.0f;
        float elevation = 0.0f;
        float reverbSend = 0.0f;  // used for UpdateSpatial
        PinnaPreset  pinnaPreset; // used for SetPinnaPreset (applies to every emitter)
        ReverbSettings reverbSettings; // used for SetReverbSettings (shared room reverb)
    };
}
#endif 