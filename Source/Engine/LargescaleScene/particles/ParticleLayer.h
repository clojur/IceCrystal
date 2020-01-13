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

#ifndef _LargescaleScene_PARTICLE_LAYER_H_
#define _LargescaleScene_PARTICLE_LAYER_H_
#include "LargescaleSceneMacro.h"
#include "LargescaleScene/particles/ParticleStorage.h"
#include "LargescaleScene/producer/TileProducer.h"

namespace LargescaleScene
{

class ParticleProducer;

/**
 * An abstract layer for a ParticleProducer.
 * @ingroup particles
 * @authors Eric Bruneton, Antoine Begault
 */
class LargescaleScene_API  ParticleLayer : public Object
{
public:
    /**
     * Creates a new ParticleLayer.
     *
     * @param type the layer's type.
     * @param particleSize the size in bytes of the layer specific data that
     *      must be stored for each particle.
     */
    ParticleLayer(const char *type, int particleSize);

    /**
     * Deletes this ParticleLayer.
     */
    virtual ~ParticleLayer();

    /**
     * Returns the ParticleProducer to which this ParticleLayer belongs.
     */
    ParticleProducer *getOwner();

    /**
     * Returns true if this layer is enabled. If it is not the case, its
     * #moveParticles, #removeOldParticles and #addNewParticles will not
     * be called by the ParticleProducer.
     */
    bool isEnabled();

    /**
     * Enables or disables this ParticleLayer. The #moveParticles,
     * #removeOldParticles and #addNewParticles methods are called only
     * on enabled layers.
     *
     * @param enable true to enable this layer, false otherwise.
     */
    void setIsEnabled(bool enable);

    /**
     * Returns the size in bytes of the layer specific data that must be
     * stored for each particle.
     */
    virtual int getParticleSize();

    /**
     * Returns a pointer to the layer specific data of the given particle.
     *
     * @param p a particle.
     * @return a pointer to the data that is specific to this layer for the
     *      given particle.
     */
    inline void *getParticleData(ParticleStorage::Particle *p)
    {
        return (void*) (((unsigned char*) p) + offset);
    }

    /**
     * Returns a pointer to the Particle corresponding to the given layer
     * specific data.
     *
     * @param p a pointer to the data that is specific to this layer for a
     *      given particle.
     * @return the corresponding Particle.
     */
    inline ParticleStorage::Particle *getParticle(void *p)
    {
        return (ParticleStorage::Particle*) (((unsigned char*) p) - offset);
    }

    /**
     * Returns the tile producers used by this ParticleLayer.
     *
     * @param[out] producers the tile producers used by this ParticleLayer.
     */
    virtual void getReferencedProducers(std::vector< ptr<TileProducer> > &producers) const
    {
    }

    /**
     * Moves the existing %particles.
     * The default implementation of this method does nothing.
     *
     * @param dt the elapsed time since the last call to this method, in
     *      microseconds.
     */
    virtual void moveParticles(double dt);

    /**
     * Removes old %particles.
     * The default implementation of this method does nothing.
     */
    virtual void removeOldParticles();

    /**
     * Adds new %particles.
     * The default implementation of this method does nothing.
     */
    virtual void addNewParticles();

protected:
    /**
     * Initializes this ParticleLayer.
     */
    void init(int particleSize);

    /**
     * Initializes this ParticleLayer. If this layer needs reference to other
     * layers it can initialize them in this method (using the template method
     * ParticleProducer#getLayer()). The default implementation of this method
     * does nothing.
     */
    virtual void initialize();

    /**
     * Initializes the data that is specific to this layer in the given
     * particle. The default implementation of this method does nothing.
     */
    virtual void initParticle(ParticleStorage::Particle *p);

    virtual void swap(ptr<ParticleLayer> p);

private:
    /**
     * The ParticleProducer to which this ParticleLayer belongs.
     */
    ParticleProducer *owner;

    /**
     * The size in bytes of the layer specific data that must be stored for
     * each particle.
     */
    int size;

    /**
     * The offset of the data that is specific to this layer in the global
     * particle data.
     */
    int offset;

    /**
     * True if this layer is enabled.
     */
    bool enabled;

    friend class ParticleProducer;
};

}

#endif
