/*
  ==============================================================================

    CircularStereoBuffer.cpp
    Created: 6 Mar 2026 10:20:26am
    Author:  yutai

  ==============================================================================
*/

#include "CircularStereoBuffer.h"
namespace YutaSound
{

    void CircularStereoBuffer::prepare(int capacity) // caller passes bufferSize + 1
    {

        m_buffer.setSize(2, capacity);
        m_buffer.clear();
        m_fifo.setTotalSize(capacity);
    }

    void CircularStereoBuffer::write(float* left, float* right, int numSamples)
    {
        const auto scope = m_fifo.write(numSamples);

        if (scope.blockSize1 > 0)
        {
            //    std::copy(left, left + scope.blockSize1, m_bufferL.data() + scope.startIndex1); // .data = pointer  other option was begin witch is iterator (arthmetic)
            //    std::copy(right, right + scope.blockSize1, m_bufferR.data() + scope.startIndex1);
            m_buffer.copyFrom(0, scope.startIndex1, left, scope.blockSize1);
            m_buffer.copyFrom(1, scope.startIndex1, right, scope.blockSize1);

        }
        if (scope.blockSize2 > 0)
        {
            //    std::copy(left + scope.blockSize1, left + scope.blockSize1 + scope.blockSize2, m_bufferL.data() + scope.blockSize2);
            //    std::copy(right + scope.blockSize1, right + scope.blockSize1 + scope.blockSize2, m_bufferR.data() + scope.blockSize2);
            m_buffer.copyFrom(0, scope.startIndex2, left + scope.blockSize1, scope.blockSize2);
            m_buffer.copyFrom(1, scope.startIndex2, right + scope.blockSize1, scope.blockSize2);
        }
    }

    void CircularStereoBuffer::read(float* left, float* right, int numSamples)
    {
        if (m_fifo.getNumReady() < numSamples)
        {
            juce::FloatVectorOperations::clear(left, numSamples);
            juce::FloatVectorOperations::clear(right, numSamples);
            return; // not enough data yet
        }
        const auto scope = m_fifo.read(numSamples);

        if (scope.blockSize1 > 0)
        {
            //copySomeData(someData, myBuffer + scope.startIndex1, scope.blockSize1); this juce cpy order is non correct i used copy from c++ has diff order 
            std::copy(m_buffer.getReadPointer(0, scope.startIndex1), m_buffer.getReadPointer(0, scope.startIndex1) + scope.blockSize1, left);
            std::copy(m_buffer.getReadPointer(1, scope.startIndex1), m_buffer.getReadPointer(1, scope.startIndex1) + scope.blockSize1, right);
            //std::copy(m_bufferR.data() + scope.startIndex1, m_bufferR.data() + scope.startIndex1 + scope.blockSize1, right);
        }

        if (scope.blockSize2 > 0)
        {
            //std::copy(m_bufferL.data() + scope.startIndex2, m_bufferL.data() + scope.startIndex2 + scope.blockSize2, left + scope.blockSize1);
            //std::copy(m_bufferR.data() + scope.startIndex2, m_bufferR.data() + scope.startIndex2 + scope.blockSize2, right + scope.blockSize1);
            std::copy(m_buffer.getReadPointer(0, scope.startIndex2), m_buffer.getReadPointer(0, scope.startIndex2) + scope.blockSize2, left + scope.blockSize1);
            std::copy(m_buffer.getReadPointer(1, scope.startIndex2), m_buffer.getReadPointer(1, scope.startIndex2) + scope.blockSize2, right + scope.blockSize1);
        }
    }
}





//JUCE copySomeData :
//(destination, source, size)
//(source_begin, source_end, destination)
