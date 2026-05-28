#include "AcousticTimeline.h"
#include <algorithm>
#include <cmath>
#include <numbers>

using namespace acoustic;

// ── TimelineSlot ─────────────────────────────────────────────────────────────

void TimelineSlot::reset()
{
    weightedPos       = Eigen::Vector3f::Zero();
    directWeightedPos = Eigen::Vector3f::Zero();
    weightedGain      = Eigen::Vector3f::Zero();
    totalWeight       = 0.f;
    directTotalWeight = 0.f;
    rayCount          = 0;
    hasDirect         = false;
    isSynthetic       = false;
}

// ── AcousticTimeline ─────────────────────────────────────────────────────────

void AcousticTimeline::addDTO(const DTO_RecievedRay& dto)
{
    int idx = dto.getStepCount() / SLOT_SIZE;
    if (idx >= MAX_SLOTS) idx = MAX_SLOTS - 1;

    auto& slot = m_slots[idx];
    float w = dto.getEnergy().norm();
    if (w < 1e-8f) return;

    if (dto.isDirect()) {
        // Ray direct : lastSeen = position exacte de l'émetteur → stocké séparément
        slot.directWeightedPos += dto.getLastSeen() * w;
        slot.directTotalWeight += w;
        slot.hasDirect          = true;
    } else {
        // Ray indirect : utilise le dernier point de rebond (r.getPosition()) comme position audio,
        // pas lastSeen (= emitter.position) qui sert uniquement à l'affichage visuel.
        const auto& bp = dto.getBouncePoints();
        Eigen::Vector3f audioPos = bp.empty() ? dto.getLastSeen() : bp.back();
        slot.weightedPos += audioPos * w;
    }
    slot.weightedGain += dto.getEnergy() * w;
    slot.totalWeight  += w;
    slot.rayCount++;
    slot.isSynthetic = false;
}

void AcousticTimeline::decayAll()
{
    for (auto& slot : m_slots)
    {
        if (slot.isEmpty()) continue;
        slot.weightedPos       *= SLOT_DECAY;
        slot.directWeightedPos *= SLOT_DECAY;
        slot.weightedGain      *= SLOT_DECAY;
        slot.totalWeight       *= SLOT_DECAY;
        slot.directTotalWeight *= SLOT_DECAY;
        if (slot.isEmpty()) slot.reset(); // purgé si trop faible
    }
}

void AcousticTimeline::fillGaps()
{
    // Efface d'abord les anciens synthétiques pour repartir propre.
    for (auto& slot : m_slots)
        if (slot.isSynthetic) slot.reset();

    // Passe forward : pour chaque slot vide, on decay depuis le slot précédent.
    // Crée un profil de décroissance exponentielle dans les blancs.
    const TimelineSlot* prev = nullptr;
    for (auto& slot : m_slots)
    {
        if (!slot.isEmpty())
        {
            prev = &slot;
        }
        else if (prev != nullptr)
        {
            slot.weightedPos  = prev->weightedPos  * GAP_DECAY; // ← idem : doit scaler avec totalWeight
            slot.weightedGain = prev->weightedGain * GAP_DECAY;
            slot.totalWeight  = prev->totalWeight  * GAP_DECAY;
            slot.hasDirect    = false;
            slot.isSynthetic  = true;
            prev = &slot; // chaîne : le prochain slot décadera depuis celui-ci
        }
    }
}

