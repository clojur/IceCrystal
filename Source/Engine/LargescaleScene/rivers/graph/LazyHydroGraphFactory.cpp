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
 * Authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include "LargescaleScene/rivers/graph/LazyHydroGraph.h"

#include "LargescaleScene/rivers/graph/LazyHydroGraphFactory.h"

namespace LargescaleScene
{

LazyHydroGraphFactory::LazyHydroGraphFactory() : GraphFactory()
{
}

LazyHydroGraphFactory::~LazyHydroGraphFactory()
{
}

Graph *LazyHydroGraphFactory::newGraph(int nodeCacheSize, int curveCacheSize, int areaCacheSize)
{
    LazyHydroGraph *res = new LazyHydroGraph();
    res->setNodeCacheSize(nodeCacheSize);
    res->setCurveCacheSize(curveCacheSize);
    res->setAreaCacheSize(areaCacheSize);
    return res;
}

class LazyHydroGraphFactoryResource : public ResourceTemplate<3, LazyHydroGraphFactory>
{
public:
    LazyHydroGraphFactoryResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<3, LazyHydroGraphFactory>(manager, name, desc)
    {
    }
};

extern const char lazyHydroGraphFactory[] = "lazyHydroGraphFactory";

static ResourceFactory::Type<lazyHydroGraphFactory, LazyHydroGraphFactoryResource> LazyHydroGraphFactoryType;

}
