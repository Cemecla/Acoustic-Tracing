/*
  ==============================================================================

    PinnaCalibration.h
    Created: 27 Apr 2026 10:22:01am
    Author:  Administrateur

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "PinnaPreset.h"
#include "json.hpp"

#ifndef PINNACALIBRATION_H
#define PINNACALIBRATION_H

using json = nlohmann::json;

namespace yutasound {
	class PinnaCalibration
	{
	public:
		void loadFromJson(const juce::String& path);
		const PinnaPreset& getPreset(int id) const;
		const PinnaPreset& getDefaultPreset() const;
		int getCount() const;

	private:
		std::vector<PinnaPreset> m_presets;
		// fallback if id not found
		PinnaPreset m_defaultPreset;
	};
}

#endif // !PINNACALIBRATION_H
