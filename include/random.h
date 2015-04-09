#ifndef RANDOM_H
#define RANDOM_H

#include "defs.h"

//------------------------------------------------------------

class IRandom
{
public:
    virtual ~IRandom() {};

    virtual void setSeed( unsigned seed ) = 0;
    virtual BASETYPE nextBasetype() = 0;
    virtual int nextInt() = 0;
};

//------------------------------------------------------------
#endif
