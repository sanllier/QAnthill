#include "qworld.h"
#include "mpicheck.h"
#include "randomizer.h"

#include <math.h>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

const std::vector< QWorld::SNode >& QWorld::getNeighbors( int node ) const
{
    static std::vector< QWorld::SNode > dummy;
    if ( node >= (int)m_graph.size() )
        return dummy;

    return m_graph[ node ];
}

//------------------------------------------------------------

void QWorld::setInitialPheromone()
{
    const BASETYPE initialPheromone = BASETYPE(1) / sqrtf(2.0f);

    for ( int i = 0; i < (int)m_graph.size(); ++i )
    {
        for ( int q = 0; q < (int)m_graph[i].size(); ++q )
        {
            m_graph[i][q].pheromone.a = Randomizer::Get()->nextBasetype();
            m_graph[i][q].pheromone.b = sqrt( QComplex( BASETYPE(1) ) - m_graph[i][q].pheromone.a );
        }
    }
}

//------------------------------------------------------------

void QWorld::setHeuristic( const std::vector< std::vector< BASETYPE > >& heuristicData )
{
    m_graph.resize( heuristicData.size() );
    for ( int i = 0; i < (int)heuristicData.size(); ++i )
    {
        for ( int q = 0; q < (int)heuristicData[i].size(); ++q )
        {
            if ( heuristicData[i][q] != BASETYPE(0) )
            {
                SNode tempNode;
                tempNode.heuristic = heuristicData[i][q];
                tempNode.idx       = q;
                m_graph[i].push_back( tempNode );
            }
        }
    }
}

//------------------------------------------------------------

void QWorld::loadFromFile( MPI_Comm comm, const std::string& fileName )
{
    if ( fileName.empty() )
        throw std::string( "Invalid file name. " ).append( __FUNCTION__ );
    if ( comm == MPI_COMM_NULL )
        throw std::string( "Invalid communicator. " ).append( __FUNCTION__ );

    m_graph.clear();

    MPI_File fp = MPI_FILE_NULL;
    CHECK( MPI_File_open( comm, fileName.c_str(), MPI_INFO_NULL, MPI_MODE_RDONLY, &fp ) );
    
    MPI_Offset fileSize = 0;
    CHECK( MPI_File_get_size( fp, &fileSize ) );

    MPI_Status status;
    int nodesNum = 0;
    CHECK( MPI_File_read_at_all( fp, 0, &nodesNum, 1, MPI_INT, &status ) );
    m_graph.resize( nodesNum );

    if ( nodesNum <= 0 )
        throw std::string( "Invalid data file format. " ).append( __FUNCTION__ );

    std::vector< BASETYPE > buf;
    MPI_Offset offset = sizeof(int);
    for ( int i = 0; i < nodesNum; ++i )
    {
        int neighborsNum = 0;        
        CHECK( MPI_File_read_at_all( fp, offset, &neighborsNum, 1, MPI_INT, &status ) );
        offset += sizeof(int);
        if ( neighborsNum <= 0 )
            throw std::string( "Invalid data file format. " ).append( __FUNCTION__ );

        m_graph[i].resize( neighborsNum );
        buf.resize( neighborsNum );
        CHECK( MPI_File_read_at_all( fp, offset, &buf[0], neighborsNum, MPI_BASETYPE, &status ) );
        offset += sizeof( BASETYPE ) * neighborsNum;

        for ( int q = 0; q < neighborsNum; ++q )
        {
            if ( buf[q] != BASETYPE(0) )
            {
                SNode tempNode;
                tempNode.heuristic = buf[q];
                tempNode.idx       = q;
                m_graph[i].push_back( tempNode );
            }
        }
    }

    CHECK( MPI_File_close( &fp ) );
}

//------------------------------------------------------------
}
