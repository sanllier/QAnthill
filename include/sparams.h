#ifndef SPARAMS_H
#define SPARAMS_H

#include "mpi.h"
#include "mpicheck.h"
#include "qworld.h"
#include "interfaces.h"

#include <string>
#include <vector>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

enum EColonyInitializationType
{
    COLONY_INIT_TYPE_RANDOM,
    COLONY_INIT_TYPE_POINT,
    COLONY_INIT_TYPE_UNSET
};

//------------------------------------------------------------

struct SParams
{
    long long cycThreshold;
    int cycStepsThreshold;
	int antsPerHill;
    int hillsNum;
	int procsPerNode;
    float heuristicInfluence;
    int target;
    EColonyInitializationType colonyInitType;
    int initPoint;
    float choosePathMethodConst;
    QWorld* world;
    std::string heuristicDataFile;
    IFitness* fClass;
    IRestriction* rClass;
    std::string outFile;

    SParams( MPI_Comm comm = MPI_COMM_NULL, const char* file = 0 );
    ~SParams();

    void initWithFile( MPI_Comm comm, const char* file );
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
