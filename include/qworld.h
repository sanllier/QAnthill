#ifndef QWORLD_H	
#define QWORLD_H

#include <vector>
#include <complex>
#include <string>

#include "defs.h"
#include "mpi.h"

//------------------------------------------------------------

namespace QAnthill {
//------------------------------------------------------------

class QWorld
{
public:
    struct SNode
    {
        QComplex pheromone;
        float    heuristic;
        int      idx;
    };

public:
	QWorld();
	~QWorld();

    int nodesNum() const { return m_graph.size(); }

    const std::vector< SNode >& getSiblings( int node ) const;

    void setInitialPheromone();
    void setHeuristic( const std::vector< std::vector<float> >& heuristicData );

    int addNode( int node, const std::vector< SNode >& siblings );
	void loadFromFile( MPI_Comm comm, const std::string& fileName );
	
private:
    std::vector< std::vector< SNode > > m_graph;
};

//-----------------------------------------------------------
}

//------------------------------------------------------------
#endif
