#ifndef QBIT_H
#define QBIT_H

#include "defs.h"
#include "randomizer.h"

#include <math.h>
#include <complex>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

typedef std::complex< BASETYPE > QComplex;

//------------------------------------------------------------

struct QBit
{
    QComplex a;
    QComplex b;

    QBit() {}
    QBit( QComplex a, QComplex b )
        : a(a)
        , b(b)
    {}

    bool measure() const
    {
        BASETYPE mod = fabs( a.real() * a.real() + a.imag() * a.imag() );
	    return Randomizer::Get()->nextBasetype() >= mod;
    }
};

//------------------------------------------------------------
}

//------------------------------------------------------------
#endif
