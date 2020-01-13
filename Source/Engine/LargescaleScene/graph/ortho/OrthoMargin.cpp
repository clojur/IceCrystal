/*
 * LargescaleScene: a procedural landscape rendering library.
 * Authors:jiangsi.
 */

#include "LargescaleScene/graph/ortho/OrthoMargin.h"

#include "LargescaleScene/graph/graph/Curve.h"

namespace LargescaleScene
{

OrthoMargin::OrthoMargin()
{
    samplesPerTile = 0;
    borderFactor = 0.0f;
    widthFactor = 2.0f;
}

OrthoMargin::OrthoMargin(int samplesPerTile, float borderFactor, float widthFactor)
{
    this->samplesPerTile = samplesPerTile;
    this->borderFactor = borderFactor;
    this->widthFactor = widthFactor;
}

OrthoMargin::~OrthoMargin()
{
}

double OrthoMargin::getMargin(double clipSize)
{
    return (clipSize / 2) * borderFactor;
}

double OrthoMargin::getMargin(double clipSize, CurvePtr p)
{
    float pwidth = p->getWidth();
    float scale = 2.0f * (samplesPerTile - 1) / clipSize;
    if (p->getParent() != NULL && pwidth * scale >= 1) {
        return pwidth / 2.0f * widthFactor;
    } else {
        return 0;
    }
}

}
