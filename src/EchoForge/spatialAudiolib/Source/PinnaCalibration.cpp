/*
  ==============================================================================

    PinnaCalibration.cpp
    Created: 27 Apr 2026 10:22:01am
    Author:  Administrateur

  ==============================================================================
*/

#include "PinnaCalibration.h"

namespace yutasound {
    void PinnaCalibration::loadFromJson(const juce::String& path)
    {
        juce::File file(path);
        json input = json::parse(file.loadFileAsString().toStdString());
        for (const auto& p : input["presets"])
        {
            PinnaPreset preset;
            preset.id = p["id"].get<int>();
            preset.cipicSubject = p["cipic_subject"].get<int>();
            preset.category = juce::String(p["category"].get<std::string>());
            preset.earSizeCm = p["ear_size_cm"].get<float>();
            preset.gain4000 = p["bands"]["4000"].get<float>();
            preset.gain6000 = p["bands"]["6000"].get<float>();
            preset.gain8000 = p["bands"]["8000"].get<float>();
            preset.gain10000 = p["bands"]["10000"].get<float>();
            preset.gain12000 = p["bands"]["12000"].get<float>();
            preset.gain14000 = p["bands"]["14000"].get<float>();
            preset.gain16000 = p["bands"]["16000"].get<float>();
            m_presets.push_back(preset);
        }
        DBG("PinnaCalibration: loaded " + juce::String(m_presets.size()) + " presets");
    }
    const PinnaPreset& PinnaCalibration::getPreset(int id) const
    {
        for (const auto& preset : m_presets)
            if (preset.id == id)
                return preset;
        jassertfalse;
        return m_defaultPreset;
    }
    const PinnaPreset& PinnaCalibration::getDefaultPreset() const
    {
        return getPreset(13);
    }
    int PinnaCalibration::getCount() const
    {
        return (int)m_presets.size();
    }
}