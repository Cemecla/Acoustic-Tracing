#include "AcousticInterpreter.h"
#include <cmath>
#include <numbers>

AcousticInterpreter::AcousticInterpreter(int windowSize, float decayPerFrame, float smoothingFactor)
    : m_windowSize(windowSize)
    , m_decayPerFrame(decayPerFrame)
    , m_smoothingFactor(smoothingFactor)
{}

void AcousticInterpreter::setUnitSize(float unitSize)
{
    m_unitSize = unitSize;
}

void AcousticInterpreter::setKnownEmitters(const std::vector<int>& ids)
{
    m_knownEmitterIds = std::set<int>(ids.begin(), ids.end());

    // Pré-initialiser tous les émetteurs à gain=0
    // Sans ça, updateSoundSource n'est jamais appelé avant le 1er ray
    // et l'audio engine joue au gain par défaut (fort)
    for (int id : m_knownEmitterIds)
    {
        if (m_smoothed.find(id) == m_smoothed.end())
        {
            AcousticResult zero{};
            zero.emitterId = id;
            zero.gain      = Eigen::Vector3f::Zero();
            m_smoothed[id] = zero;
        }
    }
}

void AcousticInterpreter::feed(const std::vector<DTO_RecievedRay>& dtosThisFrame,
                                const Eigen::Vector3f& listenerPos)
{
    // 1. Ajouter les nouveaux rays à leur timeline (age=0, travelTimeMs calculé)
    for (const auto& dto : dtosThisFrame)
    {
        int id = static_cast<int>(dto.getEmitterID());
        if (!m_knownEmitterIds.empty() && !m_knownEmitterIds.count(id)) continue;

        float travelTimeMs = dto.getStepCount() * m_unitSize / SPEED_OF_SOUND * 1000.f;
        m_timelines[id].push_back({dto, 0, travelTimeMs});
    }

    // 2. Vieillir tous les rays et expirer les trop anciens
    for (auto& [id, timeline] : m_timelines)
    {
        for (auto& tr : timeline)
            ++tr.age;

        while (!timeline.empty() && timeline.front().age > m_windowSize)
            timeline.pop_front();
    }

    // 3. Pour chaque émetteur connu : calculer et lisser
    for (int id : m_knownEmitterIds)
    {
        auto timelineIt = m_timelines.find(id);
        bool hasRays = (timelineIt != m_timelines.end() && !timelineIt->second.empty());

        if (hasRays)
        {
            m_silenceFrames[id] = 0;
            AcousticResult raw = compute(id, timelineIt->second, listenerPos);
            lerpResult(m_smoothed[id], raw, m_smoothingFactor, 0.6f);
        }
        else
        {
            int& frames = m_silenceFrames[id];
            ++frames;
            if (frames >= SILENCE_THRESHOLD)
            {
                auto it = m_smoothed.find(id);
                if (it != m_smoothed.end())
                {
                    it->second.gain *= SILENCE_DECAY;
                    if (it->second.gain.maxCoeff() < 0.001f)
                        it->second.gain = Eigen::Vector3f::Zero();
                }
            }
        }
    }
}

const std::map<int, AcousticResult>& AcousticInterpreter::getResults() const
{
    return m_smoothed;
}

void AcousticInterpreter::clear()
{
    m_timelines.clear();
    m_smoothed.clear();
    m_knownEmitterIds.clear();
    m_silenceFrames.clear();
}

AcousticResult AcousticInterpreter::compute(int emitterId,
                                             const std::deque<TimedRay>& timeline,
                                             const Eigen::Vector3f& listenerPos) const
{
    constexpr float RAD2DEG = 180.f / std::numbers::pi_v<float>;

    Eigen::Vector3f earlyWeightedGain = Eigen::Vector3f::Zero();
    float earlyTotalWeight = 0.f;
    Eigen::Vector3f lateWeightedGain = Eigen::Vector3f::Zero();
    float lateTotalWeight = 0.f;
    Eigen::Vector3f emitterPosition = Eigen::Vector3f::Zero();

    for (const auto& tr : timeline)
    {
        float decay = std::pow(m_decayPerFrame, static_cast<float>(tr.age));
        float w = tr.dto.getEnergy().norm() * decay;
        if (w < 1e-6f) continue;

        emitterPosition = tr.dto.getEmitterPosition();

        if (tr.travelTimeMs <= EARLY_TIME_MS)
        {
            earlyWeightedGain += tr.dto.getEnergy() * w;
            earlyTotalWeight  += w;
        }
        else
        {
            lateWeightedGain += tr.dto.getEnergy() * w;
            lateTotalWeight  += w;
        }
    }

    AcousticResult result;
    result.emitterId = emitterId;

    float totalWeight = earlyTotalWeight + lateTotalWeight;
    if (totalWeight < 1e-6f) return result;

    Eigen::Vector3f totalGain = Eigen::Vector3f::Zero();
    if (earlyTotalWeight > 1e-6f)
        totalGain += (earlyWeightedGain / earlyTotalWeight) * (earlyTotalWeight / totalWeight);
    if (lateTotalWeight > 1e-6f)
        totalGain += (lateWeightedGain / lateTotalWeight) * (lateTotalWeight / totalWeight);

    result.gain = totalGain.array().sqrt();

    // Direction géométrique (stable, pas de snaps)
    Eigen::Vector3f finalDir = (emitterPosition - listenerPos).normalized();
    float dist2D = std::sqrt(finalDir.x() * finalDir.x() + finalDir.z() * finalDir.z());
    result.azimuth   = std::atan2(finalDir.x(), -finalDir.z()) * RAD2DEG;
    result.elevation = std::atan2(finalDir.y(), dist2D)        * RAD2DEG;

    return result;
}

void AcousticInterpreter::lerpResult(AcousticResult& current,
                                      const AcousticResult& target,
                                      float tGain, float tPos) const
{
    auto lerpAngle = [](float a, float b, float t) {
        float diff = b - a;
        while (diff >  180.f) diff -= 360.f;
        while (diff < -180.f) diff += 360.f;
        return a + diff * t;
    };

    current.azimuth   = lerpAngle(current.azimuth, target.azimuth, tPos);
    current.elevation = current.elevation + (target.elevation - current.elevation) * tPos;
    current.gain      = current.gain + (target.gain - current.gain) * tGain;
    if (current.gain.maxCoeff() < 0.001f)
        current.gain = Eigen::Vector3f::Zero();
}
