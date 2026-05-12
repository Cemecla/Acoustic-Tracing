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
        constexpr int AMBIANT_MIN = 0;
        constexpr int AMBIANT_MAX = 9;
        constexpr int DYNAIC_START = 10;

        inline bool isAmbient(int id) { return id >= AMBIANT_MIN && id <= AMBIANT_MAX;  }
        inline bool isDynamic(int id) { return id >= DYNAIC_START; }
    };
}

#endif // !RESERVEDSOUNDIDS_H
