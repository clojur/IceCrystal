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

#ifndef _LargescaleScene_COLOR_H_
#define _LargescaleScene_COLOR_H_
#include "LargescaleSceneMacro.h"
#include "Soul/math/vec3.h"
#include "Soul/math/mat3.h"

using namespace Soul;

namespace LargescaleScene
{

/**
 * Converts a color from rgb space to hsl space. Color coordinates in [0,1]
 * @ingroup LargescaleScene_math
 */
LargescaleScene_API vec3f rgb2hsl(const vec3f &rgb);

/**
 * Converts a color from hsl space to rgb space. Color coordinates in [0,1]
 * @ingroup LargescaleScene_math
 */
LargescaleScene_API vec3f hsl2rgb(const vec3f &hsl);

/**
 * Computes the derivates of an rgb color with respect to hsl coordinates.
 * @ingroup LargescaleScene_math
 *
 * @param rgb a color in rgb space.
 * @param amp scaling factors for each partial derivative.
 * @return a matrix whose columns correspond to the partial derivatives drgb/dh,
 *      drgb/ds and drgb/dl, scaled by the factors amp.x, amp.y and amp.z.
 */
LargescaleScene_API mat3f dcolor(const vec3f &rgb, const vec3f &amp = vec3f(1/12.0f, 1/3.0f, 1/6.0f));

}

#endif
