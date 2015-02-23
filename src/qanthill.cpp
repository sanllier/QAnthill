#include "qanthill.h"
#ifdef STDRAND
	#include <stdlib.h>
	#include <time.h>
#else
	#include "sharedmtrand.h"
#endif

#include <omp.h>
#include <vector>

//------------------------------------------------------------

#define ACTIVE_ONLY if ( !active() ) return
#define ACTIVE_ONLY_R( _x_ ) if ( !active() ) return _x_

#define MASTER_PRINT( _x_ ) if ( m_ctx.generalRank == ROOT_ID ) { std::cout << _x_ ; std::cout.flush(); }

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

static MPI_Comm baseComm = MPI_COMM_NULL;

static MPI_Comm QAnthillInit( int* argc, char*** argv )
{
	if ( QAnthillInitialized() )
		return baseComm;

	CHECK( MPI_Init( argc, argv ) );
	baseComm = MPI_COMM_WORLD;

	return baseComm;
}

static bool QAnthillInitialized()
{
	return baseComm != MPI_COMM_NULL;
}

static void QAnthillFinilize()
{
	if ( !QAnthillInitialized() )
		return;

	CHECK( MPI_Finalize() );
}

//-----------------------------------------------------------

struct QAnthillProcess::SQAnthillProcessContext
{
	MPI_Comm generalComm;
    int generalRank;
    int generalSize;

    SQAnthillProcessContext()
        : generalComm( MPI_COMM_NULL )        
        , generalRank( -1 )
        , generalSize(0)
    {}
};

//-----------------------------------------------------------

QAnthillProcess::QAnthillProcess( const SParams& params, MPI_Comm comm/* = MPI_COMM_WORLD*/ )
    : m_params( params )
	, m_ctx(0)
	, m_localHill(0)
{
//    if ( !m_params.fClass )
//        throw std::string( "Invalid fitness class: NULL pointer." ).append( __FUNCTION__ );

	if ( !QAnthillInitialized() )
		throw std::string( "Trying to access QAnthill, but it was not initialized." ).append( __FUNCTION__ );

    int initialCommRank = 0;
    int initialCommSize = 0;
    CHECK( MPI_Comm_rank( comm, &initialCommRank ) );
    CHECK( MPI_Comm_size( comm, &initialCommSize ) );

#ifdef STDRAND
	srand( unsigned( time(0) ) ^ initialCommRank + 1 );
#else
    SharedMTRand::getClosedInstance( initialCommRank + 1 ); //setting seed
#endif

    if ( initialCommSize <= 0 )
        throw std::string( "Invalid communicator: comm size <= 0. " ).append( __FUNCTION__ );
    if ( initialCommSize < m_params.hillsNum )
        throw std::string( "Initial communicator size is smaller than requsted. " ).append( __FUNCTION__ );

    m_ctx = new SQAnthillProcessContext();

    const int activesCount = std::min( m_params.hillsNum, initialCommSize );
	std::vector<int> ranks( activesCount );
    for ( int i = 0; i < activesCount; ++i )
        ranks[i] = i;

    MPI_Group initialGroup;
    MPI_Group workingGroup;
    CHECK( MPI_Comm_group( comm, &initialGroup ) );
    CHECK( MPI_Group_incl( initialGroup, activesCount, &ranks[0], &workingGroup ) );
    CHECK( MPI_Comm_create( comm, workingGroup, &(m_ctx->generalComm) ) ); 
    CHECK( MPI_Group_free( &initialGroup ) );
    CHECK( MPI_Group_free( &workingGroup ) );

    if ( active() )
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

QAnthillProcess::~QAnthillProcess()
{
    if ( active() )
    {
		delete m_localHill;
        CHECK( MPI_Comm_free( &(m_ctx->generalComm) ) );
    }
    
    delete m_ctx;     
}

//-----------------------------------------------------------

bool QAnthillProcess::isMaster() const
{ 
    return m_ctx->generalRank == ROOT_ID; 
}

//-----------------------------------------------------------

bool QAnthillProcess::active() const 
{ 
    return m_ctx->generalComm != MPI_COMM_NULL;
}

//-----------------------------------------------------------

double QAnthillProcess::process()
{
    omp_set_num_threads( m_params.procsPerNode );

    ACTIVE_ONLY_R( 0.0 );

    double startTime = MPI_Wtime();

    return MPI_Wtime() - startTime;
}

//------------------------------------------------------------
}
