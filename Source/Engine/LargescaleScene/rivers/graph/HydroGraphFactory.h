

#ifndef _LargescaleScene_HYDROGRAPHFACTORY_H_
#define _LargescaleScene_HYDROGRAPHFACTORY_H_

#include "LargescaleScene/graph/graph/producer/GraphProducer.h"

namespace LargescaleScene
{

/**
 * A GraphProducer::GraphFactory that creates HydroGraph.
 * @ingroup rivergraph
 * @author Antoine Begault
 */
class LargescaleScene_API  HydroGraphFactory : public GraphProducer::GraphFactory
{
public:
    /**
     * Creates a new HydroGraphFactory.
     */
    HydroGraphFactory();

    /**
     * Deletes this HydroGraphFactory.
     */
    virtual ~HydroGraphFactory();

    /**
     * Returns a new HydroGraph.
     */
    virtual Graph *newGraph(int nodeCacheSize, int curveCacheSize, int areaCacheSize);
};

}

#endif
