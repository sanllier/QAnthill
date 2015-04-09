#ifndef QWORLD_H	
#define QWORLD_H

#include <vector>
#include <string>

#include "defs.h"
#include "qbit.h"
#include "mpi.h"

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

class QWorld
{
public:
    struct SNode
    {
        QBit     pheromone;
        BASETYPE heuristic;
        int      idx;
    };

public:
	QWorld();
	~QWorld();

    int nodesNum() const { return m_graph.size(); }

    const std::vector< SNode >& getNeighbors( int node ) const;

    void setInitialPheromone();
    void setHeuristic( const std::vector< std::vector< BASETYPE > >& heuristicData );

	void loadFromFile( MPI_Comm comm, const std::string& fileName );
	
private:
    std::vector< std::vector< SNode > > m_graph;
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
