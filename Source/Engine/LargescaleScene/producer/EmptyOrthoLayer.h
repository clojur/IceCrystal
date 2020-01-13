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

#ifndef _LargescaleScene_EMPTYORTHOLAYER_H_
#define _LargescaleScene_EMPTYORTHOLAYER_H_
#include "LargescaleSceneMacro.h"
#include "Soul/math/vec4.h"
#include "LargescaleScene/producer/TileLayer.h"

using namespace Soul;
using namespace std;
namespace LargescaleScene
{

/**
 * An OrthoGPUProducer layer that simply fills tiles with a constant color.
 * This layer is useful when the OrthoGPUProducer is used without an
 * OrthoCPUProducer. It can provide a background color for other layers.
 * @ingroup ortho
 * @authors Eric Bruneton, Antoine Begault
 */
class LargescaleScene_API EmptyOrthoLayer : public TileLayer
{
public:
    /**
     * Creates a new EmptyOrthoLayer.
     *
     * @param color the color to be used to fill the produced tiles.
     */
    EmptyOrthoLayer(vec4f color);

    /**
     * Deletes this EmptyOrthoLayer.
     */
    virtual ~EmptyOrthoLayer();

    virtual bool doCreateTile(int level, int tx, int ty, TileStorage::Slot *data);

protected:
    /**
     * Creates an uninitialized EmptyOrthoLayer.
     */
    EmptyOrthoLayer();

    /**
     * Initializes this EmptyOrthoLayer.
     *
     * @param color the color to be used to fill the produced tiles.
     */
    void init(vec4f color);

    virtual void swap(ptr<EmptyOrthoLayer> p);

private:
    /**
     * The color to be used to fill the produced tiles.
     */
    vec4f color;
};

}

#endif
