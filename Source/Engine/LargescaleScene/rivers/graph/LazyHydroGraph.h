
#ifndef _LargescaleScene_LAZYHYDROGRAPH_H
#define _LargescaleScene_LAZYHYDROGRAPH_H

#include "LargescaleScene/graph/graph/LazyGraph.h"

namespace LargescaleScene
{

/**
 * A HydroGraph with lazy loading behavior.
 * See graph::LazyGraph and rivers::HydroGraph.
 * @ingroup rivergraph
 * @author Antoine Begault
 */
class LargescaleScene_API  LazyHydroGraph : public LazyGraph
{
public:
    /**
     * Creates a new LazyHydroGraph.
     */
    LazyHydroGraph();

    /**
     * Deletes this LazyHydroGraph.
     */
    virtual ~LazyHydroGraph();

    /**
     * Checks if the provided params count are correct for this graph.
     * A correct amount of parameters may depend on the graph it is used by.
     * For Graph, the amounts must be respectively at least 2, 4, 3, 3, 5, 2, 0.
     * Should be overwritten for graphs containing more data.
     */
    virtual void checkParams(int nodes, int curves, int areas, int curveExtremities, int curvePoints, int areaCurves, int subgraphs);

    /**
     * Adds a curve to this graph.
     *
     * @param parent the parent curve (NULL if none).
     * @param setParent if true, the new curve's parent will be set to the 'parent' parameter.
     * @return the new curve.
     */
    virtual CurvePtr newCurve(CurvePtr parent, bool setParent);

    /**
     * Adds a curve to this graph.
     *
     * @param model a model curve : the new curve will have the same vertices.
     * @param start the start node.
     * @param end the end node.
     * @return the new curve.
     */
    virtual CurvePtr newCurve(CurvePtr model, NodePtr start, NodePtr end);

    /**
     * Returns a new HydroGraph.
     */
    virtual Graph *createChild();

    /**
     * Saves this graph from a basic file.
     *
     * @param fileWriter the FileWriter used to save the file.
     * @param saveAreas if true, will save the subgraphs.
     */
    virtual void save(FileWriter *fileWriter, bool saveAreas = true);

    /**
     * Saves this graph from an indexed file.
     * Indexed files are used to load LazyGraphs faster. It contains the indexes of each element in the file.
     *
     * @param fileWriter the FileWriter used to save the file
     * @param saveAreas if true, will save the subgraphs
     */
    virtual void indexedSave(FileWriter *fileWriter, bool saveAreas = true);

    virtual void movePoint(CurvePtr c, int i, const vec2d &p, set<CurveId> &changedCurves);

    virtual NodePtr addNode(CurvePtr c, int i, Graph::Changes &changed);

protected:
    /**
     * Loads the Curve corresponding to the given Id.
     * The Curve description will be fetched via #fileReader at the offset given as parameter.
     *
     * @param offset the offset of this Curve in the file.
     * @param id the id of this Curve.
     * @return the loaded Curve.
     */
    virtual CurvePtr loadCurve(long int offset, CurveId id);

    friend class LazyHydroCurve;
};

}

#endif
