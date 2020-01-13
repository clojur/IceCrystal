
#include "LargescaleScene/preprocess/atmo/PreprocessAtmo.h"

#include <cstdio>
#include <cstdlib>
#include <cstdlib>

#include "Soul/core/Logger.h"
#include "Soul/render/FrameBuffer.h"
#include "Soul/ui/GlfwWindow.h"

#include "LargescaleScene/preprocess/atmo/constants.glsl"
#include "LargescaleScene/preprocess/atmo/common.glsl"
#include "LargescaleScene/preprocess/atmo/copyInscatter1.glsl"
#include "LargescaleScene/preprocess/atmo/copyInscatterN.glsl"
#include "LargescaleScene/preprocess/atmo/copyIrradiance.glsl"
#include "LargescaleScene/preprocess/atmo/inscatter1.glsl"
#include "LargescaleScene/preprocess/atmo/inscatterN.glsl"
#include "LargescaleScene/preprocess/atmo/inscatterS.glsl"
#include "LargescaleScene/preprocess/atmo/irradiance1.glsl"
#include "LargescaleScene/preprocess/atmo/irradianceN.glsl"
#include "LargescaleScene/preprocess/atmo/transmittance.glsl"

using namespace std;
using namespace Soul;

namespace LargescaleScene
{

AtmoParameters::AtmoParameters() : Rg(6360.0), Rt(6420.0), RL(6421.0),
    TRANSMITTANCE_W(256), TRANSMITTANCE_H(64),
    SKY_W(64), SKY_H(16),
    RES_R(32), RES_MU(128), RES_MU_S(32), RES_NU(8),
    AVERAGE_GROUND_REFLECTANCE(0.1),
    HR(8.0), betaR(5.8e-3, 1.35e-2, 3.31e-2),
    HM(1.2), betaMSca(4e-3, 4e-3, 4e-3), betaMEx(4.44e-3, 4.44e-3, 4.44e-3), mieG(0.8)
{
}

class PreprocessAtmo : public GlfwWindow
{
public:
    AtmoParameters params;
    const char *output;
    ptr<Texture2D> transmittanceT;
    ptr<Texture2D> irradianceT;
    ptr<Texture3D> inscatterT;
    ptr<Texture2D> deltaET;
    ptr<Texture3D> deltaSRT;
    ptr<Texture3D> deltaSMT;
    ptr<Texture3D> deltaJT;
    ptr<Program> copyInscatter1;
    ptr<Program> copyInscatterN;
    ptr<Program> copyIrradiance;
    ptr<Program> inscatter1;
    ptr<Program> inscatterN;
    ptr<Program> inscatterS;
    ptr<Program> irradiance1;
    ptr<Program> irradianceN;
    ptr<Program> transmittance;
    ptr<FrameBuffer> fbo;
    int step;
    int order;

