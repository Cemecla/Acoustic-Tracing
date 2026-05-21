/*
  ==============================================================================

    Librarian.h
    Created: 20 Apr 2026 12:18:38pm
    Author:  yutai

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "SoundFileEntry.h"
#include "FileProcessor.h"
#include "FileScanner.h"
#include "AmbientEntry.h"
#include "ReservedSoundIds.h"
#include "json.hpp"

#ifndef LIBRARIAN_H
#define LIBRARIAN_H

using json = nlohmann::json;

namespace yutasound {
	class Librarian
	{
	public:
		int addItem(const juce::String& filePath);
		void removeItem(int id);
		void saveSoundRegistry(const juce::String& jsonPath);
		void loadSoundRegistry(const juce::String& jsonPath);
		void unloadSoundRegistry();

		const std::vector<SoundFileEntry>& getEntries() const;

		const juce::AudioBuffer<float>& getBuffer(int id) const;
		juce::String getFilePath(int id) const;
		int getCount() const;

		// Ambient sound manipulation
		void addAmbientItem(int id, const juce::String& role, const juce::String& filePath, float gain = 1.0f);
		void removeAmbientItem(int id);
		const std::vector<AmbientEntry>& getAmbientEntries() const;

	private:
		std::vector<SoundFileEntry> m_entries;
		int m_nextId = 0;

		bool isDuplicate_(const juce::String& md5) const;
		int getExistingId_(const juce::String& md5) const;
		std::vector<AmbientEntry> m_ambientEntries;

	};

}
#endif // !LIBRARIAN_H

