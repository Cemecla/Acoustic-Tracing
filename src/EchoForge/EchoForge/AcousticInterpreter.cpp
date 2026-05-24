#include "AcousticInterpreter.h"

void AcousticInterpreter::feed(const std::vector<DTO_RecievedRay>& dtosThisFrame,
                                const Eigen::Vector3f& listenerPos,
                                float listenerYaw)
{
    // 1. Vieillissement : toutes les timelines existantes perdent de l'énergie.
    //    Doit être fait avant d'ajouter les nouveaux DTOs pour que les données
    //    fraîches de ce frame ne soient pas dégradées immédiatement.
    for (auto& [id, timeline] : m_timelines)
        timeline.decayAll();

    // 2. Distribution des DTOs dans les bons slots.
    for (const auto& dto : dtosThisFrame)
    {
        if (dto.getEmitterID() == AMBIENT_ID) continue; // non spatialisé

        int id = static_cast<int>(dto.getEmitterID());
        if (!m_knownEmitterIds.empty() && !m_knownEmitterIds.count(id)) continue;

        m_timelines[id].addDTO(dto);
    }

    // 3. Reconstruction des blancs + collapse pour chaque emitter connu.
    for (int id : m_knownEmitterIds)
    {
        auto& timeline = m_timelines[id];
        timeline.fillGaps();

        if (timeline.hasAnyData())
            m_results[id] = timeline.collapse(id, listenerPos, listenerYaw);
        else
            m_results.erase(id);
    }
}

void AcousticInterpreter::setKnownEmitters(const std::vector<int>& ids)
{
    m_knownEmitterIds = std::set<int>(ids.begin(), ids.end());
}

const std::map<int, AcousticResult>& AcousticInterpreter::getResults() const
{
    return m_results;
}

void AcousticInterpreter::clear()
{
    for (auto& [id, timeline] : m_timelines)
        timeline.clear();
    m_results.clear();
    m_knownEmitterIds.clear();
}
