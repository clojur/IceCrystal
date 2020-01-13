
/*
 * Authors:jiangsi.
 */

#ifndef _LargescaleScene_MASKORTHOLAYER_H_
#define _LargescaleScene_MASKORTHOLAYER_H_
#include "LargescaleScene/common/LargescaleSceneMacro.h"
#include "LargescaleScene/graph/graph/producer/GraphLayer.h"

namespace LargescaleScene
{

/**
 * An OrthoGPUProducer layer to draw a Graph as a mask. This layer
 * can draw the %graph in a specific channel, and can combine it
 * with the previous content of this channel via blending equations.
 * @ingroup ortho
 * @author Antoine Begault, Guillaume Piolat
 */
class LargescaleScene_API MaskOrthoLayer : public GraphLayer
{
public:
    /**
     * Structure used to pass blend arguments to the framebuffer.
     */
    struct BlendParams
    {
        BlendParams()
        {
            enable = false;
            buffer = (BufferId) -1;
            rgb = ADD;
            srgb = ONE;
            drgb = ZERO;
            alpha = ADD;
            salpha = ONE;
            dalpha = ZERO;
        }

        BufferId buffer;

        bool enable;

        BlendEquation rgb;

        BlendArgument srgb;

        BlendArgument drgb;

        BlendEquation alpha;

        BlendArgument salpha;

        BlendArgument dalpha;
    };

    /**
     * Creates a new MaskOrthoLayer.
     *
     * @param graphs the GraphProducer that produces the graphs to
     *      be drawn by this layer.
     * @param ignored the curve type that the mask should ignore (not draw).
     * @param layerProgram the Program to be used to draw the graphs.
     * @param writeMask the channels into which the graphs must be drawn.
     *      Must be used as a bit value.
     *      0 = none, 1 = Red, 2 = Green, 4 = Blue, 8 = Alpha
     *      16 = DepthBuffer, 32 = FSTENCIL, 64 = BSTENCIL.
     * @param color drawn color.
     * @param depth drawn depth.
     * @param blend the blending equations to be used to combine this mask with
     *      the previous content of the channel #channel.
     * @param blendColor the color used in blend equations.
     * @param displayLevel the quadtree level at which the display of
     *      this layer must start.
     */
    MaskOrthoLayer(ptr<GraphProducer> graphs, set<int> ignored, ptr<Program> layerProgram, int writeMask, vec4f color, float depth, float widthFactor, BlendParams blendParams, vec4f blendColor, int displayLevel, bool deform = false);

    /**
     * Deletes this MaskOrthoLayer.
     */
    virtual ~MaskOrthoLayer();

    virtual void setTileSize(int tileSize, int tileBorder, float rootQuadSize);

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

protected:
    /**
     * Creates an uninitialized MaskOrthoLayer.
     */
    MaskOrthoLayer();

    /**
     * Initializes thie MaskOrthoLayer. See #MaskOrthoLayer.
     */
    void init(ptr<GraphProducer> graphs, set<int> ignored, ptr<Program> layerProgram, int writeMask, vec4f color, float depth, float widthFactor, BlendParams blendParams, vec4f blendColor, int displayLevel, bool deform = false);

    void swap(ptr<MaskOrthoLayer> p);

private:
    /**
     * The curve type that the mask should ignore (not draw).
     */
    set<int> ignored;

    /**
     * The channels into which the graphs must be drawn.
     * Must be used as a bit value.
     * 0 = none, 1 = Red, 2 = Green, 4 = Blue, 8 = Alpha
     * 16 = DepthBuffer, 32 = FSTENCIL, 64 = BSTENCIL.
     */
    int writeMask;

    /**
     * Drawn color.
     */
    vec4f color;

    /**
     * Drawn depth.
     */
    float depth;

    float widthFactor;

    /**
     * The blending equations to be used to combine this mask with
     * the previous content of the channel #channel.
     */
    BlendParams blendParams;

    /**
     * The Color used in blend equations.
     */
    vec4f blendColor;

    /**
     * The mesh used to draw curves and areas.
     */
    ptr<Mesh< vec2f, unsigned int> > mesh;

    /**
     * The tesselator used to draw areas.
     */
    ptr<Tesselator> tess;

    ptr<Uniform3f> tileOffsetU;

    ptr<Uniform4f> colorU;

    ptr<Uniform1f> depthU;

};

}

#endif
