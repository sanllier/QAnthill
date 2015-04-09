#ifndef QHILL_H	
#define QHILL_H

#include "mpi.h"
#include "qworld.h"
#include "sparams.h"
#include "sqant.h"

#include <vector>
#include <string>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

class QHill
{
public:
    QHill( MPI_Comm baseComm, const SParams& params );
	~QHill();

    void run();

private:
	MPI_Comm m_comm;

    QWorld* m_world;
    std::vector< SQAnt > m_hill;

    float m_heuristicInfluence;
    float m_pheromoneInfluence;
    float m_choosePathConst;
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
