#pragma once
#include <array>
#include <cstdint>
#include <Eigen/Dense>

#include "AcousticResult.h"
#include "DTO_RecievedRay.h"

namespace acoustic {

// Un slot représente tous les rays arrivés dans une tranche de stepCount.
// stepCount est un proxy du temps de trajet : plus il est grand, plus le son
// a voyagé longtemps (réflexions tardives, réverb).
struct TimelineSlot
{
    Eigen::Vector3f weightedPos       = Eigen::Vector3f::Zero(); // somme(pos * w) — rays indirects
    Eigen::Vector3f directWeightedPos = Eigen::Vector3f::Zero(); // somme(pos * w) — rays directs uniquement
    Eigen::Vector3f weightedGain      = Eigen::Vector3f::Zero(); // somme(energy * w)
    float           totalWeight       = 0.f;
    float           directTotalWeight = 0.f;
    int             rayCount          = 0;
    bool            hasDirect         = false; // au moins un DTO isDirect dans ce slot
    bool            isSynthetic       = false; // slot reconstruit par fillGaps()

    bool isEmpty() const { return totalWeight < 1e-6f; }
    void reset();
};

// Timeline d'énergie acoustique pour un emitter donné.
// Chaque frame : decayAll() → addDTO() → fillGaps() → collapse()
class AcousticTimeline
{
public:
    // --- Paramètres de la timeline ---
    static constexpr int   SLOT_SIZE      = 8;    // steps par slot
    static constexpr int   MAX_SLOTS      = 32;   // couverture max : 32 × 8 = 256 steps
    static constexpr float SLOT_DECAY     = 0.88f; // décroissance d'énergie par frame
    static constexpr float GAP_DECAY      = 0.80f; // décroissance supplémentaire par slot vide

    // --- Poids temporels pour le collapse ---
    static constexpr float DIRECT_WEIGHT  = 12.0f; // son direct (isDirect=true)
    static constexpr int   EARLY_CUTOFF   = 4;     // slots 0..3 = réflexions précoces
    static constexpr float EARLY_WEIGHT   = 4.0f;
    static constexpr float LATE_WEIGHT    = 1.0f;

    // Place un DTO dans le slot correspondant à son stepCount.
    void addDTO(const DTO_RecievedRay& dto);

    // Fait vieillir tous les slots d'un frame (appelé avant addDTO).
    void decayAll();

    // Crée des slots synthétiques dans les blancs par décroissance depuis le
    // dernier slot réel. Appelé après addDTO, avant collapse.
    void fillGaps();

    // Calcule l'AcousticResult final depuis l'ensemble des slots.
    // - Azimuth/élévation : pondérés par (énergie × poids temporel), biais vers direct/précoce
    // - Gain : somme pondérée avec décroissance géométrique par index de slot
    // listenerYaw : angle de rotation Y de l'avatar (radians, depuis getHeadAngles().y).
    // Converti l'azimuth monde en azimuth relatif au regard du listener.
    // Direction is exponentially smoothed frame-to-frame to avoid batch-to-batch jumping.
    AcousticResult collapse(int emitterId, const Eigen::Vector3f& listenerPos, float listenerYaw);

    void clear();
    bool hasAnyData() const;

    // How fast the smoothed direction tracks new data: 0=frozen, 1=no smoothing.
    static constexpr float DIR_SMOOTH_ALPHA = 0.15f;

private:
    std::array<TimelineSlot, MAX_SLOTS> m_slots{};
    // Smoothed direction as (cos, sin) of azimuth — avoids ±180° wrap issues.
    Eigen::Vector2f m_smoothedDir  = Eigen::Vector2f(1.f, 0.f); // default: front
    float           m_smoothedElev = 0.f;
    bool            m_hasSmoothDir = false;
};

} // namespace acoustic
