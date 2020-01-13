/*
 * LargescaleScene: a procedural landscape rendering library.
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_WATERORTHOLAYER_H_
#define _LargescaleScene_WATERORTHOLAYER_H_

#include "LargescaleScene/graph/graph/producer/GraphLayer.h"

namespace LargescaleScene
{

/**
 * An OrthoGPUProducer layer to draw static %rivers and lakes.
 * @ingroup ortho
 * @author Antoine Begault, Guillaume Piolat
 */
class LargescaleScene_API WaterOrthoLayer : public GraphLayer
{
public:
    /**
     * Predefined type for rivers.
     * Used for drawing and managing rivers.
     */
    enum riverType
    {
        BORDER = -2, //!< River Banks. Curves with this type have as ancestor the curve representing the center of the river they belong to.
        OBSTACLE = -1, //!< Floating obstacle or islands.
        RIVER = 0, //!< Basic river.
        ISLAND = 1, //!< Represents islands when area1 != NULL.
        LAKE = 2 //!< Lake.
    };

    /**
     * Creates a new WaterOrthoLayer.
     *
     * @param graphProducer the GraphProducer that produces the graphs to
     *      be drawn by this layer.
     * @param layerProgram the Program to be used to draw the graphs.
     * @param displayLevel the quadtree level at which the display of
     *      this layer must start.
     * @param quality enable or not the quality mode (better display).
     * @param color the color of water (default 0:0:0).
     * @param deform true if the produced tiles will be mapped on a spherical
     *      %terrain.
     */
    WaterOrthoLayer(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram,
        int displayLevel = 0, bool quality = true, vec4f color = vec4f(0, 0, 0, 0),
        bool deform = false);

    /**
     * Deletes this WaterOrthoLayer.
     */
    virtual ~WaterOrthoLayer();

    virtual void setTileSize(int tileSize, int tileBorder, float rootQuadSize);

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

protected:
    /**
     * Creates an uninitialized WaterOrthoLayer.
     */
    WaterOrthoLayer();

    /**
     * Initializes this WaterOrthoLayer. See #WaterOrthoLayer.
     */
    void init(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram,
        int displayLevel = 0, bool quality = true, vec4f color = vec4f(0, 0, 0, 0),
        bool deform = true);

    virtual void swap(ptr<WaterOrthoLayer> p);

private:
    /**
     * Water color.
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

    ptr<Uniform3f> tileOffsetU;

    ptr<Uniform4f> colorU;
};

}

#endif
