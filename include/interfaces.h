#ifndef INTERFACES_H
#define INTERFACES_H

#include "sqant.h"
#include "qworld.h"

#include <vector>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

class IRestrition
{
    virtual ~IRestrition() {}
    virtual const std::vector< bool >& restricts( const SQAnt& ant, const std::vector< QWorld::SNode >& siblings ) = 0;
};

//------------------------------------------------------------

class IFitness
{
    virtual ~IFitness() {}
    virtual const BASETYPE compute( const SQAnt::Trace& trace, const QWorld& world ) = 0;
};

//------------------------------------------------------------
}

//------------------------------------------------------------
#endif
