
/*
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_LINEORTHOLAYER_H_
#define _LargescaleScene_LINEORTHOLAYER_H_
#include "LargescaleScene/common/LargescaleSceneMacro.h"
#include "LargescaleScene/graph/graph/producer/GraphLayer.h"

namespace LargescaleScene
{

/**
 * A GraphLayer that displays curves with lines of one pixel width.
 * @ingroup ortho
 * @author Antoine Begault, Guillaume Piolat
 */
class LargescaleScene_API LineOrthoLayer : public GraphLayer
{
public:
    /**
     * Creates a new LineOrthoLayer.
     *
     * @param graphProducer the GraphProducer that produces the graphs to
     *      be drawn by this layer.
     * @param layerProgram the Program to be used to draw the graphs.
     * @param displayLevel the quadtree level at which the display of
     *      this layer must start.
     */
    LineOrthoLayer(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram, int displayLevel = 0);

    /**
     * Deletes this LineOrthoLayer.
     */
    virtual ~LineOrthoLayer();

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

protected:
    /**
     * Creates an uninitialized LineOrthoLayer.
     */
    LineOrthoLayer();

    /**
     * Initializes this LineOrthoLayer. See #LineOrthoLayer.
     */
    void init(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram, int displayLevel = 0);

    virtual void swap(ptr<LineOrthoLayer> p);

private:
    /**
     * The mesh used for drawing curves.
     */
    ptr<Mesh<vec2f, unsigned int> > mesh;

    ptr<Uniform3f> tileOffsetU;
};

}

#endif
