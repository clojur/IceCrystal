
#include "LargescaleScene/common/LargescaleSceneMacro.h"
#include "LargescaleScene/rivers/graph/HydroCurve.h"

#include "LargescaleScene/math/seg2.h"
#include "LargescaleScene/rivers/graph/HydroGraph.h"

using namespace Soul;

namespace LargescaleScene
{

HydroCurve::HydroCurve(Graph *owner) : Curve(owner), potential(-1.f)
{
    river.id = NULL_ID;
}

HydroCurve::HydroCurve(Graph *owner, CurvePtr c, NodePtr s, NodePtr e) :
    Curve(owner, c, s ,e)
{
    if (c != NULL) {
        ptr<HydroCurve> h = c.cast<HydroCurve>();
        assert(h != NULL);
        setRiver(h->getRiver());
        potential = h->getPotential();
    } else {
        river.id = NULL_ID;
        potential = -1.f;
    }
}

HydroCurve::~HydroCurve()
{
}

float HydroCurve::getWidth() const
{
    if (river.id != NULL_ID) {
        return getRiverPtr()->getWidth();
    }

    return width;
}

float HydroCurve::getPotential() const
{
    return potential;
}

void HydroCurve::setPotential(float potential)
{
    this->potential = potential;
}

CurveId HydroCurve::getRiver() const
{
    return river;
}

CurvePtr HydroCurve::getRiverPtr() const
{
    return getOwner()->getAncestor()->getCurve(river);
}

void HydroCurve::setRiver(CurveId river)
{
    this->river = river;
    if (river.id != NULL_ID) {
        this->width = getRiverPtr()->getWidth();
    }
}

void HydroCurve::print() const
{
    printf("%d-> %d %f %d %f %d\n", getId().id, getSize(), getWidth(), getType(), getPotential(), getRiver().id);
    vec2d v;
    for (int i = 0; i < getSize(); i++) {
        v = getXY(i);
        printf("%f %f %d %f %f\n", v.x, v.y, getIsControl(i), getS(i), getL(i));
    }
}

}
