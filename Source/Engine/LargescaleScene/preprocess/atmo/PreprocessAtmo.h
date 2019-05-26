/*
 * LargescaleScene: a procedural landscape rendering library.
 * Website : http://LargescaleScene.inrialpes.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * LargescaleScene is distributed under the Berkeley Software Distribution 3 Licence. 
 * For any assistance, feedback and enquiries about training programs, you can check out the 
 * contact page on our website : 
 * http://LargescaleScene.inrialpes.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#ifndef _LargescaleScene_PREPROCESS_ATMO_
#define _LargescaleScene_PREPROCESS_ATMO_

#include "LargescaleSceneMacro.h"
#include "Soul/math/vec3.h"

using namespace Soul;
using namespace std;
namespace LargescaleScene
{

/**
 * Paramaters describing an atmosphere and the size of its precomputed tables.
 *
 * @ingroup preprocess
 * @author Eric Bruneton
 */
class LargescaleScene_API AtmoParameters
{
public:
    float Rg;
    float Rt;
    float RL;
    int TRANSMITTANCE_W;
    int TRANSMITTANCE_H;
    int SKY_W;
    int SKY_H;
    int RES_R;
    int RES_MU;
    int RES_MU_S;
    int RES_NU;
    float AVERAGE_GROUND_REFLECTANCE;
    float HR;
    vec3f betaR;
    float HM;
    vec3f betaMSca;
    vec3f betaMEx;
    float mieG;

    AtmoParameters();
};

/**
 * Precomputes the tables for the given atmosphere parameters.
 *
 * @param params the atmosphere parameters.
 * @param output the folder where to write the generated tables.
 */
LargescaleScene_API void preprocessAtmo(const AtmoParameters &params, const char *output);

}

#endif
