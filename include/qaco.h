#ifndef QACO_H
#define QACO_H

#include "randomizer.h"
#include "sparams.h"
#include "qhill.h"
#include "qworld.h"
#include "mpi.h"
#include "mpicheck.h"
#include "defs.h"

//------------------------------------------------------------

namespace QAnthill {
//-----------------------------------------------------------

class QACOProcess
{
public:
    QACOProcess( const SParams& params, MPI_Comm comm = MPI_COMM_WORLD );
    ~QACOProcess();

    double process();

    bool isMaster() const;

private:
    bool active() const;

private:
    SParams m_params;

    struct SQACOProcessContext;
    SQACOProcessContext* m_ctx;

	QHill* m_localHill;

    Randomizer* m_randomizer;
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
