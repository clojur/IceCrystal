

#include "LargescaleScene/graph/graph/BasicCurvePart.h"

namespace LargescaleScene
{

BasicCurvePart::BasicCurvePart(CurvePtr p, int start, int end) :
    CurvePart(), curve(p), orientation(0), start(start), end(end)
{
    assert(end > start);
}

BasicCurvePart::BasicCurvePart(CurvePtr p, int orientation, int start, int end) :
    CurvePart(), curve(p), orientation(orientation), start(start), end(end)
{
    assert(end > start);
    assert(canClip(0));
    assert(canClip(end - start));
}

CurveId BasicCurvePart::getId() const
{
    return curve->getId();
}

CurveId BasicCurvePart::getParentId() const
{
    return getCurve()->getParentId();
}

CurvePtr BasicCurvePart::getCurve() const
{
    return curve;
}

int BasicCurvePart::getCurveIndex(int i) const
{
    return orientation == 0 ? start + i : end - i;
}

int BasicCurvePart::getType() const
{
    return getCurve()->getType();
}

float BasicCurvePart::getWidth() const
{
    return getCurve()->getWidth();
}

int BasicCurvePart::getEnd() const
{
    return end - start;
}

vec2d BasicCurvePart::getXY(int i) const
{
    return getCurve()->getXY(orientation == 0 ? start + i : end - i);
}

bool BasicCurvePart::getIsControl(int i) const
{
    return getCurve()->getIsControl(orientation == 0 ? start + i : end - i);
}

float BasicCurvePart::getS(int i) const
{
    return getCurve()->getS(orientation == 0 ? start + i : end - i);
}

box2d BasicCurvePart::getBounds() const
{
    return getCurve()->getBounds();
}

bool BasicCurvePart::canClip(int i) const
{
    return !getCurve()->getIsControl(orientation == 0 ? start + i : end - i);
}

CurvePart *BasicCurvePart::clip(int start, int end) const
{
    assert(start >= 0 && end > start && end <= getEnd());
    if (orientation == 0) {
        return getCurve()->getOwner()->createCurvePart(getCurve(), 0, this->start + start, this->start + end);
    } else {
        return getCurve()->getOwner()->createCurvePart(getCurve(), 1, this->end - end, this->end - start);
    }
}

}
