

#include "LargescaleScene/rivers/WaveTile.h"

#include "Soul/resource/ResourceTemplate.h"

using namespace std;
using namespace Soul;

namespace LargescaleScene
{

WaveTile::WaveTile() : Object("WaveTile"), tex(NULL), gridSize(0), tileSize(0), timeLoop(0), waveLength(1.0f)
{
}

WaveTile::WaveTile(string &name, ptr<Texture2D> tex, int gridSize, int tileSize, float waveLength, int timeLoop) : Object("WaveTile")
{
    init(name, tex, gridSize, tileSize, waveLength, timeLoop);
}

WaveTile::~WaveTile()
{
}

void WaveTile::timeStep(float dt)
{
    time += dt;
}

void WaveTile::init(string &name, ptr<Texture2D> tex, int gridSize, int tileSize, float waveLength, int timeLoop)
{
    this->tex = tex;
    this->gridSize = gridSize;
    this->tileSize = tileSize;
    this->waveLength = waveLength;
    this->timeLoop = timeLoop;
    this->time = 0.f;
    this->name = name;
    lastProgram = NULL;
}

float WaveTile::getWaveLength() const
{
    return waveLength;
}

void WaveTile::setWaveLength(float length)
{
    waveLength = length;
}

void WaveTile::checkUniforms(ptr<Program> p)
{
    if (lastProgram != p) {
        patternTexU = p->getUniformSampler(name + ".patternTex");
        patternTexSizeU = p->getUniform1f(name + ".patternTexSize");
        lengthU = p->getUniform1f(name + ".length");
        timeU = p->getUniform1f(name + ".time");
        timeLoopU = p->getUniform1f(name + ".timeLoop");

        lastProgram = p;
    }
}

void WaveTile::updateUniform(ptr<Program> p)
{
    checkUniforms(p);

    if (patternTexU != NULL) {
        patternTexU->set(tex);
    }
    if (patternTexSizeU != NULL) {
        patternTexSizeU->set(tileSize);
    }
    if (lengthU != NULL) {
        lengthU->set(waveLength);
    }
    if (timeU != NULL) {
        timeU->set(time);
    }
    if (timeLoopU != NULL) {
        timeLoopU->set(timeLoop);
    }
}

void WaveTile::swap(ptr<WaveTile> t)
{
    std::swap(tex, t->tex);
}

class WaveTileResource : public ResourceTemplate<50, WaveTile>
{
public:
    WaveTileResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<50, WaveTile>(manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;

        checkParameters(desc, e, "name,samplerName,texture,gridSize,tileSize,waveLength,timeLoop,");

        ptr<Texture2D> tex = manager->loadResource(getParameter(desc, e, "texture")).cast<Texture2D>();
        int gridSize = 256;
        int tileSize = 1;
        float waveLength = 1.0f;
        int timeLoop = 64;
        string sName = e->Attribute("samplerName");

        if (e->Attribute("gridSize") != NULL) {
            getIntParameter(desc, e, "gridSize", &gridSize);
        }
        if (e->Attribute("tileSize") != NULL) {
            getIntParameter(desc, e, "tileSize", &tileSize);
        }
        if (e->Attribute("waveLength") != NULL) {
            getFloatParameter(desc, e, "waveLength", &waveLength);
        }
        if (e->Attribute("timeLoop") != NULL) {
            getIntParameter(desc, e, "timeLoop", &timeLoop);
        }
        init(sName, tex, gridSize, tileSize, waveLength, timeLoop);
    }
};

extern const char waveTile[] = "waveTile";

static ResourceFactory::Type<waveTile, WaveTileResource> WaveTileType;

}

