#ifndef SPARAMS_H
#define SPARAMS_H

#include "mpi.h"
#include "mpicheck.h"
#include "qworld.h"

#include <string>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

struct SParams
{
    long long cycThreshold;
	int antsPerHill;
    int hillsNum;
	int procsPerNode;
    float heuristicInfluence;
    QWorld* world;
    std::string heuristicDataFile;
    std::string outFile;

    SParams( MPI_Comm comm = MPI_COMM_NULL, const char* file = 0 );

    void initWithFile( MPI_Comm comm, const char* file );
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
