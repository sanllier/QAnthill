#include "qhill.h"
#include "mpicheck.h"
#include "pugixml.hpp"
#include "mpicheck.h"

#include <string>
#include <vector>
#include <omp.h>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

QHill::QHill( MPI_Comm baseComm, const SParams& params )
{
	if ( params.antsPerHill < 0 )
		throw std::string( "Invalid QHill ants number. " ).append( __FUNCTION__ );
	if ( baseComm == MPI_COMM_NULL )
		throw std::string( "Invalid QHill base communicator. " ).append( __FUNCTION__ );

    m_comm  = baseComm;
    m_world = params.world;
    heuristicInfluence = params.heuristicDataFile.empty() ? 0.0f : params.heuristicInfluence;
    pheromoneInfluence = 1.0f - heuristicInfluence;
}

//------------------------------------------------------------

QHill::~QHill()
{
	CHECK( MPI_Comm_free( &m_comm ) );
}

//------------------------------------------------------------

void QHill::process()
{
    if ( !m_world )
        throw std::string( "Trying to start process, but the world was not setted. " ).append( __FUNCTION__ );

    // implement process here
}

//------------------------------------------------------------
}
