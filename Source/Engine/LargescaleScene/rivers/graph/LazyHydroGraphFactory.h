

#ifndef _LargescaleScene_LAZYHYDROGRAPHFACTORY_H
#define _LargescaleScene_LAZYHYDROGRAPHFACTORY_H

#include "LargescaleScene/graph/graph/producer/GraphProducer.h"

namespace LargescaleScene
{

/**
 * A GraphProducer::GraphFactory that creates LazyHydroGraph.
 * @ingroup rivergraph
 * @author Antoine Begault
 */
class LargescaleScene_API  LazyHydroGraphFactory : public GraphProducer::GraphFactory
{
public:
    /**
     * Creates a new LazyHydroGraphFactory.
     */
    LazyHydroGraphFactory();

    /**
     * Deletes this LazyHydroGraphFactory.
     */
    virtual ~LazyHydroGraphFactory();

    /**
     * Returns a new LazyHydroGraph.
     */
    virtual Graph *newGraph(int nodeCacheSize, int curveCacheSize, int areaCacheSize);
};

}

#endif
