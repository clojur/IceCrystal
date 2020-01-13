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

#include "LargescaleScene/graph/graph/LazyNode.h"

#include "LargescaleScene/graph/graph/LazyGraph.h"

namespace LargescaleScene
{

LazyNode::LazyNode(Graph* owner, NodeId id, double x, double y) :
    Node(owner, x, y), id(id)
{
}

LazyNode::~LazyNode()
{
    if (owner != NULL) {
        dynamic_cast<LazyGraph*>(owner)->deleteNode(id);
    }
}

NodeId LazyNode::getId() const
{
    return id;
}

void LazyNode::addCurve(CurveId c)
{
    dynamic_cast<LazyGraph*>(owner)->getNodeCache()->add(this, true);
    Node::addCurve(c);
}

void LazyNode::removeCurve(CurveId c)
{
    if (owner != NULL) { // the graph might have already been deleted.
        dynamic_cast<LazyGraph*>(owner)->getNodeCache()->add(this, true);
    }
    Node::removeCurve(c);
}

void LazyNode::loadCurve(CurveId c)
{
    Node::addCurve(c);
}

void LazyNode::doRelease()
{
    if (owner != NULL) {
        dynamic_cast<LazyGraph*>(owner)->releaseNode(id);
    } else {
        delete this;
    }
}

}
