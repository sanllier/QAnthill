#ifndef SQANT_H	
#define SQANT_H

#include "mpi.h"
#include <vector>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

struct SQAnt
{
public:
    class Trace
    {
    public:
        Trace()
            : m_size(0)
            , m_tracePos(0)
        {}

        void reset() { m_tracePos = 0; }
        long long getValidLen() const { return m_tracePos; } 
        std::vector< int > getTrace() { return m_trace; }
        const std::vector< int > getTrace() const { return m_trace; }
        void add( const int& pos ) 
        {
            if ( m_size < m_tracePos )
            {
                ++m_size;
                m_trace.push_back( pos );
            }
            else
            {
                m_trace[ m_tracePos ] = pos;
            }
            ++m_tracePos;
        }

        std::vector< int >::iterator begin() { return m_trace.begin(); }
        std::vector< int >::iterator end()   { return m_trace.begin() + m_tracePos; }

    private:
        std::vector< int > m_trace;
        int m_size;
        int m_tracePos;
    };

public:
    int   curPos;
    Trace trace;
};

//------------------------------------------------------------
}

//------------------------------------------------------------
#endif
