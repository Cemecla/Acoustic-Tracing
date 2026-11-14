/*
  ==============================================================================

    FilterChain.cpp
    Created: 27 Apr 2026 10:22:41am
    Author:  Administrateur

  ==============================================================================
*/

#include "FilterChain.h"
#include <complex>

namespace {
    constexpr size_t kNumPinnaBands = 7;
    using BandArray = std::array<double, kNumPinnaBands>;

    constexpr BandArray kPinnaFreqs { 4000.0, 6000.0, 8000.0, 10000.0, 12000.0, 14000.0, 16000.0 };

    // Every band is ~2.8 kHz wide, a bit wider than the 2 kHz spacing, so the bands blend into a
    // smooth curve. Constant width means Q = f / width: the higher bands get a higher Q.
    constexpr double kPinnaBandwidthHz = 2800.0;

    // safety limit for the corrected filter gains (preset values are within -8..+3 dB)
    constexpr double kMaxPinnaFilterGainDb = 12.0;

    double pinnaQ(size_t band)
    {
        return kPinnaFreqs[band] / kPinnaBandwidthHz;
    }

    // magnitude in dB of one pinna peak filter at frequency f (same formula as JUCE's makePeakFilter)
    double peakResponseDb(double sampleRate, size_t band, double gainDb, double f)
    {
        const auto c = juce::dsp::IIR::ArrayCoefficients<double>::makePeakFilter(
            sampleRate, kPinnaFreqs[band], pinnaQ(band), juce::Decibels::decibelsToGain(gainDb));
        const auto z = std::polar(1.0, -juce::MathConstants<double>::twoPi * f / sampleRate);
        const auto h = (c[0] + c[1] * z + c[2] * z * z) / (c[3] + c[4] * z + c[5] * z * z);
        return juce::Decibels::gainToDecibels(std::abs(h), -200.0);
    }

    // solves a * x = b (Gaussian elimination with partial pivoting); b is replaced by x
    bool solveLinear(std::array<BandArray, kNumPinnaBands> a, BandArray& b)
    {
        constexpr size_t n = kNumPinnaBands;
        for (size_t col = 0; col < n; ++col)
        {
            size_t pivot = col;
            for (size_t r = col + 1; r < n; ++r)
                if (std::abs(a[r][col]) > std::abs(a[pivot][col]))
                    pivot = r;
            if (std::abs(a[pivot][col]) < 1e-12)
                return false;
            std::swap(a[col], a[pivot]);
            std::swap(b[col], b[pivot]);

            for (size_t r = col + 1; r < n; ++r)
            {
                const double factor = a[r][col] / a[col][col];
                for (size_t c = col; c < n; ++c)
                    a[r][c] -= factor * a[col][c];
                b[r] -= factor * b[col];
            }
        }
        for (size_t i = n; i-- > 0;)
        {
            double sum = b[i];
            for (size_t c = i + 1; c < n; ++c)
                sum -= a[i][c] * b[c];
            b[i] = sum / a[i][i];
        }
        return true;
    }

    // The bands overlap, so at each centre frequency the dB gains of the neighbouring bands add up
    // (with the old Q = 1 the real EQ was about 2x the preset values). Newton iterations find the
    // filter gains whose combined response equals the preset value at every centre frequency.
    BandArray correctedPinnaGains(double sampleRate, const BandArray& targetDb)
    {
        BandArray gains = targetDb;

        for (int iteration = 0; iteration < 6; ++iteration)
        {
            BandArray error {};
            double maxError = 0.0;
            for (size_t i = 0; i < kNumPinnaBands; ++i)
            {
                double response = 0.0;
                for (size_t k = 0; k < kNumPinnaBands; ++k)
                    response += peakResponseDb(sampleRate, k, gains[k], kPinnaFreqs[i]);
                error[i] = targetDb[i] - response;
                maxError = std::max(maxError, std::abs(error[i]));
            }
            if (maxError < 0.01)
                break;

            // jacobian[i][k] = how many dB the response at centre i moves per dB of filter k
            std::array<BandArray, kNumPinnaBands> jacobian {};
            constexpr double step = 0.1;
            for (size_t k = 0; k < kNumPinnaBands; ++k)
                for (size_t i = 0; i < kNumPinnaBands; ++i)
                    jacobian[i][k] = (peakResponseDb(sampleRate, k, gains[k] + step, kPinnaFreqs[i])
                                    - peakResponseDb(sampleRate, k, gains[k] - step, kPinnaFreqs[i]))
                                   / (2.0 * step);

            if (!solveLinear(jacobian, error))
                break;
            for (size_t k = 0; k < kNumPinnaBands; ++k)
                gains[k] = juce::jlimit(-kMaxPinnaFilterGainDb, kMaxPinnaFilterGainDb, gains[k] + error[k]);
        }
        return gains;
    }
}

