#include "qworld.h"
#include "mpicheck.h"

#include <math.h>

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

const std::vector< QWorld::SNode >& QWorld::getSiblings( int node ) const
{
    static std::vector< QWorld::SNode > dummy;
    if ( node >= (int)m_graph.size() )
        return dummy;

    return m_graph[ node ];
}

//------------------------------------------------------------

void QWorld::setInitialPheromone()
{
    const float initialPheromone = 1.0f / sqrtf(2.0f);

    for ( int i = 0; i < (int)m_graph.size(); ++i )
        for ( int q = 0; q < (int)m_graph[q].size(); ++q )
            m_graph[i][q].pheromone = QComplex( initialPheromone, initialPheromone );
}

//------------------------------------------------------------

void QWorld::setHeuristic( const std::vector< std::vector<float> >& heuristicData )
{
    m_graph.clear();

    const float initialPheromone = 1.0f / sqrtf(2.0f);

    for ( int i = 0; i < (int)heuristicData.size(); ++i )
    {
        bool firstNode = true;
        for ( int q = 0; q < (int)heuristicData[i].size(); ++q )
        {
            if ( heuristicData[i][q] != 0.0f )
            {
                if ( firstNode )
                {
                    m_graph.push_back( std::vector< SNode >() );
                    firstNode = false;
                }
                SNode tempNode;
                tempNode.heuristic = heuristicData[i][q];
                tempNode.pheromone = QComplex( initialPheromone, initialPheromone );
                tempNode.idx       = q;
                m_graph.back().push_back( tempNode );
            }
        }
    }
}
  
//------------------------------------------------------------

int QWorld::addNode( int node, const std::vector< QWorld::SNode >& siblings )
{
    if ( node >= (int)m_graph.size() )
    {
        m_graph.push_back( siblings );
        return m_graph.size() - 1;
    }

    m_graph.insert( m_graph.begin() + node, siblings );
    return node;
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

    for ( int i = 0; i < nodesNum; ++i )
    {
        MPI_Offset offset = ( i + 1 ) * sizeof(int) + i * sizeof( SNode );

        int siblingsNum = 0;        
        CHECK( MPI_File_read_at_all( fp, offset, &nodesNum, 1, MPI_INT, &status ) );
        if ( siblingsNum <= 0 )
            throw std::string( "Invalid data file format. " ).append( __FUNCTION__ );

        m_graph[i].resize( siblingsNum );
        CHECK( MPI_File_read_at_all( fp, offset + sizeof(int), &m_graph[i][0], sizeof( SNode ) * siblingsNum, MPI_CHAR, &status ) );
    }

    CHECK( MPI_File_close( &fp ) );
}

//------------------------------------------------------------
}
