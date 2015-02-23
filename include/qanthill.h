#ifndef QANTHILL_H
#define QANTHILL_H

#include <complex>

#include "sparams.h"
#include "qhill.h"
#include "qworld.h"
#include "mpi.h"
#include "mpicheck.h"
#include "defs.h"

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

static MPI_Comm QAnthillInit( int* argc, char*** argv );
static bool QAnthillInitialized();
static void QAnthillFinilize();

//-----------------------------------------------------------

class QAnthillProcess
{
public:
    QAnthillProcess( const SParams& params, MPI_Comm comm = MPI_COMM_WORLD );
    ~QAnthillProcess();

    double process();

    bool isMaster() const;

private:
    bool active() const;

private:
    SParams m_params;

    struct SQAnthillProcessContext;
    SQAnthillProcessContext* m_ctx;

	QHill* m_localHill;
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