    PreprocessAtmo(const AtmoParameters &params, const char *output) :
        GlfwWindow(Window::Parameters().size(256, 256)), params(params), output(output)
    {
        Logger::INFO_LOGGER = NULL;

        transmittanceT = new Texture2D(params.TRANSMITTANCE_W, params.TRANSMITTANCE_H, RGB16F,
            RGB, FLOAT, Texture::Parameters().min(LINEAR).mag(LINEAR), Buffer::Parameters(), CPUBuffer(NULL));
        irradianceT = new Texture2D(params.SKY_W, params.SKY_H, RGB16F,
            RGB, FLOAT, Texture::Parameters().min(LINEAR).mag(LINEAR), Buffer::Parameters(), CPUBuffer(NULL));
        inscatterT = new Texture3D(params.RES_MU_S * params.RES_NU, params.RES_MU, params.RES_R, RGBA16F,
            RGBA, FLOAT, Texture::Parameters().min(LINEAR).mag(LINEAR), Buffer::Parameters(), CPUBuffer(NULL));
        deltaET = new Texture2D(params.SKY_W, params.SKY_H, RGB16F,
            RGB, FLOAT, Texture::Parameters().min(LINEAR).mag(LINEAR), Buffer::Parameters(), CPUBuffer(NULL));
        deltaSRT = new Texture3D(params.RES_MU_S * params.RES_NU, params.RES_MU, params.RES_R, RGB16F,
            RGB, FLOAT, Texture::Parameters().min(LINEAR).mag(LINEAR), Buffer::Parameters(), CPUBuffer(NULL));
        deltaSMT = new Texture3D(params.RES_MU_S * params.RES_NU, params.RES_MU, params.RES_R, RGB16F,
            RGB, FLOAT, Texture::Parameters().min(LINEAR).mag(LINEAR), Buffer::Parameters(), CPUBuffer(NULL));
        deltaJT = new Texture3D(params.RES_MU_S * params.RES_NU, params.RES_MU, params.RES_R, RGB16F,
            RGB, FLOAT, Texture::Parameters().min(LINEAR).mag(LINEAR), Buffer::Parameters(), CPUBuffer(NULL));

        copyInscatter1 = new Program(new Module(330,
            (string(constantsAtmoShader) + string(copyInscatter1Shader)).c_str()));
        copyInscatterN = new Program(new Module(330,
            (string(constantsAtmoShader) + string(commonAtmoShader) + string(copyInscatterNShader)).c_str()));
        copyIrradiance = new Program(new Module(330,
            (string(constantsAtmoShader) + string(copyIrradianceShader)).c_str()));
        inscatter1 = new Program(new Module(330,
            (string(constantsAtmoShader) + string(commonAtmoShader) + string(inscatter1Shader)).c_str()));
        inscatterN = new Program(new Module(330,
            (string(constantsAtmoShader) + string(commonAtmoShader) + string(inscatterNShader)).c_str()));
        inscatterS = new Program(new Module(330,
            (string(constantsAtmoShader) + string(commonAtmoShader) + string(inscatterSShader)).c_str()));
        irradiance1 = new Program(new Module(330,
            (string(constantsAtmoShader) + string(commonAtmoShader) + string(irradiance1Shader)).c_str()));
        irradianceN = new Program(new Module(330,
            (string(constantsAtmoShader) + string(commonAtmoShader) + string(irradianceNShader)).c_str()));
        transmittance = new Program(new Module(330,
            (string(constantsAtmoShader) + string(commonAtmoShader) + string(transmittanceShader)).c_str()));

        setParameters(copyInscatter1);
        setParameters(copyInscatterN);
        setParameters(copyIrradiance);
        setParameters(inscatter1);
        setParameters(inscatterN);
        setParameters(inscatterS);
        setParameters(irradiance1);
        setParameters(irradianceN);
        setParameters(transmittance);

        copyInscatter1->getUniformSampler("deltaSRSampler")->set(deltaSRT);
        copyInscatter1->getUniformSampler("deltaSMSampler")->set(deltaSMT);
        copyInscatterN->getUniformSampler("deltaSSampler")->set(deltaSRT);
        copyIrradiance->getUniformSampler("deltaESampler")->set(deltaET);
        inscatter1->getUniformSampler("transmittanceSampler")->set(transmittanceT);
        inscatterN->getUniformSampler("transmittanceSampler")->set(transmittanceT);
        inscatterN->getUniformSampler("deltaJSampler")->set(deltaJT);
        inscatterS->getUniformSampler("transmittanceSampler")->set(transmittanceT);
        inscatterS->getUniformSampler("deltaESampler")->set(deltaET);
        inscatterS->getUniformSampler("deltaSRSampler")->set(deltaSRT);
        inscatterS->getUniformSampler("deltaSMSampler")->set(deltaSMT);
        irradiance1->getUniformSampler("transmittanceSampler")->set(transmittanceT);
        irradianceN->getUniformSampler("deltaSRSampler")->set(deltaSRT);
        irradianceN->getUniformSampler("deltaSMSampler")->set(deltaSMT);

        fbo = new FrameBuffer();
        fbo->setReadBuffer(COLOR0);
        fbo->setDrawBuffer(COLOR0);

        step = 0;
        order = 2;
    }

    void setParameters(ptr<Program> p)
    {
        if (p->getUniform1f("Rg") != NULL) {
            p->getUniform1f("Rg")->set(params.Rg);
        }
        if (p->getUniform1f("Rt") != NULL) {
            p->getUniform1f("Rt")->set(params.Rt);
        }
        if (p->getUniform1f("RL") != NULL) {
            p->getUniform1f("RL")->set(params.RL);
        }
        if (p->getUniform1i("TRANSMITTANCE_W") != NULL) {
            p->getUniform1i("TRANSMITTANCE_W")->set(params.TRANSMITTANCE_W);
        }
        if (p->getUniform1i("TRANSMITTANCE_H") != NULL) {
            p->getUniform1i("TRANSMITTANCE_H")->set(params.TRANSMITTANCE_H);
        }
        if (p->getUniform1i("SKY_W") != NULL) {
            p->getUniform1i("SKY_W")->set(params.SKY_W);
        }
        if (p->getUniform1i("SKY_H") != NULL) {
            p->getUniform1i("SKY_H")->set(params.SKY_H);
        }
        if (p->getUniform1i("RES_R") != NULL) {
            p->getUniform1i("RES_R")->set(params.RES_R);
        }
        if (p->getUniform1i("RES_MU") != NULL) {
            p->getUniform1i("RES_MU")->set(params.RES_MU);
        }
        if (p->getUniform1i("RES_MU_S") != NULL) {
            p->getUniform1i("RES_MU_S")->set(params.RES_MU_S);
        }
        if (p->getUniform1i("RES_NU") != NULL) {
            p->getUniform1i("RES_NU")->set(params.RES_NU);
        }
        if (p->getUniform1f("AVERAGE_GROUND_REFLECTANCE") != NULL) {
            p->getUniform1f("AVERAGE_GROUND_REFLECTANCE")->set(params.AVERAGE_GROUND_REFLECTANCE);
        }
        if (p->getUniform1f("HR") != NULL) {
            p->getUniform1f("HR")->set(params.HR);
        }
        if (p->getUniform3f("betaR") != NULL) {
            p->getUniform3f("betaR")->set(params.betaR);
        }
        if (p->getUniform1f("HM") != NULL) {
            p->getUniform1f("HM")->set(params.HM);
        }
        if (p->getUniform3f("betaMSca") != NULL) {
            p->getUniform3f("betaMSca")->set(params.betaMSca);
        }
        if (p->getUniform3f("betaMEx") != NULL) {
            p->getUniform3f("betaMEx")->set(params.betaMEx);
        }
        if (p->getUniform1f("mieG") != NULL) {
            p->getUniform1f("mieG")->set(params.mieG);
        }
    }

