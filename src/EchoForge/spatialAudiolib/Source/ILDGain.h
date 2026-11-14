/*
  ==============================================================================

    ILDGain.h
    Created: 20 Apr 2026 12:01:41pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#ifndef ILDGAIN_H
#define ILDGAIN_H

namespace yutasound {
    // Linear gain per band for each ear: low (< 300 Hz), mid (300 Hz - 3 kHz), high (> 3 kHz).
    // Applied by CrossoverFilter. Left != right is what makes the interaural level difference.
    struct ILDGain
    {
        float lowL  = 0.0f;
        float midL  = 0.0f;
        float highL = 0.0f;

        float lowR  = 0.0f;
        float midR  = 0.0f;
        float highR = 0.0f;
    };
}
#endif 