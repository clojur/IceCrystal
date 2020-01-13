/*
 * LargescaleScene: a procedural landscape rendering library.
 * Copyright (c) 2008-2011 INRIA
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * LargescaleScene is distributed under a dual-license scheme.
 * You can obtain a specific license from Inria: LargescaleScene-licensing@inria.fr.
 */

/*
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_LINECURVEPART_H_
#define _LargescaleScene_LINECURVEPART_H_

#include "LargescaleScene/graph/graph/CurvePart.h"

namespace LargescaleScene
{

/**
 * A part of a curve. This part is defined by 2 points (start and end points).
 * This is used for completing a clipped area when one or more of its curves
 * are out of the clipping box.
 * @ingroup graph
 * @author Antoine Begault
 */
class LargescaleScene_API LineCurvePart : public CurvePart
{
public:
    /**
     * Creates a new LineCurvePart.
     *
     * @param start Coordinates of the starting point.
     * @param end Coordinates of the end point.
     */
    LineCurvePart(vec2d start, vec2d end);

    /**
     * Returns the end index of this curve part inside the original curve.
     */
    virtual int getEnd() const;

    /**
     * Returns the coordinates of a point at a given index.
     *
     * @param i the rank of the Node to get the coordinates from.
     */
    virtual vec2d getXY(int i) const;

    /**
     * Returns true if the i'th point is a Control Point.
     *
     * @param i the rank of the Node to get the boolean from.
     */
    virtual bool getIsControl(int i) const;

    /**
     * Returns the i'th point S coordinate.
     *
     * @param i the rank of the Node to get the coordinate from.
     */
    virtual float getS(int i) const;

    /**
     * Returns the curve part's bounding box.
     */
    virtual box2d getBounds() const;

    /**
     * Returns a sub-curvePart clipped from this one.
     *
     * @param start starting point.
     * @param end end point.
     * @return a new CurvePart, result of the clipping of this CurvePart.
     */
    virtual CurvePart *clip(int start, int end) const;

private:
    /**
     * The start point that defines this CurvePart.
     */
    vec2d start;

    /**
     * The start point that defines this CurvePart.
     */
    vec2d end;
};

}

#endif
