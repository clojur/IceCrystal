#ifndef _LARGESCALESCENE_DRAW_OCEAN_TASK_H_
#define _LARGESCALESCENE_DRAW_OCEAN_TASK_H_
#include "LargescaleSceneMacro.h"
#include "Soul/scenegraph/AbstractTask.h"
#include "Soul/render/Mesh.h"
#include "Soul/render/Uniform.h"

using namespace Soul;

namespace LargescaleScene
{

/**
 * @defgroup ocean ocean
 * Provides a task to draw a flat or spherical animated %ocean.
 * @ingroup LargescaleScene
 */

/**
 * An AbstractTask to draw a flat or spherical %ocean.
 * @ingroup ocean
 * @authors Eric Bruneton, Antoine Begault, Guillaume Piolat
 */
class LargescaleScene_API DrawOceanTask : public AbstractTask
{
public:
    /**
     * Creates a new DrawOceanTask.
     *
     * @param radius the radius of the planet for a spherical ocean, or
     *      0 for a flat ocean.
     * @param zmin the maximum altitude at which the ocean must be
     *      displayed.
     * @param brdfShader the Shader used to render the ocean surface.
     */
    DrawOceanTask(float radius, float zmin, ptr<Module> brdfShader);

    /**
     * Deletes this DrawOceanTask.
     */
    virtual ~DrawOceanTask();

    virtual ptr<Task> getTask(ptr<Object> context);

protected:
    /**
     * Creates an uninitialized DrawOceanTask.
     */
    DrawOceanTask();

    /**
     * Initializes this DrawOceanTask.
     *
     * @param radius the radius of the planet for a spherical ocean, or
     *      0 for a flat ocean.
     * @param zmin the maximum altitude at which the ocean must be
     *      displayed.
     * @param brdfShader the Shader used to display the ocean surface.
     */
    void init(float radius, float zmin, ptr<Module> brdfShader);

    void swap(ptr<DrawOceanTask> t);

private:
    /**
     * The radius of the planet for a spherical ocean, or 0 for a flat ocean.
     */
    float radius;

    /**
     * The maximum altitude at which the ocean must be displayed.
     */
    float zmin;

    /**
     * Number of wave trains used to synthesize the ocean surface.
     */
    int nbWaves;

    /**
     * Minimum wavelength of the waves.
     */
    float lambdaMin;

    /**
     * Maximum wavelength of the waves.
     */
    float lambdaMax;

    /**
     * Parameter to color the height of waves.
     */
    float heightMax;

    /**
     * Color of the seabed.
     */
    vec3f seaColor;

    // -------

    /**
     * Variance of the x slope over the sea surface.
     */
    float sigmaXsq;

    /**
     * Variance of the y slope over the sea surface.
     */
    float sigmaYsq;

    /**
     * Average height of the sea surface.
     */
    float meanHeight;

    /**
     * Variance of the sea surface height.
     */
    float heightVariance;

    /**
     * Maximum amplitude between crests and throughs.
     */
    float amplitudeMax;

    // -------

    /**
     * Number of pixels per cell to use for the screen space grid
     * used to display the ocean surface.
     */
    int resolution;

    /**
     * Current width of the viewport, in pixels.
     */
    int screenWidth;

    /**
     * Current height of the viewport, in pixels.
     */
    int screenHeight;

    /**
     * The mesh used to display the ocean surface.
     */
    ptr< Mesh<vec2f, unsigned int> > screenGrid;

    // -------

    mat4d oldLtoo;

    vec3d offset;

    // -------

    /**
     * The Shader used to render the ocean surface.
     */
    ptr<Module> brdfShader;

    ptr<Uniform1f> nbWavesU;

    ptr<UniformSampler> wavesU;

    ptr<UniformMatrix4f> cameraToOceanU;

    ptr<UniformMatrix4f> screenToCameraU;

    ptr<UniformMatrix4f> cameraToScreenU;

    ptr<UniformMatrix4f> oceanToWorldU;

    ptr<UniformMatrix3f> oceanToCameraU;

    ptr<Uniform3f> oceanCameraPosU;

    ptr<Uniform3f> oceanSunDirU;

    ptr<Uniform3f> horizon1U;

    ptr<Uniform3f> horizon2U;

    ptr<Uniform1f> timeU;

    ptr<Uniform1f> radiusU;

    ptr<Uniform1f> heightOffsetU;

    ptr<Uniform4f> lodsU;

    void generateWaves();

    class Impl : public Task
    {
    public:
        ptr<SceneNode> n;

        ptr<DrawOceanTask> o;

        Impl(ptr<SceneNode> n, ptr<DrawOceanTask> owner);

        virtual ~Impl();

        virtual bool run();
    };
};

}

#endif
