#pragma once
#include <Eigen/Dense>

struct AcousticResult
{
    int             emitterId = -1;
    float           azimuth   = 0.f;   // degrees, 0 = front, +90 = right
    float           elevation = 0.f;   // degrees, 0 = horizontal, +90 = above
    Eigen::Vector3f gain = { 1.f, 1.f, 1.f }; // {HF, MF, LF}
    float avgPathLength = 0.f;
    bool hasDirect = false;
};
