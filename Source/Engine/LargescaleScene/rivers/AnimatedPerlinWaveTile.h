
#ifndef _LargescaleScene_ANIMATEDPERLINWAVETILE_H_
#define _LargescaleScene_ANIMATEDPERLINWAVETILE_H_
#include "LargescaleScene/common/LargescaleSceneMacro.h"
#include "LargescaleScene/rivers/WaveTile.h"

namespace LargescaleScene
{

/**
 * WaveTile are Texture used to advect rivers normal.
 * They can be updated through time, in order to change the waves profiles.
 * AnimatedPerlinWaveTile is a serie of #timeLoop Noise Textures displayed successively.
 * @ingroup rivers
 * @author Antoine Begault
 */
class LargescaleScene_API  AnimatedPerlinWaveTile : public WaveTile
{
public:
    /**
     * Creates a new AnimatedPerlinWaveTile.
     * See WaveTile#WaveTile().
     */
    AnimatedPerlinWaveTile(std::string &name, int tileSize, int gridSize, float waveLength, int timeLoop);

    /**
     * Deletes an AnimatedPerlinWaveTile.
     */
    virtual ~AnimatedPerlinWaveTile();

    /**
     * See WaveTile#updateUniform().
     */
    virtual void updateUniform(ptr<Program> p);

protected:
    /**
     * 3D Noise generator. Taken from Qizhi's implementation.
     */
    struct Noise //3D noise
    {
    public:
       Noise (unsigned int seed=171717);

       void reinitialize(unsigned int seed);

       float operator()(float x, float y, float z) const;

       float operator()(float x, float y, float z, int wxy, int wz) const;

       float operator()(const vec3f &x) const
       {
           return (*this)(x[0], x[1], x[2]);
       }

    protected:
       static const unsigned int n=256;

       vec3f basis[n];

       int perm[n];

       unsigned int hash_index(int i, int j, int k, int wxy, int wz) const
       {
           return perm[(perm[(perm[i % wxy] + j) % wxy] + k % wz) % wxy];
       }

       unsigned int hash_index(int i, int j, int k) const
       {
           return perm[(perm[(perm[i % n] + j) % n] + k) % n];
       }
    };

    /**
     * Creates a new AnimatedPerlinWaveTile.
     */
    AnimatedPerlinWaveTile();

    /**
     * Initializes the fields of a AnimatedPerlinWaveTile.
     * See WaveTile#init().
     */
    void init(ptr<Texture2D> t, int size, int numLodLevels, int timeLoop, unsigned int seed);

    /**
     * Initializes the fields of a AnimatedPerlinWaveTile.
     * See WaveTile#init().
     */
    virtual void init(std::string &name, int tileSize, int gridSize, float waveLength, int timeLoop);

    virtual void swap(ptr<AnimatedPerlinWaveTile> t);

    /**
     * Contains the textures used to animate the wave profile.
     */
    std::vector<ptr<Texture2D> > tex;
};

}

#endif
