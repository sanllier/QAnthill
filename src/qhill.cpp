#include "qhill.h"
#include "mpicheck.h"
#include "randomizer.h"

#include <math.h>
#include <string>
#include <vector>
#include <omp.h>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

QHill::QHill( MPI_Comm baseComm, const SParams& params )
{
	if ( params.antsPerHill <= 0 )
		throw std::string( "Invalid QHill ants number. " ).append( __FUNCTION__ );
	if ( baseComm == MPI_COMM_NULL )
		throw std::string( "Invalid QHill base communicator. " ).append( __FUNCTION__ );
    if ( params.target >= 0 && params.target >= params.world->nodesNum() )
        throw std::string( "Invalid target node. " ).append( __FUNCTION__ );

    m_comm  = baseComm;
    m_world = params.world;
    m_heuristicInfluence = params.heuristicDataFile.empty() ? 0.0f : params.heuristicInfluence;
    m_pheromoneInfluence = 1.0f - m_heuristicInfluence;
    m_choosePathConst     = params.choosePathMethodConst;

    m_hill.resize( params.antsPerHill );

    if ( params.colonyInitType == COLONY_INIT_TYPE_RANDOM )
    {
        #pragma omp parallel for
        for ( int i = 0; i < params.antsPerHill; ++i )
        {
            m_hill[i].startPos = Randomizer::Get()->nextInt() % params.antsPerHill;
            m_hill[i].curPos = m_hill[i].startPos;
        }
    }
    else if ( params.colonyInitType == COLONY_INIT_TYPE_POINT )
    {
        if ( params.initPoint < 0 )
            throw std::string( "Invalid init point. " ).append( __FUNCTION__ );

        #pragma omp parallel for
        for ( int i = 0; i < params.antsPerHill; ++i )
        {
            m_hill[i].startPos = params.initPoint;
            m_hill[i].curPos   = params.initPoint;
            int targetNode = i / params.antsPerHill;
        }
    }
    else
    {
        throw std::string( "Colony init type was not specified. " ).append( __FUNCTION__ );
    }
}

//------------------------------------------------------------

QHill::~QHill()
{
}

//------------------------------------------------------------

void QHill::run()
{
    if ( !m_world )
        throw std::string( "Trying to run, but the world was not setted. " ).append( __FUNCTION__ );

    const int hillSize = m_hill.size();

    #pragma omp parallel
    {
        std::vector<BASETYPE> probabilities;

        #pragma omp for
        for ( int i = 0; i < hillSize; ++i ) 
        {
            int step = 0;
            m_hill[i].trace.reset();
            m_hill[i].trace.add( m_hill[i].startPos );

            const std::vector< QWorld::SNode >& neighbors = m_world->getNeighbors( m_hill[i].curPos );
            BASETYPE q = Randomizer::Get()->nextBasetype();
            if ( q <= m_choosePathConst )
            {
                int targetIdx = 0;
                BASETYPE targetWeight = BASETYPE(0);
                for ( std::vector< QWorld::SNode >::const_iterator curNode = neighbors.begin(); curNode != neighbors.end(); ++curNode )
                {
                    BASETYPE curWeight = BASETYPE(0);
                    if ( curNode->pheromone.measure() )
                        curWeight = powf( curNode->heuristic, m_heuristicInfluence ); // TODO + powf( modf( curNode->pheromone ) );

                    if ( curWeight > targetWeight )
                    {
                        targetIdx    = curNode - neighbors.begin();
                        targetWeight = curWeight;
                    }
                }

                m_hill[i].curPos = targetIdx;
                m_hill[i].trace.add( targetIdx );
            }
            else
            {
                if ( probabilities.size() < neighbors.size() )
                    probabilities.resize( neighbors.size() );

                const int neighborsNum = neighbors.size();
                for ( int i = 0; i < neighborsNum; ++i )
                {
                    // TODO probabilities[i] = powf( neighbors[i].heuristic, m_heuristicInfluence );
                }
            }
        }
    }
    
}

//------------------------------------------------------------
}
