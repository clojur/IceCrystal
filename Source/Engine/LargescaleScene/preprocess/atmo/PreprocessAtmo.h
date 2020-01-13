/*
 * LargescaleScene: a procedural landscape rendering library.
 *
 */

 /**
  * Author: Jiangsi
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