    void setLayer(ptr<Program> p, int layer)
    {
        double r = layer / (params.RES_R - 1.0);
        r = r * r;
        r = sqrt(params.Rg * params.Rg + r * (params.Rt * params.Rt - params.Rg * params.Rg)) + (layer == 0 ? 0.01 : (layer == params.RES_R - 1 ? -0.001 : 0.0));
        double dmin = params.Rt - r;
        double dmax = sqrt(r * r - params.Rg * params.Rg) + sqrt(params.Rt * params.Rt - params.Rg * params.Rg);
        double dminp = r - params.Rg;
        double dmaxp = sqrt(r * r - params.Rg * params.Rg);
        if (p->getUniform1f("r") != NULL) {
            p->getUniform1f("r")->set(r);
        }
        if (p->getUniform4f("dhdH") != NULL) {
            p->getUniform4f("dhdH")->set(vec4f(dmin, dmax, dminp, dmaxp));
        }
        p->getUniform1i("layer")->set(layer);
    }

    void preprocess()
    {
        if (step == 0) {
            // computes transmittance texture T (line 1 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, transmittanceT, 0);
            fbo->setViewport(vec4i(0, 0, params.TRANSMITTANCE_W, params.TRANSMITTANCE_H));
            fbo->drawQuad(transmittance);
        } else if (step == 1) {
            // computes irradiance texture deltaE (line 2 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, deltaET, 0);
            fbo->setViewport(vec4i(0, 0, params.SKY_W, params.SKY_H));
            fbo->drawQuad(irradiance1);
        } else if (step == 2) {
            // computes single scattering texture deltaS (line 3 in algorithm 4.1)
            // Rayleigh and Mie separated in deltaSR + deltaSM
            fbo->setTextureBuffer(COLOR0, deltaSRT, 0, -1);
            fbo->setTextureBuffer(COLOR1, deltaSMT, 0, -1);
            fbo->setDrawBuffers((BufferId) (COLOR0 | COLOR1));
            fbo->setViewport(vec4i(0, 0, params.RES_MU_S * params.RES_NU, params.RES_MU));
            for (int layer = 0; layer < params.RES_R; ++layer) {
                setLayer(inscatter1, layer);
                fbo->drawQuad(inscatter1);
            }
            fbo->setTextureBuffer(COLOR1, (Texture2D*) NULL, 0);
            fbo->setDrawBuffer(COLOR0);
        } else if (step == 3) {
            // copies deltaE into irradiance texture E (line 4 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, irradianceT, 0);
            fbo->setViewport(vec4i(0, 0, params.SKY_W, params.SKY_H));
            copyIrradiance->getUniform1f("k")->set(0.0);
            fbo->drawQuad(copyIrradiance);
        } else if (step == 4) {
            // copies deltaS into inscatter texture S (line 5 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, inscatterT, 0, -1);
            fbo->setViewport(vec4i(0, 0, params.RES_MU_S * params.RES_NU, params.RES_MU));
            for (int layer = 0; layer < params.RES_R; ++layer) {
                setLayer(copyInscatter1, layer);
                fbo->drawQuad(copyInscatter1);
            }
        } else if (step == 5) {
            // computes deltaJ (line 7 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, deltaJT, 0, -1);
            fbo->setViewport(vec4i(0, 0, params.RES_MU_S * params.RES_NU, params.RES_MU));
            inscatterS->getUniform1f("first")->set(order == 2 ? 1.0 : 0.0);
            for (int layer = 0; layer < params.RES_R; ++layer) {
                setLayer(inscatterS, layer);
                fbo->drawQuad(inscatterS);
            }
        } else if (step == 6) {
            // computes deltaE (line 8 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, deltaET, 0);
            fbo->setViewport(vec4i(0, 0, params.SKY_W, params.SKY_H));
            irradianceN->getUniform1f("first")->set(order == 2 ? 1.0 : 0.0);
            fbo->drawQuad(irradianceN);
        } else if (step == 7) {
            // computes deltaS (line 9 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, deltaSRT, 0, -1);
            fbo->setViewport(vec4i(0, 0, params.RES_MU_S * params.RES_NU, params.RES_MU));
            for (int layer = 0; layer < params.RES_R; ++layer) {
                setLayer(inscatterN, layer);
                fbo->drawQuad(inscatterN);
            }
        } else if (step == 8) {
            fbo->setBlend(true, ADD, ONE, ONE);
            // adds deltaE into irradiance texture E (line 10 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, irradianceT, 0);
            fbo->setViewport(vec4i(0, 0, params.SKY_W, params.SKY_H));
            copyIrradiance->getUniform1f("k")->set(1.0);
            fbo->drawQuad(copyIrradiance);
        } else if (step == 9) {
            // adds deltaS into inscatter texture S (line 11 in algorithm 4.1)
            fbo->setTextureBuffer(COLOR0, inscatterT, 0, -1);
            fbo->setViewport(vec4i(0, 0, params.RES_MU_S * params.RES_NU, params.RES_MU));
            for (int layer = 0; layer < params.RES_R; ++layer) {
                setLayer(copyInscatterN, layer);
                fbo->drawQuad(copyInscatterN);
            }
            fbo->setBlend(false);
            if (order < 4) {
                step = 4;
                order += 1;
            }
        } else if (step == 10) {
            int trailer[5];
            trailer[0] = 0xCAFEBABE;
            trailer[1] = params.TRANSMITTANCE_W;
            trailer[2] = params.TRANSMITTANCE_H;
            trailer[3] = 0;
            trailer[4] = 3;
            float *buf = new float[3 * params.TRANSMITTANCE_W * params.TRANSMITTANCE_H];
            transmittanceT->getImage(0, RGB, FLOAT, buf);
            char name[512];
            sprintf(name, "%s/transmittance.raw", output);
            FILE *f;
            fopen(&f, name, "wb");
            fwrite(buf, 3 * params.TRANSMITTANCE_W * params.TRANSMITTANCE_H * sizeof(float), 1, f);
            fwrite(trailer, 5 * sizeof(int), 1, f);
            fclose(f);
            delete[] buf;
        } else if (step == 11) {
            int trailer[5];
            trailer[0] = 0xCAFEBABE;
            trailer[1] = params.SKY_W;
            trailer[2] = params.SKY_H;
            trailer[3] = 0;
            trailer[4] = 3;
            float *buf = new float[3 * params.SKY_W * params.SKY_H];
            irradianceT->getImage(0, RGB, FLOAT, buf);
            char name[512];
            sprintf(name, "%s/irradiance.raw", output);
            FILE *f;
            fopen(&f, name, "wb");
            fwrite(buf, 3 * params.SKY_W * params.SKY_H * sizeof(float), 1, f);
            fwrite(trailer, 5 * sizeof(int), 1, f);
            fclose(f);
            delete[] buf;
        } else if (step == 12) {
            int trailer[5];
            trailer[0] = 0xCAFEBABE;
            trailer[1] = params.RES_MU_S * params.RES_NU;
            trailer[2] = params.RES_MU * params.RES_R;
            trailer[3] = params.RES_R;
            trailer[4] = 4;
            float *buf = new float[4 * params.RES_MU_S * params.RES_NU * params.RES_MU * params.RES_R];
            inscatterT->getImage(0, RGBA, FLOAT, buf);
            char name[512];
            sprintf(name, "%s/inscatter.raw", output);
            FILE *f;
            fopen(&f, name, "wb");
            fwrite(buf, 4 * params.RES_MU_S * params.RES_NU * params.RES_MU * params.RES_R * sizeof(float), 1, f);
            fwrite(trailer, 5 * sizeof(int), 1, f);
            fclose(f);
            delete[] buf;
        } else if (step == 13) {
            printf("PRECOMPUTATONS DONE. RESTART APPLICATION.\n\n");
            ::exit(0);
        }
        step += 1;
    }

    void redisplay(double t, double dt)
    {
        preprocess();
        GlfwWindow::redisplay(t, dt);
    }

    static static_ptr<Window> app;
};

static_ptr<Window> PreprocessAtmo::app;

void preprocessAtmo(const AtmoParameters &params, const char *output)
{
    char name[512];
    sprintf(name, "%s/inscatter.raw", output);
    FILE *f;
    fopen(&f, name, "rb");
    if (f != NULL) {
        fclose(f);
    } else {
        PreprocessAtmo::app = new PreprocessAtmo(params, output);
        PreprocessAtmo::app->start();
    }
}

}
