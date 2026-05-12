#pragma once
#include <map>
#include <vector>
#include <Eigen/Dense>

#include "AcousticResult.h"
#include "DTO_RecievedRay.h"

class AcousticInterpreter
{
public:
    AcousticInterpreter(int quantityThreshold = 10,
                        int deltaFrame        = 8,
                        float smoothingFactor = 0.05f);

    void feed(const std::vector<DTO_RecievedRay>& dtosThisFrame,
              const Eigen::Vector3f& listenerPos);

    const std::map<int, AcousticResult>& getResults() const;

private:
    int   m_quantityThreshold;
    int   m_deltaFrame;
    float m_smoothingFactor;

    std::map<int, std::vector<DTO_RecievedRay>> m_accumulator;
    std::map<int, AcousticResult>               m_smoothed;
    int m_frameCount = 0;

    void   flush(const Eigen::Vector3f& listenerPos);
    AcousticResult compute(int emitterId,
                           const std::vector<DTO_RecievedRay>& batch,
                           const Eigen::Vector3f& listenerPos) const;
    void   lerpResult(AcousticResult& current, const AcousticResult& target, float t) const;
};
