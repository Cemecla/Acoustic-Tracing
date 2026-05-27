/*
  ==============================================================================

    Librarian.cpp
    Created: 20 Apr 2026 12:18:38pm
    Author:  yutai

  ==============================================================================
*/

#include "Librarian.h"

namespace yutasound {
    int Librarian::addItem(const juce::String& filePath)
    {
        juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
        juce::File file = juce::File::isAbsolutePath(filePath)
            ? juce::File(filePath)
            : appDir.getChildFile(filePath);
        FileProcessor fp(file);
        SoundFileEntry entry = fp.process();

        if (entry.md5Name.isEmpty())
        {
            DBG("Librarian: failed to process file: " + filePath);
            return -1;
        }

        // check duplicate
        if (isDuplicate_(entry.md5Name))
        {
            DBG("Librarian: duplicate file, returning existing id");
            return getExistingId_(entry.md5Name);
        }

        // assign id
        int newId = m_nextId++;
        entry.id = newId;
        m_entries.push_back(std::move(entry));
        DBG("Librarian: added sound id=" + juce::String(newId) + " file=" + filePath);
        return newId;
    }

    void Librarian::removeItem(int id)
    {
        m_entries.erase(
            std::remove_if(m_entries.begin(), m_entries.end(),
                [id](const SoundFileEntry& e) {return e.id == id; }),
            m_entries.end()
        );
        DBG("Librarian: removed sound id=" + juce::String(id));
    }

    void Librarian::saveSoundRegistry(const juce::String & jsonPath)
    {
        json output;
        output["sounds"] = json::array();

        for (const auto& entry : m_entries)
        {
            json soundEntry;
            soundEntry["id"] = entry.id;
            soundEntry["md5"] = entry.md5Name.toStdString();
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            juce::File soundFile(entry.filePath);
            soundEntry["filePath"] = soundFile.getRelativePathFrom(appDir).toStdString();
            output["sounds"].push_back(soundEntry);
        }

        output["ambient"] = json::array();
        for (const auto& entry : m_ambientEntries)
        {
            json ambientEntry;
            ambientEntry["id"] = entry.id;
            ambientEntry["role"] = entry.role.toStdString();
            ambientEntry["gain"] = entry.gain;
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            juce::File ambientFile(entry.filePath);
            ambientEntry["filePath"] = ambientFile.getRelativePathFrom(appDir).toStdString();
            output["ambient"].push_back(ambientEntry);
        }

        juce::File file = juce::File::isAbsolutePath(jsonPath)
            ? juce::File(jsonPath)
            : juce::File::getSpecialLocation(juce::File::currentApplicationFile)
            .getParentDirectory()
            .getChildFile(jsonPath);

        file.replaceWithText(output.dump(4)); //nice indentattion!
        DBG("Librarian: save registery to" + jsonPath);
    }

    void Librarian::loadSoundRegistry(const juce::String& jsonPath)
    {
        juce::File file = juce::File::isAbsolutePath(jsonPath)
            ? juce::File(jsonPath)
            : juce::File::getSpecialLocation(juce::File::currentApplicationFile)
            .getParentDirectory()
            .getChildFile(jsonPath);

        if (!file.existsAsFile())
        {
            DBG("Librarian: registry file not found: " + jsonPath);
            return;
        }

        json input = json::parse(file.loadFileAsString().toStdString());

        for (const auto& soundEntry : input["sounds"])
        {
            juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            juce::String filePath = appDir.getChildFile(
                juce::String(soundEntry["filePath"].get<std::string>())
            ).getFullPathName();
            juce::String md5 = juce::String(soundEntry["md5"].get<std::string>());
            int id = soundEntry["id"].get<int>();

            juce::File soundFile(filePath);
            FileProcessor fp(soundFile);
            SoundFileEntry entry = fp.process();

            if (entry.md5Name != md5)
            {
                DBG("Librarian: md5 mismatch for " + filePath + " file may have changed");
            }

            entry.id = id;
            m_entries.push_back(std::move(entry));

            // keep track of highest id
            if (id >= m_nextId)
                m_nextId = id + 1;
        }
        DBG("Librarian: loaded " + juce::String(m_entries.size()) + " sounds");

        // for the ambient sounds
        if (input.contains("ambient"))
        {
            for (const auto& ambientEntry : input["ambient"])
            {
                juce::File appDir = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
                juce::String filePath = appDir.getChildFile(
                    juce::String(ambientEntry["filePath"].get<std::string>())
                ).getFullPathName();

                int   id = ambientEntry["id"].get<int>();
                float gain = ambientEntry["gain"].get<float>();
                juce::String role = juce::String(ambientEntry["role"].get<std::string>());

                addAmbientItem(id, role, filePath, gain);
            }
            DBG("Librarian: loaded " + juce::String(m_ambientEntries.size()) + " ambient sounds");
        }
    }

    void Librarian::unloadSoundRegistry()
    {
        m_entries.clear();
        m_ambientEntries.clear();
        m_nextId = ReservedIds::DYNAMIC_START;

        DBG("Librarian: unloaded all sounds");
    }
    const juce::AudioBuffer<float>& Librarian::getBuffer(int id) const
    {
        for (const auto& entry : m_entries)
            if (entry.id == id)
                return entry.buffer;
    
        jassertfalse; //suggestion of ai because if id valid should never go here 
        return m_entries[0].buffer;
    }
    juce::String Librarian::getFilePath(int id) const
    {
        for (const auto& entry : m_entries)
            if (entry.id == id)
                return entry.filePath;

        return juce::String();
    }
    int Librarian::getCount() const
    {
        return (int)m_entries.size();
    }
    void Librarian::addAmbientItem(int id, const juce::String& role, const juce::String& filePath, float gain)
    {
        AmbientEntry entry;
        entry.id = id;
        entry.role = role;
        entry.filePath = filePath;
        entry.gain = gain;

        juce::File file = juce::File::isAbsolutePath(filePath)
            ? juce::File(filePath)
            : juce::File::getSpecialLocation(juce::File::currentApplicationFile)
            .getParentDirectory()
            .getChildFile(filePath);

        FileProcessor fp(file);
        auto processed = fp.process();
        entry.buffer = std::move(processed.buffer);

        m_ambientEntries.push_back(std::move(entry));
        DBG("Librarian: added ambient id=" + juce::String(id) + " role=" + role);

    }
    void Librarian::removeAmbientItem(int id)
    {
        m_ambientEntries.erase(
            std::remove_if(m_ambientEntries.begin(), m_ambientEntries.end(),
                [id](const AmbientEntry& e) {return e.id == id; }),
            m_ambientEntries.end()
        );
        DBG("Librarian: removed Ambient id=" + juce::String(id));
    }

    const std::vector<AmbientEntry>& Librarian::getAmbientEntries() const
    {
        return m_ambientEntries;
    }

    std::vector<AmbientEntry>& Librarian::getAmbientEntries()
    {
        return m_ambientEntries;
    }

    bool Librarian::isDuplicate_(const juce::String& md5) const
    {
        for (const auto& entry : m_entries)
            if (entry.md5Name == md5)
                return true;
        return false;
    }
    int Librarian::getExistingId_(const juce::String& md5) const
    {

        for (const auto& entry : m_entries)
            if (entry.md5Name == md5)
                return entry.id;
        return -1;
    }
    const std::vector<SoundFileEntry>& Librarian::getEntries() const
    {
        return m_entries;
    }
}