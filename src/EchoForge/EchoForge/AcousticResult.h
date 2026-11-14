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
    // 0..1, share of the arriving energy that comes late (after the first EARLY_CUTOFF slots,
    // counted from the first arrival). High in reverberant rooms, low in open/absorbing ones.
    float reverb = 0.f;
    // 0..1, average energy left in the arriving rays (wall absorption only, no distance weighting).
    float rayEnergy = 0.f;
};
