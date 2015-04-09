#include "sparams.h"
#include "mpicheck.h"
#include "pugixml.hpp"

#include <vector>

//------------------------------------------------------------

namespace QAnthill {
//-----------------------------------------------------------

SParams::SParams( MPI_Comm comm/* = MPI_COMM_NULL*/, const char* file/* = 0*/ )
        : cycThreshold(0)
        , cycStepsThreshold(0)
        , antsPerHill(0)
        , hillsNum(0)
		, procsPerNode(1)
        , heuristicInfluence(0.0f)
        , target(-1)
        , colonyInitType( COLONY_INIT_TYPE_UNSET )
        , initPoint(0)
        , choosePathMethodConst(0.0f)
        , world(0)
{
    if ( comm == MPI_COMM_NULL )
        throw std::string( "Invalid communicator. " ).append( __FUNCTION__ );

    if ( file )
        initWithFile( comm, file );
}

//-----------------------------------------------------------

SParams::~SParams()
{
    delete world;
}

//-----------------------------------------------------------

void SParams::initWithFile( MPI_Comm comm, const char* file )
{
    if ( !file || !file[0] )
        throw std::string( "Some problems with params file. " ).append( __FUNCTION__ );

    MPI_File fp = MPI_FILE_NULL;
    CHECK( MPI_File_open( comm, file, MPI_INFO_NULL, MPI_MODE_RDONLY, &fp ) );
    
    MPI_Offset fileSize = 0;
    CHECK( MPI_File_get_size( fp, &fileSize ) );
    
    std::vector<char> buf( (unsigned)fileSize );
    MPI_Status status;    
    MPI_File_read_at_all( fp, 0, &buf[0], int( fileSize ), MPI_CHAR, &status );

    CHECK( MPI_File_close( &fp ) );

    pugi::xml_document doc;
    doc.load_buffer( &buf[0], unsigned( fileSize ) );
    
    pugi::xml_node qgenNode = doc.child( "qgen" );    
    if ( !qgenNode )
        throw std::string( "Some problems with params file. " ).append( __FUNCTION__ );

    for ( pugi::xml_node node = qgenNode.child( "parameter" ); node; node = node.next_sibling() )
    {
        const char* name = node.attribute( "name" ).as_string();
        if ( !name )
            continue;
        
        if ( 0 == strcmp( "cycle-threshold", name ) )
        {
            cycThreshold = (long long)node.attribute( "value" ).as_uint(0);
        }
        else if ( 0 == strcmp( "cycle-steps-threshold", name ) )
        {
            cycStepsThreshold = (long long)node.attribute( "value" ).as_uint(0);
        }
        else if ( 0 == strcmp( "ants-per-hill", name ) )
        {
            antsPerHill = (int)node.attribute( "value" ).as_int(0);
        }
        else if ( 0 == strcmp( "procs-per-node", name ) )
        {
            procsPerNode = (int)node.attribute( "value" ).as_int(0);
        }
        else if ( 0 == strcmp( "heuristic-influence", name ) )
        {
            heuristicInfluence = node.attribute( "value" ).as_float(0.0f);
        } 
        else if ( 0 == strcmp( "target", name ) )
        {
            target = (int)node.attribute( "value" ).as_int(-1);
        }
        else if ( 0 == strcmp( "colony-init-type", name ) )
        {      
            const char* type = node.attribute( "value" ).as_string("");
            if ( 0 == strcmp( "random", type ) )
                colonyInitType = COLONY_INIT_TYPE_RANDOM;
            else if ( 0 == strcmp( "point", type ) )
                colonyInitType = COLONY_INIT_TYPE_POINT;
            else
                colonyInitType = COLONY_INIT_TYPE_UNSET;
        }
        else if ( 0 == strcmp( "colony-init-point", name ) )
        {
            initPoint = node.attribute( "value" ).as_int(0);
        }  
        else if ( 0 == strcmp( "choose-path-method-const", name ) )
        {
            choosePathMethodConst = node.attribute( "value" ).as_float(0.0f);
        }        
        else if ( 0 == strcmp( "heuristic-data-file", name ) )
        {
            heuristicDataFile = node.attribute( "value" ).as_string("");
        } 
        else if ( 0 == strcmp( "out-file", name ) )
        {
            outFile = node.attribute( "value" ).as_string("");
        }
    }
}

//------------------------------------------------------------
}
