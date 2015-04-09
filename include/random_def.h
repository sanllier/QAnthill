#ifndef RANDOM_DEF_H
#define RANDOM_DEF_H

#include "random.h"
#include <stdlib.h>

//------------------------------------------------------------

class RandomDefault: public IRandom
{
public:
    void setSeed( unsigned seed ) OVERRIDE
    {
        srand( seed );
    }

    BASETYPE nextBasetype() OVERRIDE
    {
        return BASETYPE( rand() ) / RAND_MAX;
    }

    int nextInt() OVERRIDE
    {
        return rand();
    }
};

//------------------------------------------------------------
#endif
