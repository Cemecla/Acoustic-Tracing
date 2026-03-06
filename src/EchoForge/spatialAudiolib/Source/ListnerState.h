/*
  ==============================================================================

    ListnerState.h
    Created: 4 Mar 2026 3:11:35pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#ifndef LISTENERSTATE_H
#define LISTENERSTATE_H

namespace YutaSound {

    class ListnerState
    {
    public:
        // will get azimuth angle and elevation angle of the head 
        float azimuth = 0.0f;
        float elevation = 0.0f;

    private:

    };
};
#endif