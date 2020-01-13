
#ifndef _LargescaleScene_TESSELATOR_H_
#define _LargescaleScene_TESSELATOR_H_

#include "Soul/math/vec2.h"
#include "Soul/render/Mesh.h"

#include "LargescaleScene/common/LargescaleSceneMacro.h"
using namespace Soul;

namespace LargescaleScene
{

 class  LargescaleScene_API Tesselator : public Object
{
public:
    /**
     * Creates a new tesselator.
     */
    Tesselator();

    /**
     * Destroys this tesselator.
     */
    virtual ~Tesselator();

    /**
     * Starts a new triangulation.
     *
     * @param mesh the mesh where the triangles generated by triangulation
     * are added. This mesh must be in AttributeBuffer#TRIANGLES mode (see
     * Mesh#setMode).
     */
    void beginPolygon(ptr< Mesh<vec2f, unsigned int> > mesh);

    /**
     * Starts a new contour.
     */
    void beginContour();

    /**
     * Defines a new vertex in the current contour.
     */
    void newVertex(float x, float y);

    /**
     * Ends the current contour.
     */
    void endContour();

    /**
     * Ends the current triangulation.
     */
    void endPolygon();

private:
    ptr< Mesh<vec2f, unsigned int> > mesh;

    void *tess;
};

}

#endif