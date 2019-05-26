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

// computes ground irradiance due to skylight E[deltaS] (line 8 in algorithm 4.1)

namespace LargescaleScene
{

const char *irradianceNShader = "\n\
uniform sampler3D deltaSRSampler;\n\
uniform sampler3D deltaSMSampler;\n\
uniform float first;\n\
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
#ifdef _FRAGMENT_\n\
\n\
const float dphi = M_PI / float(IRRADIANCE_INTEGRAL_SAMPLES);\n\
const float dtheta = M_PI / float(IRRADIANCE_INTEGRAL_SAMPLES);\n\
\n\
layout(location=0) out vec4 data;\n\
\n\
void main() {\n\
    float r, muS;\n\
    getIrradianceRMuS(r, muS);\n\
    vec3 s = vec3(sqrt(max(1.0 - muS * muS, 0.0)), 0.0, muS);\n\
\n\
    vec3 result = vec3(0.0);\n\
    // integral over 2.PI around x with two nested loops over w directions (theta,phi) -- Eq (15)\n\
    for (int iphi = 0; iphi < 2 * IRRADIANCE_INTEGRAL_SAMPLES; ++iphi) {\n\
        float phi = (float(iphi) + 0.5) * dphi;\n\
        for (int itheta = 0; itheta < IRRADIANCE_INTEGRAL_SAMPLES / 2; ++itheta) {\n\
            float theta = (float(itheta) + 0.5) * dtheta;\n\
            float dw = dtheta * dphi * sin(theta);\n\
            vec3 w = vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));\n\
            float nu = dot(s, w);\n\
            if (first == 1.0) {\n\
                // first iteration is special because Rayleigh and Mie were stored separately,\n\
                // without the phase functions factors; they must be reintroduced here\n\
                float pr1 = phaseFunctionR(nu);\n\
                float pm1 = phaseFunctionM(nu);\n\
                vec3 ray1 = texture4D(deltaSRSampler, r, w.z, muS, nu).rgb;\n\
                vec3 mie1 = texture4D(deltaSMSampler, r, w.z, muS, nu).rgb;\n\
                result += (ray1 * pr1 + mie1 * pm1) * w.z * dw;\n\
            } else {\n\
                result += texture4D(deltaSRSampler, r, w.z, muS, nu).rgb * w.z * dw;\n\
            }\n\
        }\n\
    }\n\
\n\
    data = vec4(result, 1.0);\n\
}\n\
\n\
#endif\n\
";

}
