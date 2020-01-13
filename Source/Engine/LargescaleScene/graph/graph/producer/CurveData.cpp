
#include "LargescaleScene/graph/graph/producer/CurveData.h"

#include "LargescaleScene/producer/CPUTileStorage.h"
#include "LargescaleScene/graph/graph/producer/GraphLayer.h"

namespace LargescaleScene
{

#define UNINITIALIZED -1e9


CurveData::CurveData(CurveId id, CurvePtr flattenCurve) :
    id(id), flattenCurve(flattenCurve)
{
    length = flattenCurve->computeCurvilinearLength();

    if (flattenCurve->getStart()->getCurveCount() > 1) {
        NodePtr n = flattenCurve->getStart();
        startCapLength = getCapLength(n, flattenCurve->getXY(n, 1));
    } else {
        startCapLength = 0.f;
    }
    if (flattenCurve->getEnd()->getCurveCount() > 1) {
        NodePtr n = flattenCurve->getEnd();
        endCapLength = getCapLength(n, flattenCurve->getXY(n, 1));
    } else {
        endCapLength = 0.f;
    }
}

CurveData::~CurveData()
{
    flattenCurve = NULL;
}


CurveId CurveData::getId()
{
    return id;
}

float CurveData::getCapLength(NodePtr p, vec2d q)
{
    vec2d o = p->getPos();
    float capLength = 0;
    for (int i = 0; i < p->getCurveCount(); ++i) {
        CurvePtr ipath = p->getCurve(i);
        if (!(ipath->getId() == getId())) {
            vec2d r = ipath->getXY(p, 1);
            if (abs(angle(q - o, r - o) - M_PI) < 0.01) {
                continue;
            }
            float pw = 2 * flattenCurve->getWidth();
            float ipw = 2 * ipath->getWidth();
            vec2d corner = LargescaleScene::corner(o, q, r, (double) pw, (double) ipw);
            float dot = (q - o).dot(corner - o);
            capLength = max((double) capLength, dot / (o - q).length());
        }
    }
    return ceil(capLength);
}

float CurveData::getStartCapLength()
{
    return startCapLength / 2;
}

float CurveData::getEndCapLength()
{
    return endCapLength / 2;
}

float CurveData::getCurvilinearLength()
{
    return length;
}

float CurveData::getCurvilinearLength(float s, vec2d *p, vec2d *n)
{
    return flattenCurve->getCurvilinearLength(s, p, n);
}

float CurveData::getCurvilinearCoordinate(float l, vec2d *p, vec2d *n)
{
    return flattenCurve->getCurvilinearCoordinate(l, p, n);
}

void CurveData::getUsedTiles(set<TileCache::Tile::Id> &tiles, float rootSampleLength)
{
    return;
}

float CurveData::getS(int rank)
{
    return flattenCurve->getS(rank);
}

CurvePtr CurveData::getFlattenCurve()
{
    return flattenCurve;
}

}
