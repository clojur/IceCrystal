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

#include "LargescaleScene/graph/graph/Margin.h"

#include "LargescaleScene/graph/graph/Area.h"

namespace LargescaleScene
{

double Margin::getMargin(double clipSize, AreaPtr a)
{
    double m = 0.0f;
    for (int i = 0; i < a->getCurveCount(); i++) {
        CurvePtr c = a->getCurve(i);
        m = max(m, getMargin(clipSize, c));
    }
    return m;
}

}
