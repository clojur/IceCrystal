

#ifndef _LargescaleScene_TERRAIN_NODE_H_
#define _LargescaleScene_TERRAIN_NODE_H_

#include "Soul/math/mat2.h"
#include "Soul/scenegraph/SceneNode.h"
#include "LargescaleScene/terrain/Deformation.h"
#include "LargescaleScene/terrain/TerrainQuad.h"

using namespace Soul;

namespace LargescaleScene
{


class LargescaleScene_API  TerrainNode : public Object
{
public:
    /**
     * The deformation of this %terrain. In the %terrain <i>local</i> space the
     * %terrain sea level surface is flat. In the %terrain <i>deformed</i> space
     * the sea level surface can be spherical or cylindrical (or flat if the
     * identity deformation is used).
     */
    ptr<Deformation> deform;

    /**
     * The root of the %terrain quadtree. This quadtree is subdivided based on the
     * current viewer position by the #update method.
     */
    ptr<TerrainQuad> root;

    /**
     * Describes how the %terrain quadtree must be subdivided based on the viewer
     * distance. For a field of view of 80 degrees, and a viewport width of 1024
     * pixels, a quad of size L will be subdivided into subquads if the viewer
     * distance is less than splitFactor * L. For a smaller field of view and/or
     * a larger viewport, the quad will be subdivided at a larger distance, so
     * that its size in pixels stays more or less the same. This number must be
     * strictly larger than 1.
     */
    float splitFactor;

    /**
     * True to subdivide invisible quads based on distance, like visible ones.
     * The default value of this flag is false.
     */
    bool splitInvisibleQuads;

    /**
     * True to perform horizon occlusion culling tests.
     */
    bool horizonCulling;

    /**
     * The maximum level at which the %terrain quadtree must be subdivided (inclusive).
     * The %terrain quadtree will never be subdivided beyond this level, even if the
     * viewer comes very close to the %terrain.
     */
    int maxLevel;

    /**
     * The %terrain elevation below the current viewer position. This field must be
     * updated manually by users (the TileSamplerZ class can do this for you).
     * It is used to compute the 3D distance between the viewer and a quad, to decide
     * whether this quad must be subdivided or not.
     */
    static float groundHeightAtCamera;

    /**
     * The value #groundHeightAtCamera will have at the next frame.
     */
    static float nextGroundHeightAtCamera;

    /**
     * Creates a new TerrainNode.
     *
     * @param deform the %terrain deformation.
     * @param root the root of the %terrain quadtree.
     * @param splitFactor how the %terrain quadtree must be subdivided (see
     *      #splitFactor).
     * @param maxLevel the maximum level at which the %terrain quadtree must be
     *      subdivided (inclusive).
     */
    TerrainNode(ptr<Deformation> deform, ptr<TerrainQuad> root, float splitFactor, int maxLevel);

    /**
     * Deletes this TerrainNode.
     */
    virtual ~TerrainNode();

    /**
     * Returns the current viewer position in the deformed %terrain space
     * (see #deform). This position is updated by the #update method.
     */
    vec3d getDeformedCamera() const;

    /**
     * Returns the current viewer frustum planes in the deformed #terrain
     * space (see #deform). These planes are updated by the #update method.
     */
    const vec4d *getDeformedFrustumPlanes() const;

    /**
     * Returns the current viewer position in the local %terrain space
     * (see #deform). This position is updated by the #update method.
     */
    vec3d getLocalCamera() const;

    /**
     * Returns the distance between the current viewer position and the
     * given bounding box. This distance is measured in the local %terrain
     * space (with Deformation#getLocalDist), with altitudes divided by
     * #getDistFactor() to take deformations into account.
     */
    float getCameraDist(const box3d &localBox) const;

    /**
     * Returns the visibility of the given bounding box from the current
     * viewer position. This visibility is computed with
     * Deformation#getVisbility.
     */
    SceneManager::visibility getVisibility(const box3d &localBox) const;

    /**
     * Returns the viewer distance at which a quad is subdivided, relatively
     * to the quad size. This relative distance is equal to #splitFactor for
     * a field of view of 80 degrees and a viewport width of 1024 pixels. It
     * is larger for smaller field of view angles and/or larger viewports.
     */
    float getSplitDistance() const;

    /**
     * Returns the ratio between local and deformed lengths at #getLocalCamera().
     */
    float getDistFactor() const;

    /**
     * Updates the %terrain quadtree based on the current viewer position.
     * The viewer position relatively to the local and deformed %terrain
     * spaces is computed based on the given SceneNode, which represents
     * the %terrain position in the scene graph (which also contains the
     * current viewer position).
     *
     * @param owner the SceneNode representing the terrain position in
     *      the global scene graph.
     */
    void update(ptr<SceneNode> owner);

    /**
     * Adds the given bounding box as an occluder. <i>The bounding boxes must
     * be added in front to back order</i>.
     *
     * @param occluder a bounding box in local (i.e. non deformed) coordinates.
     * @return true is the given bounding box is occluded by the bounding boxes
     *      previously added as occluders by this method.
     */
    bool addOccluder(const box3d &occluder);

    /**
     * Returns true if the given bounding box is occluded by the bounding boxes
     * previously added by #addOccluder().
     *
     * @param box a bounding box in local (i.e. non deformed) coordinates.
     * @return true is the given bounding box is occluded by the bounding boxes
     *      previously added as occluders by #addOccluder.
     **/
    bool isOccluded(const box3d &box);

protected:
    /**
     * Creates an uninitialized TerrainNode.
     */
    TerrainNode();

    /**
     * Initializes this TerrainNode.
     *
     * @param deform the %terrain deformation.
     * @param root the root of the %terrain quadtree.
     * @param splitFactor how the %terrain quadtree must be subdivided (see
     *      #splitFactor).
     * @param maxLevel the maximum level at which the %terrain quadtree must be
     *      subdivided (inclusive).
     */
    void init(ptr<Deformation> deform, ptr<TerrainQuad> root, float splitFactor, int maxLevel);

    void swap(ptr<TerrainNode> node);

private:
    /**
     * The current viewer position in the deformed %terrain space (see #deform).
     */
    vec3d deformedCameraPos;

    /**
     * The current viewer frustum planes in the deformed #terrain space (see #deform).
     */
    vec4d deformedFrustumPlanes[6];

    /**
     * The current viewer position in the local %terrain space (see #deform).
     */
    vec3d localCameraPos;

    /**
     * The viewer distance at which a quad is subdivided, relatively to the quad size.
     */
    float splitDist;

    /**
     * The ratio between local and deformed lengths at #localCameraPos.
     */
    float distFactor;

    /**
     * Local reference frame used to compute horizon occlusion culling.
     */
    mat2f localCameraDir;

    /**
     * Rasterized horizon elevation angle for each azimuth angle.
     */
    float *horizon;
};

}

#endif
