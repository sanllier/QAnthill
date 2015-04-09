#include "qaco.h"
#ifdef MTRAND
    #include "random_mtrand.h"
#else
    #include "random_def.h"
#endif

#include <omp.h>
#include <vector>
#include <time.h>

//------------------------------------------------------------

#define ACTIVE_ONLY if ( !active() ) return
#define ACTIVE_ONLY_R( _x_ ) if ( !active() ) return _x_

#define MASTER_PRINT( _x_ ) if ( m_ctx.generalRank == ROOT_ID ) { std::cout << _x_ ; std::cout.flush(); }

//------------------------------------------------------------

namespace QAnthill {
//-----------------------------------------------------------

struct QACOProcess::SQACOProcessContext
{
	MPI_Comm generalComm;
    int generalRank;
    int generalSize;

    bool active;

    SQACOProcessContext()
        : generalComm( MPI_COMM_NULL )        
        , generalRank( -1 )
        , generalSize(0)
        , active( false )
    {}
};

//-----------------------------------------------------------

QACOProcess::QACOProcess( const SParams& params, MPI_Comm comm/* = MPI_COMM_WORLD*/ )
    : m_params( params )
	, m_ctx(0)
	, m_localHill(0)
{
    if ( !m_params.fClass )
        throw std::string( "Invalid fitness class: NULL pointer." ).append( __FUNCTION__ );

    int isMPIInitialized = 0;
    CHECK( MPI_Initialized( &isMPIInitialized ) );
    if ( !isMPIInitialized )
        throw std::string( "MPI was not initialized." ).append( __FUNCTION__ );

    int initialCommRank = 0;
    int initialCommSize = 0;
    CHECK( MPI_Comm_rank( comm, &initialCommRank ) );
    CHECK( MPI_Comm_size( comm, &initialCommSize ) );

    if ( initialCommSize <= 0 )
        throw std::string( "Invalid communicator: comm size <= 0. " ).append( __FUNCTION__ );
    if ( initialCommSize < m_params.hillsNum )
        throw std::string( "Initial communicator size is smaller than requsted. " ).append( __FUNCTION__ );

    const unsigned seed = unsigned( time(0) ) ^ unsigned( initialCommRank );
#ifdef MTRAND
    m_randomizer = Randomizer::Create( new RandomMTRand(), seed );
#else
    m_randomizer = Randomizer::Create( new RandomDefault(), seed );
#endif

    m_ctx = new SQACOProcessContext();

    CHECK( MPI_Comm_split( comm, initialCommRank < m_params.hillsNum, initialCommRank, &m_ctx->generalComm ) );
    m_ctx->active = initialCommRank < m_params.hillsNum;

    const int activesCount = std::min( m_params.hillsNum, initialCommSize );

    if ( initialCommRank < m_params.hillsNum )
    {
        CHECK( MPI_Comm_rank( m_ctx->generalComm, &(m_ctx->generalRank) ) );
        CHECK( MPI_Comm_size( m_ctx->generalComm, &(m_ctx->generalSize) ) );

        if ( !m_params.heuristicDataFile.empty() )
        {
            m_params.world = new QWorld();
            m_params.world->loadFromFile( m_ctx->generalComm, m_params.heuristicDataFile );   
        }
        else if ( !m_params.world )
        {
            throw std::string( "You must provide world data in some way ( file or explicit world data ). " ).append( __FUNCTION__ );
        }

	    m_localHill = new QHill( m_ctx->generalComm, m_params );  
    }
    else
    {
        m_ctx->generalRank = -1;
        m_ctx->generalSize =  0;
    }
}

//-----------------------------------------------------------

QACOProcess::~QACOProcess()
{
    if ( active() )
    {
		delete m_localHill;
        CHECK( MPI_Comm_free( &(m_ctx->generalComm) ) );
    }
    
    Randomizer::Dispose();
    delete m_ctx;     
}

//-----------------------------------------------------------

bool QACOProcess::isMaster() const
{ 
    return m_ctx->generalRank == ROOT_ID; 
}

//-----------------------------------------------------------

bool QACOProcess::active() const 
{ 
    return m_ctx->active;
}

//-----------------------------------------------------------

double QACOProcess::process()
{
    if ( m_params.procsPerNode <= 0 )
        throw std::string( "Invalid number of processes per node. " ).append( __FUNCTION__ );

    omp_set_num_threads( m_params.procsPerNode );

    ACTIVE_ONLY_R( 0.0 );

    double startTime = MPI_Wtime();

    // TODO

    return MPI_Wtime() - startTime;
}

//------------------------------------------------------------
}
