/*
  ==============================================================================

    CircularStereoBuffer.cpp
    Created: 6 Mar 2026 10:20:26am
    Author:  yutai

  ==============================================================================
*/

#include "CircularStereoBuffer.h"
using namespace YutaSound;

void YutaSound::CircularStereoBuffer::prepare(int bufferSize)
{
}

void YutaSound::CircularStereoBuffer::write(float* left, float* right, int numSamples)
{
    const auto scope = m_fifo.write(numSamples);

    if (scope.blockSize1>0)
        std::copy(left, left + scope.startIndex1,m_bufferL.data() + scope.startIndex1); // .data = pointer  other option was begin witch is iterator (arthmetic) 
        std::copy(right, right + scope.startIndex1, m_bufferR.data() + scope.startIndex1);

    if (scope.blockSize1 > 0)
        std::copy(left + scope.blockSize1, left + scope.blockSize1 + scope.blockSize2, m_bufferL.begin() + scope.blockSize2); 
        std::copy(right + scope.blockSize1, right + scope.blockSize1 + scope.blockSize2, m_bufferR.begin() + scope.blockSize2);
}

void YutaSound::CircularStereoBuffer::read(float* left, float* right, int numSamples)
{
    const auto scope = m_fifo.write(numSamples);

}
