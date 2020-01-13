
#include "LargescaleScene/graph/dem/ElevationMargin.h"
#include "LargescaleScene/graph/graph/Area.h"

namespace LargescaleScene
{

ElevationMargin::ElevationMargin()
{
    samplesPerTile = 0;
    borderFactor = 0.0f;
}

ElevationMargin::ElevationMargin(int samplesPerTile, float borderFactor)
{
    this->samplesPerTile = samplesPerTile;
    this->borderFactor = borderFactor;
}

ElevationMargin::~ElevationMargin()
{
}

double ElevationMargin::getMargin(double clipSize)
{
    return (clipSize / 2) * borderFactor;
}

}
