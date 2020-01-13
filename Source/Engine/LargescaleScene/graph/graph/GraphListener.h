
/*
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_GRAPH_LISTENER_H_
#define _LargescaleScene_GRAPH_LISTENER_H_
#include "LargescaleScene/common/LargescaleSceneMacro.h"
namespace LargescaleScene
{

/**
 * Abstract class used to monitor changes on a graph.
 * @ingroup graph
 * @author Antoine Begault
 */
class LargescaleScene_API GraphListener
{
public:
    /**
     * Creates a new GraphListener.
     */
    GraphListener();

    /**
     * Deletes this GraphListener.
     */
    virtual ~GraphListener();

    /**
     * This virtual method must be called when updating the graph.
     * It will determine what has to be done on this listener when the graph has changed.
     */
    virtual void graphChanged() = 0;
};

}

#endif
