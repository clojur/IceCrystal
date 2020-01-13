
#ifndef _LargescaleScene_CURVEDATAFACTORY_H_
#define _LargescaleScene_CURVEDATAFACTORY_H_

#include "LargescaleScene/producer/TileProducer.h"
#include "LargescaleScene/graph/graph/GraphListener.h"
#include "LargescaleScene/graph/graph/producer/CurveData.h"

namespace LargescaleScene
{

class LargescaleScene_API CurveDataFactory : public GraphListener
{
public:
    /**
     * Creates a new CurveDataFactory.
     */
    CurveDataFactory();

    /**
     * Creates a new CurveDataFactory.
     *
     * @param producer the GraphProducer used to generate Curves and their flattened version.
     */
    CurveDataFactory(ptr<TileProducer> producer);

    /**
     * Deletes this CurveDataFactory.
     */
    virtual ~CurveDataFactory();

    /**
     * Returns a new CurveData, whose type depends on the class that implements CurveDataFactory.
     *
     * @param id the CurveId of the described Curve.
     * @param flattenCurve flattened version of the described Curve.
     */
    virtual CurveData *newCurveData(CurveId id, CurvePtr flattenCurve);

    /**
     * See GraphListener#graphChanged().
     */
    virtual void graphChanged();

    /**
     * Returns the CurveData corresponding to the given Curve.
     * Handles a count of references.
     *
     * @param c a Curve.
     */
    CurveData *getCurveData(CurvePtr c);

    /**
     * Releases the CurveData corresponding to the given CurveId. The reference count is decreased, and if it is equal to zero,  the CurveData is deleted.
     * It will then call GraphProducer#putFlattenCurve().
     *
     * @param id the CurveId of an existing CurveData.
     */
    void putCurveData(CurveId id);

    /**
     * Returns the CurveData corresponding to the given Curve. Doesn't change the reference count.
     *
     * @param c a Curve.
     */
    CurveData *findCurveData(CurvePtr c);

    /**
     * Releases every used CurveData corresponding to a given tile.
     *
     * @param level level of the Tile.
     * @param x x coordinate of the Tile.
     * @param y y coordinate of the Tile.
     */
    void releaseCurveData(int level, int tx, int ty);

    /**
     * Adds a list of Curves used by a tile of this factory. They will require a call to #releaseCurveData.
     *
     * @param level level of the Tile.
     * @param x x coordinate of the Tile.
     * @param y y coordinate of the Tile.
     * @param curveDatas list of Ids corresponding to the used CurveData.
     */
    void addUsedCurveDatas(int level, int tx, int ty, set<CurveId> curveDatas);

    /**
     * Deletes every CurveData.
     */
    void clear();

    bool hasCurveData(CurveId id) {
        return curveDatas.find(id) != curveDatas.end();
    }

protected:
    /**
     * Initializes CurveDataFactory fields.
     *
     * @param producer the GraphProducer used to generate Curves and their flattened version.
     */
    void init(ptr<TileProducer> producer);

    virtual void swap(CurveDataFactory* factory);

    /**
     * The GraphProducer used to generate Curves and their flattened version.
     */
    ptr<TileProducer> producer;

    /**
     * List of every CurveData and their associated CurveId.
     */
    map<CurveId, CurveData*> curveDatas;

    /**
     * Reference count for each CurveData.
     */
    map<CurveData*, int> usedDataCount;

    /**
     * Maps every tile to the CurveDatas it uses.
     */
    map<TileCache::Tile::Id, set<CurveId> > usedDatas;
};

}

#endif
