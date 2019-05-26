/*
 * Proland: a procedural landscape rendering library.
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
 * Proland is distributed under a dual-license scheme.
 * You can obtain a specific license from Inria: LargescaleScene-licensing@inria.fr.
 */

/**
 * Precomputed Atmospheric Scattering
 * Copyright (c) 2008 INRIA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Author: Eric Bruneton
 */

// adds deltaS into S (line 11 in algorithm 4.1)

namespace LargescaleScene
{

const char *copyInscatterNShader = "\n\
uniform float r;\n\
uniform vec4 dhdH;\n\
uniform int layer;\n\
\n\
uniform sampler3D deltaSSampler;\n\
\n\
#ifdef _VERTEX_\n\
\n\
layout(location=0) in vec4 vertex;\n\
\n\
void main() {\n\
    gl_Position = vertex;\n\
}\n\
\n\
#endif\n\
\n\
#ifdef _GEOMETRY_\n\
#extension GL_EXT_geometry_shader4 : enable\n\
\n\
layout(triangles) in;\n\
layout(triangle_strip,max_vertices=3) out;\n\
\n\
void main() {\n\
    gl_Position = gl_PositionIn[0];\n\
    gl_Layer = layer;\n\
    EmitVertex();\n\
    gl_Position = gl_PositionIn[1];\n\
    gl_Layer = layer;\n\
    EmitVertex();\n\
    gl_Position = gl_PositionIn[2];\n\
    gl_Layer = layer;\n\
    EmitVertex();\n\
    EndPrimitive();\n\
}\n\
\n\
#endif\n\
\n\
#ifdef _FRAGMENT_\n\
\n\
layout(location=0) out vec4 data;\n\
\n\
void main() {\n\
    float mu, muS, nu;\n\
    getMuMuSNu(r, dhdH, mu, muS, nu);\n\
    vec3 uvw = vec3(gl_FragCoord.xy, float(layer) + 0.5) / vec3(ivec3(RES_MU_S * RES_NU, RES_MU, RES_R));\n\
    data = vec4(texture(deltaSSampler, uvw).rgb / phaseFunctionR(nu), 0.0);\n\
}\n\
\n\
#endif\n\
";

}