namespace yutasound {
    void FilterChain::prepare(double sampleRate, int bufferSize)
    {
        m_sampleRate = sampleRate;

        m_svfILD.prepare(sampleRate, bufferSize);

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = (juce::uint32)bufferSize;
        spec.numChannels = 1;

        for (auto& filter : m_pinnaFiltersL)
            filter.prepare(spec);
        for (auto& filter : m_pinnaFiltersR)
            filter.prepare(spec);

        m_crossover.prepare(sampleRate, bufferSize);

        m_isDirty = false;
    }

    void FilterChain::setParameters(float azimuth, float elevation, PinnaPreset preset)
    {
        m_elevation = elevation;
        setAzimuth(azimuth);
        setPinnaEQ(preset);
    }

    StereoSample FilterChain::processSample(float left, float right)
    {
        auto stereo = m_svfILD.processSample(left, right);

        for (size_t b = 0; b < m_pinnaFiltersL.size(); b++)
        {
            stereo.left = m_pinnaFiltersL[b].processSample(stereo.left);
            stereo.right = m_pinnaFiltersR[b].processSample(stereo.right);
        }

        stereo = m_crossover.processSample(stereo.left, stereo.right);

        return stereo;
    }

    bool FilterChain::isDirty()const
    {
        return m_isDirty;
    }
   
    void FilterChain::clearDirty()
    {
        m_isDirty = false;
    }
    void FilterChain::resetState()
    {
        for (auto& filter : m_pinnaFiltersL)
            filter.reset();
        for (auto& filter : m_pinnaFiltersR)
            filter.reset();
        m_svfILD.resetState();
        m_crossover.reset();
    }
    void FilterChain::warmUp()
    {
        for (int n = 0; n < 64; n++)
            processSample(0.0f, 0.0f);
    }
    void FilterChain::setAzimuth(float azimuth)
    {
        m_azimuth = azimuth;
        m_svfILD.setAzimuth(azimuth);
    }
    void FilterChain::setPinnaEQ(PinnaPreset preset)
    {
        const BandArray targetDb {
            preset.gain4000,  preset.gain6000,  preset.gain8000,
            preset.gain10000, preset.gain12000, preset.gain14000,
            preset.gain16000
        };

        // every chain receives the same preset, so the solve is only done once per preset change
        thread_local double    cachedSampleRate = 0.0;
        thread_local BandArray cachedTargetDb {};
        thread_local BandArray cachedFilterGainsDb {};
        if (cachedSampleRate != m_sampleRate || cachedTargetDb != targetDb)
        {
            cachedFilterGainsDb = correctedPinnaGains(m_sampleRate, targetDb);
            cachedSampleRate = m_sampleRate;
            cachedTargetDb = targetDb;
        }

        static_assert(std::tuple_size_v<decltype(m_pinnaFiltersL)> == kNumPinnaBands);
        for (size_t b = 0; b < kNumPinnaBands; b++)
        {
            auto coefficients = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                m_sampleRate, (float)kPinnaFreqs[b], (float)pinnaQ(b),
                juce::Decibels::decibelsToGain((float)cachedFilterGainsDb[b]));
            *m_pinnaFiltersL[b].coefficients = *coefficients;
            *m_pinnaFiltersR[b].coefficients = *coefficients;
        }
        m_isDirty = true; // signal SAE to swap at next block boundary
    }
    void FilterChain::setGains(ILDGain gains)
    {
        m_crossover.setGains(gains);
    }
}