AcousticResult AcousticTimeline::collapse(int emitterId,
                                           const Eigen::Vector3f& listenerPos,
                                           float listenerYaw)
{
    AcousticResult result;
    result.emitterId = emitterId;

    Eigen::Vector3f dirWeightedPos = Eigen::Vector3f::Zero();
    float           dirTotalW      = 0.f;
    Eigen::Vector3f gainAccum      = Eigen::Vector3f::Zero();

    for (int i = 0; i < MAX_SLOTS; ++i)
    {
        const auto& slot = m_slots[i];
        if (slot.isEmpty()) continue;

        Eigen::Vector3f gain = slot.weightedGain / slot.totalWeight;

        // --- Direction : slots réels uniquement, position directe en priorité ---
        if (!slot.isSynthetic)
        {
            bool hasDir = false;
            Eigen::Vector3f pos = Eigen::Vector3f::Zero();
            float temporalW = 0.f;

            if (slot.hasDirect && slot.directTotalWeight > 1e-6f)
            {
                // Ray direct : position pure de l'émetteur, jamais contaminée par les rebonds
                pos       = slot.directWeightedPos / slot.directTotalWeight;
                temporalW = DIRECT_WEIGHT;
                hasDir    = true;
            }
            else
            {
                // Ray indirect : position du rebond (fallback)
                float indirectW = slot.totalWeight - slot.directTotalWeight;
                if (indirectW > 1e-6f)
                {
                    pos       = slot.weightedPos / indirectW;
                    temporalW = (i < EARLY_CUTOFF) ? EARLY_WEIGHT : LATE_WEIGHT;
                    hasDir    = true;
                }
            }

            if (hasDir)
            {
                float dirW  = temporalW * gain.norm();
                dirWeightedPos += pos * dirW;
                dirTotalW      += dirW;
            }
        }

        // --- Gain : tous les slots contribuent (réels + synthétiques = réverbération) ---
        // slotW penalizes later (farther) slots — do NOT divide by gainTotalW or the
        // decay cancels out and distance attenuation disappears entirely.
        float slotW = std::pow(0.78f, i);
        gainAccum  += gain  * slotW;
    }

    if (dirTotalW < 1e-6f) return result; // aucune donnée utilisable

    // Azimuth / élévation depuis la position moyenne pondérée
    Eigen::Vector3f avgPos = dirWeightedPos / dirTotalW;
    Eigen::Vector3f delta  = avgPos - listenerPos;
    float dist2D = std::sqrt(delta.x() * delta.x() + delta.z() * delta.z());

    // Raw world azimuth → local azimuth
    float worldAz = std::atan2(delta.x(), -delta.z());
    float localAz = worldAz + listenerYaw;

    // Elevation (raw — not smoothed, less sensitive to noise)
    float rawElev = std::atan2(delta.y(), dist2D);

    // Smooth direction as a (cos, sin) vector to handle ±π wrap correctly.
    // Wall-contact positions change every batch, so we track a running average
    // instead of using the raw angle directly.
    Eigen::Vector2f rawDir(std::cos(localAz), std::sin(localAz));
    if (!m_hasSmoothDir) {
        m_smoothedDir  = rawDir;
        m_smoothedElev = rawElev;
        m_hasSmoothDir = true;
    } else {
        m_smoothedDir  += DIR_SMOOTH_ALPHA * (rawDir - m_smoothedDir);
        m_smoothedElev += DIR_SMOOTH_ALPHA * (rawElev - m_smoothedElev);
        if (m_smoothedDir.norm() > 1e-6f) m_smoothedDir.normalize();
    }

    float smoothAz = std::atan2(m_smoothedDir.y(), m_smoothedDir.x());

    constexpr float RAD2DEG = 180.f / std::numbers::pi_v<float>;
    result.azimuth   = smoothAz  * RAD2DEG;
    result.elevation = m_smoothedElev * RAD2DEG;

    // gainAccum encodes distance via pow(0.78, slot): slot 0 = 1.0, farther slots decay.
    // cwiseMin clamps reverberant sums (many slots filled) at 1.
    result.gain = gainAccum.cwiseMin(1.f).cwiseMax(0.f);

    return result;
}

void AcousticTimeline::clear()
{
    for (auto& slot : m_slots) slot.reset();
    m_smoothedDir  = Eigen::Vector2f(1.f, 0.f);
    m_smoothedElev = 0.f;
    m_hasSmoothDir = false;
}

bool AcousticTimeline::hasAnyData() const
{
    return std::ranges::any_of(m_slots, [](const TimelineSlot& s){ return !s.isEmpty(); });
}
