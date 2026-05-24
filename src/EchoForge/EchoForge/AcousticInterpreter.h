#pragma once
#include <map>
#include <set>
#include <vector>
#include <Eigen/Dense>

#include "AcousticResult.h"
#include "AcousticTimeline.h"
#include "DTO_RecievedRay.h"

// AcousticInterpreter — convertit les DTO_RecievedRay en AcousticResult spatiaux.
//
// Approche : réponse impulsionnelle sparse par emitter.
// Chaque DTO est placé dans un slot de sa timeline selon son stepCount (proxy
// du temps de trajet). Les blancs sont comblés par décroissance simulée.
// La direction (azimuth/élévation) est biaisée vers les sons directs/précoces.
//
// Cycle par frame (appelé depuis EFModel::processBatch) :
//   feed(dtosThisFrame, listenerPos)
//     → decayAll()  : vieillit les timelines
//     → addDTO()    : distribue les nouveaux DTOs
//     → fillGaps()  : reconstruit les blancs par décroissance
//     → collapse()  : produit l'AcousticResult final par emitter
class AcousticInterpreter
{
public:
    AcousticInterpreter() = default;

    // Distribue les DTOs de ce frame, met à jour les timelines et recalcule
    // les résultats. À appeler une fois par frame.
    // listenerYaw : getHeadAngles().y de l'avatar (radians).
    void feed(const std::vector<DTO_RecievedRay>& dtosThisFrame,
              const Eigen::Vector3f& listenerPos,
              float listenerYaw);

    // Enregistre les IDs d'emitters connus. Les DTOs d'IDs inconnus sont ignorés.
    void setKnownEmitters(const std::vector<int>& ids);

    void clear();

    // Résultats les plus récents, un par emitter actif.
    const std::map<int, AcousticResult>& getResults() const;

private:
    static constexpr std::uint8_t AMBIENT_ID = 255; // filtré, non spatialisé

    std::set<int>                              m_knownEmitterIds;
    std::map<int, acoustic::AcousticTimeline>  m_timelines;
    std::map<int, AcousticResult>              m_results;
};
