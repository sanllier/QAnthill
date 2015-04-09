#ifndef INTERFACES_H
#define INTERFACES_H

#include "sqant.h"
#include "qworld.h"

#include <vector>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

class IFitness
{
    virtual ~IFitness() {}
    virtual BASETYPE operator()( const SQAnt::Trace& trace, const QWorld& world ) = 0;
};

//------------------------------------------------------------

class IRestriction
{
    virtual ~IRestriction() {}
    virtual const std::vector< bool >& operator()( int pos, const QWorld& world ) = 0;
};

//------------------------------------------------------------
}

//------------------------------------------------------------
#endif
