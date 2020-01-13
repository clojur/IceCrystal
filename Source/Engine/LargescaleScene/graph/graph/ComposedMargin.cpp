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

#include "LargescaleScene/graph/graph/ComposedMargin.h"

#include "LargescaleScene/graph/graph/Area.h"

namespace LargescaleScene
{

ComposedMargin::ComposedMargin()
{
}

ComposedMargin::~ComposedMargin()
{
}

double ComposedMargin::getMargin(double clipSize)
{
    double m = 0.0f;
    for (unsigned int i = 0; i < margins.size(); ++i) {
        m = max(m, margins[i]->getMargin(clipSize));
    }
    return m;
}

double ComposedMargin::getMargin(double clipSize, CurvePtr c)
{
    double m = 0.0f;
    for (unsigned int i = 0; i < margins.size(); ++i) {
        m = max(m, margins[i]->getMargin(clipSize, c));
    }
    return m;
}

double ComposedMargin::getMargin(double clipSize, AreaPtr a)
{
    double m = 0.0f;
    for (unsigned int i = 0; i < margins.size(); ++i) {
        m = max(m, margins[i]->getMargin(clipSize, a));
    }
    return m;
}

void ComposedMargin::addMargin(Margin *m)
{
    margins.push_back(m);
}

void ComposedMargin::removeMargin(Margin *m)
{
    vector<Margin*>::iterator i = find(margins.begin(), margins.end(), m);
    if (i != margins.end()) {
        margins.erase(i);
    }
}

}
