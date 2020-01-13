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

#ifndef _LargescaleScene_FORESTORTHOLAYER_H_
#define _LargescaleScene_FORESTORTHOLAYER_H_
#include "LargescaleScene/common/LargescaleSceneMacro.h"
#include "LargescaleScene/graph/graph/producer/GraphLayer.h"

namespace LargescaleScene
{

/**
 * An OrthoGPUProducer layer to draw forests.
 * @ingroup ortho
 * @author Antoine Begault, Guillaume Piolat
 */
class LargescaleScene_API ForestOrthoLayer : public GraphLayer
{
public:
    /**
     * Creates a new ForestOrthoLayer.
     *
     * @param graphProducer the GraphProducer that produces the graphs to
     *      be drawn by this layer.
     * @param layerProgram the Program to be used to draw the graphs.
     * @param displayLevel the quadtree level at which the display of
     *      this layer must start.
     * @param quality enable or not the quality mode (better display).
     * @param color the color of forests (default 0:0:0).
     */
    ForestOrthoLayer(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram,
        int displayLevel = 0, bool quality = true, vec4f color = vec4f(0, 0, 0, 0));

    /**
     * Deletes this ForestOrthoLayer.
     */
    virtual ~ForestOrthoLayer();

    virtual void setTileSize(int tileSize, int tileBorder, float rootQuadSize);

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

protected:
    /**
     * Creates an uninitialized ForestOrthoLayer.
     */
    ForestOrthoLayer();

    /**
     * Initializes this ForestOrthoLayer. See #ForestOrthoLayer.
     */
    void init(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram,
        int displayLevel = 0, bool quality = true, vec4f color = vec4f(0, 0, 0, 0));

    virtual void swap(ptr<ForestOrthoLayer> p);

private:
    /**
     * Forest color.
     */
    vec4f color;

    /**
     * The mesh used for drawing curves.
     */
    ptr< Mesh<vec2f, unsigned int> > mesh;

    /**
     * The tesselator used for drawing areas.
     */
    ptr<Tesselator> tess;

    ptr<Uniform3f> offsetU;

    ptr<Uniform4f> colorU;
};

}

#endif
