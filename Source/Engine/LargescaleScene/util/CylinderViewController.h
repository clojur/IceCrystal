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

#ifndef _LargescaleScene_CYLINDER_VIEW_CONTROLLER_H_
#define _LargescaleScene_CYLINDER_VIEW_CONTROLLER_H_

#include "LargescaleSceneMacro.h"
#include "LargescaleScene/util/TerrainViewController.h"

using namespace Soul;

namespace LargescaleScene
{

/**
 * A TerrainViewController for cylindrical terrains. This subclass
 * interprets the #y0 field as a "longitude" inside the cylinder,
 * and considers #theta and #phi as relative to the tangent plane at
 * the (#x0,#y0) point.
 * @ingroup LargescaleScene_util
 * @authors Eric Bruneton, Antoine Begault
 */
class LargescaleScene_API CylinderViewController : public TerrainViewController
{
public:
    /**
     * The radius of the cylindrical terrain at sea level.
     */
    const double R;

    /**
     * Creates a new CylinderViewController.
     *
     * @param node a SceneNode representing a camera position and orientation
     *      in the scene.
     * @param R the cylindrical terrain radius at sea level.
     */
    CylinderViewController(ptr<SceneNode> node, double R);

    /**
     * Deletes this PlanetViewController.
     */
    virtual ~CylinderViewController();

    virtual double getHeight();

    virtual void move(vec3d &oldp, vec3d &p);

    virtual void update();
};

}

#endif
