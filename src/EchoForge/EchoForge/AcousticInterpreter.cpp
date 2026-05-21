#include "AcousticInterpreter.h"
#include <cmath>
#include <numbers>
#include <set>

AcousticInterpreter::AcousticInterpreter(int quantityThreshold, int deltaFrame, float smoothingFactor)
    : m_quantityThreshold(quantityThreshold)
    , m_deltaFrame(deltaFrame)
    , m_smoothingFactor(smoothingFactor)
{}

void AcousticInterpreter::feed(const std::vector<DTO_RecievedRay>& dtosThisFrame,
                                const Eigen::Vector3f& listenerPos)
{
    for (const auto& dto : dtosThisFrame)
    {
        int id = static_cast<int>(dto.getEmitterID());
        m_accumulator[id].push_back(dto);
    }

    ++m_frameCount;

    int totalAccumulated = 0;
    for (const auto& [id, batch] : m_accumulator)
        totalAccumulated += static_cast<int>(batch.size());

    bool deltaReached    = m_frameCount >= m_deltaFrame;
    bool quantityReached = totalAccumulated >= m_quantityThreshold;

    if (deltaReached || quantityReached)
        flush(listenerPos);
}

const std::map<int, AcousticResult>& AcousticInterpreter::getResults() const
{
    return m_smoothed;
}

void AcousticInterpreter::flush(const Eigen::Vector3f& listenerPos)
{
    std::set<int> activeIds;

    for (auto& [id, batch] : m_accumulator)
    {
        if (batch.empty()) continue;
        if (!m_knownEmitterIds.empty() && !m_knownEmitterIds.count(id)) continue;

        activeIds.insert(id);
        AcousticResult raw = compute(id, batch, listenerPos);

        auto it = m_smoothed.find(id);
        if (it == m_smoothed.end())
        {
            m_smoothed[id] = raw;
        }
        else
        {
            float batchWeight = std::min(1.f, static_cast<float>(batch.size()) / m_quantityThreshold);
            float t = m_smoothingFactor + batchWeight * (1.f - m_smoothingFactor);
            lerpResult(it->second, raw, t);
        }

        batch.clear();
    }

    // Emitters déjà connus mais sans rays ce cycle -> fade out
    for (auto& [id, result] : m_smoothed)
    {
        if (!activeIds.count(id))
            result.gain *= (1.f - m_smoothingFactor);
    }

    m_frameCount = 0;
}

void AcousticInterpreter::setKnownEmitters(const std::vector<int>& ids)
{
    m_knownEmitterIds = std::set<int>(ids.begin(), ids.end());
}

AcousticResult AcousticInterpreter::compute(int emitterId,
                                             const std::vector<DTO_RecievedRay>& batch,
                                             const Eigen::Vector3f& listenerPos) const
{
    Eigen::Vector3f weightedPos = Eigen::Vector3f::Zero();
    Eigen::Vector3f weightedGain = Eigen::Vector3f::Zero();
    float totalWeight = 0.f;

    for (const auto& dto : batch)
    {
        float w = dto.getEnergy().norm();
        weightedPos  += dto.getLastSeen() * w;
        weightedGain += dto.getEnergy()   * w;
        totalWeight  += w;
    }

    AcousticResult result;
    result.emitterId = emitterId;

    if (totalWeight < 1e-6f)
        return result;

    weightedPos  /= totalWeight;
    weightedGain /= totalWeight;
    result.gain   = weightedGain;

    Eigen::Vector3f delta = weightedPos - listenerPos;
    float dist2D = std::sqrt(delta.x() * delta.x() + delta.z() * delta.z());

    result.azimuth   = std::atan2(delta.x(), -delta.z())
                       * (180.f / std::numbers::pi_v<float>);
    result.elevation = std::atan2(delta.y(), dist2D)
                       * (180.f / std::numbers::pi_v<float>);

    return result;
}

void AcousticInterpreter::lerpResult(AcousticResult& current,
                                      const AcousticResult& target,
                                      float t) const
{
    auto lerpAngle = [](float a, float b, float t) {
        float diff = b - a;
        while (diff >  180.f) diff -= 360.f;
        while (diff < -180.f) diff += 360.f;
        return a + diff * t;
    };

    current.azimuth   = lerpAngle(current.azimuth,   target.azimuth,   t);
    current.elevation = current.elevation + (target.elevation - current.elevation) * t;
    current.gain      = current.gain      + (target.gain      - current.gain)      * t;
}
