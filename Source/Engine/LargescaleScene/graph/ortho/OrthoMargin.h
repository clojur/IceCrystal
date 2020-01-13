/*
 * LargescaleScene: a procedural landscape rendering library.
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_ORTHOMARGIN_H_
#define _LargescaleScene_ORTHOMARGIN_H_

#include "LargescaleScene/graph/graph/Margin.h"

namespace LargescaleScene
{

/**
 * A Margin to clip a Graph for an OrthoGPUProducer layer. This margin
 * enlarges the clip region so that it also includes the ortho tile borders.
 * @ingroup ortho
 * @author Antoine Begault
 */
class LargescaleScene_API OrthoMargin : public Margin
{
public:
    /**
     * Creates an uninitialized OrthoMargin.
     */
    OrthoMargin();

    /**
     * Creates a new OrthoMargin.
     *
     * @param samplesPerTile number of pixels per ortho tile (without borders).
     * @param borderFactor size of the tile borders in percentage of tile size.
     * @param widthFactor width of drawn curves in percentage of curve width.
     */
    OrthoMargin(int samplesPerTile, float borderFactor, float widthFactor);

    /**
     * Deletes this OrthoMargin.
     */
    virtual ~OrthoMargin();

    virtual double getMargin(double clipSize);

    virtual double getMargin(double clipSize, CurvePtr p);

protected:
    /**
     * Number of pixels per elevation tile, without borders.
     */
    int samplesPerTile;

    /**
     * Size of the tile borders in percentage of tile size.
     */
    float borderFactor;

    /**
     * Width of drawn curves in percentage of curve width.
     */
    float widthFactor;
};

}

#endif
