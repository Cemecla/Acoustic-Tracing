/*
  ==============================================================================

    ReservedSoundIds.h
    Created: 12 May 2026 11:50:05am
    Author:  yutai

  ==============================================================================
*/

#pragma once

#ifndef RESERVEDSOUNDIDS_H
#define RESERVEDSOUNDIDS_H
namespace yutasound {
    namespace ReservedIds {
        constexpr int AMBIENT_MIN = 0;
        constexpr int AMBIENT_MAX = 9;
        constexpr int DYNAMIC_START = 10;

        inline bool isAmbient(int id) { return id >= AMBIENT_MIN && id <= AMBIENT_MAX;  }
        inline bool isDynamic(int id) { return id >= DYNAMIC_START; }
    };
}

#endif // !RESERVEDSOUNDIDS_H
