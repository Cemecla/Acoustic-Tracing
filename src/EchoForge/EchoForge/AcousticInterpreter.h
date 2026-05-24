#pragma once
#include <deque>
#include <map>
#include <set>
#include <vector>
#include <Eigen/Dense>

#include "AcousticResult.h"
#include "DTO_RecievedRay.h"

class AcousticInterpreter
{
public:
    AcousticInterpreter(int windowSize        = 20,
                        float decayPerFrame   = 0.85f,
                        float smoothingFactor = 0.15f);

    void feed(const std::vector<DTO_RecievedRay>& dtosThisFrame,
              const Eigen::Vector3f& listenerPos);

    void setKnownEmitters(const std::vector<int>& ids);
    void setUnitSize(float unitSize);
    void clear();

    const std::map<int, AcousticResult>& getResults() const;

private:
    int   m_windowSize;
    float m_decayPerFrame;
    float m_smoothingFactor;
    float m_unitSize = 1.f;

    struct TimedRay {
        DTO_RecievedRay dto;
        int   age;          // frames depuis la réception (fraîcheur)
        float travelTimeMs; // temps de parcours acoustique (ms)
    };

    std::set<int>                           m_knownEmitterIds;
    std::map<int, std::deque<TimedRay>>     m_timelines;
    std::map<int, AcousticResult>           m_smoothed;
    std::map<int, int>                      m_silenceFrames; // frames consécutives sans rays

    static constexpr float SILENCE_DECAY        = 0.5f;  // multiplicateur par frame quand vraiment silencieux
    static constexpr int   SILENCE_THRESHOLD    = 30;    // frames consécutives vides avant de fader
    static constexpr float SPEED_OF_SOUND       = 343.f; // m/s
    static constexpr float EARLY_TIME_MS        = 20.f;  // seuil early/late en ms

    AcousticResult compute(int emitterId,
                           const std::deque<TimedRay>& timeline,
                           const Eigen::Vector3f& listenerPos) const;

    void lerpResult(AcousticResult& current, const AcousticResult& target,
                    float tGain, float tPos) const;
};
