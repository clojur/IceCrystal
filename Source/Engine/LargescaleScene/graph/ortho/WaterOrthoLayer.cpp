

#include "LargescaleScene/graph/ortho/WaterOrthoLayer.h"

#include "Soul/resource/ResourceTemplate.h"
#include "Soul/scenegraph/SceneManager.h"

#include "LargescaleScene/graph/graph/Area.h"
#include "LargescaleScene/producer/ObjectTileStorage.h"
#include "LargescaleScene/graph/ortho/OrthoMargin.h"

using namespace Soul;

namespace LargescaleScene
{

WaterOrthoLayer::WaterOrthoLayer() :
    GraphLayer("WaterOrthoLayer")
{
}

WaterOrthoLayer::WaterOrthoLayer(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram, int displayLevel, bool quality, vec4f color, bool deform) :
    GraphLayer("WaterOrthoLayer")
{
    init(graphProducer, layerProgram, displayLevel, quality, color, deform);
}

void WaterOrthoLayer::init(ptr<GraphProducer> graphProducer, ptr<Program> layerProgram, int displayLevel, bool quality, vec4f color, bool deform)
{
    GraphLayer::init(graphProducer, layerProgram, displayLevel, quality, false, deform);
    this->color = color;
    this->mesh = new Mesh<vec2f, unsigned int>(TRIANGLE_STRIP, GPU_STREAM);
    this->mesh->addAttributeType(0, 2, A32F, false);
    this->tess = new Tesselator();

    tileOffsetU = layerProgram->getUniform3f("tileOffset");
    colorU = layerProgram->getUniform4f("color");
}

WaterOrthoLayer::~WaterOrthoLayer()
{
}

void WaterOrthoLayer::setTileSize(int tileSize, int tileBorder, float rootQuadSize)
{
    GraphLayer::setTileSize(tileSize, tileBorder, rootQuadSize);
    float borderFactor = tileSize / (tileSize - 1.0f - 2.0f * tileBorder) - 1.0f;
    graphProducer->addMargin(new OrthoMargin(tileSize - 2 * tileBorder, borderFactor, 1.0f));
}

bool WaterOrthoLayer::doCreateTile(int level, int tx, int ty, TileStorage::Slot *data)
{
    if (Logger::DEBUG_LOGGER != NULL) {
        ostringstream oss;
        oss << "OrthoWater tile " << getProducerId() << " " << level << " " << tx << " " << ty;
        Logger::DEBUG_LOGGER->log("ORTHO", oss.str());
    }
    if (level >= displayLevel) {
        TileCache::Tile * t = graphProducer->findTile(level, tx, ty);
        assert(t != NULL);
        ObjectTileStorage::ObjectSlot *graphData = dynamic_cast<ObjectTileStorage::ObjectSlot*>(t->getData());
        GraphPtr g = graphData->data.cast<Graph>();
        if (g != NULL) {
            ptr<FrameBuffer> fb = SceneManager::getCurrentFrameBuffer();

            vec3d q = getTileCoords(level, tx, ty);
            vec2d nx, ny, lx, ly;
            getDeformParameters(q, nx, ny, lx, ly);

            float scale = 2.0f * (1.0f - getTileBorder() * 2.0f / getTileSize()) / q.z;
            float scale2 = 2.0f * (getTileSize() - 2.0f * getTileBorder()) / q.z;
            vec3d tileOffset = vec3d(q.x + q.z / 2.0f, q.y + q.z / 2.0f, scale);
            //tileOffsetU->set(vec3f(q.x + q.z / 2.0f, q.y + q.z / 2.0f, scale));
            tileOffsetU->set(vec3f(0.0, 0.0, 1.0));
            colorU->set(color);
            mesh->setMode(TRIANGLES);
            mesh->clear();
            tess->beginPolygon(mesh);
            ptr<Graph::AreaIterator> ai = g->getAreas();
            while (ai->hasNext()) {
                AreaPtr a = ai->next();
                drawArea(tileOffset, a, *tess);
            }
            tess->endPolygon();
            fb->draw(layerProgram, *mesh);

            fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA, ADD, ONE, ZERO);

            ptr<Graph::CurveIterator> ci = g->getCurves();
            while (ci->hasNext()) {
                CurvePtr p = ci->next();
                float pwidth = p->getWidth();
                float swidth = pwidth * scale2;
                if (pwidth > 0 && p->getType() == RIVER && swidth > 0.1) {
                    float alpha = min(1.0f, swidth);
                    colorU->set(vec4f(color.xyz(), alpha));
                    drawCurve(tileOffset, p, pwidth, scale, fb, layerProgram, *mesh, &nx, &ny, &lx, &ly);
                }
            }
            fb->setBlend(false);
        } else {
            if (Logger::DEBUG_LOGGER != NULL) {
                ostringstream oss;
                oss << "NULL Graph : " << level << " " << tx << " " << ty;
                Logger::DEBUG_LOGGER->log("GRAPH", oss.str());
            }
        }
    }
    return true;
}

void WaterOrthoLayer::swap(ptr<WaterOrthoLayer> p)
{
    GraphLayer::swap(p);
    std::swap(color, p->color);
    std::swap(mesh, p->mesh);
    std::swap(tess, p->tess);
    std::swap(colorU, p->colorU);
    std::swap(tileOffsetU, p->tileOffsetU);
}

class WaterOrthoLayerResource : public ResourceTemplate<40, WaterOrthoLayer>
{
public:
    WaterOrthoLayerResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc,
            const TiXmlElement *e = NULL) :
        ResourceTemplate<40, WaterOrthoLayer> (manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        ptr<GraphProducer>graphProducer;
        int displayLevel = 0;
        vec4f color = vec4f((float)30/255,(float)62/255,(float)45/255, 1.0f);
        bool deform = false;

        checkParameters(desc, e, "name,graph,renderProg,level,color,quality,deform,");
        string g = getParameter(desc, e, "graph");

        graphProducer = manager->loadResource(g).cast<GraphProducer>();
        if (e->Attribute("level") != NULL) {
            getIntParameter(desc, e, "level", &displayLevel);
        }
        if (e->Attribute("quality") != NULL) {
            quality = strcmp(e->Attribute("quality"), "true") == 0;
        }
        if (e->Attribute("deform") != NULL) {
            deform = strcmp(e->Attribute("deform"), "true") == 0;
        }

        if (e->Attribute("color") != NULL) {
            string c = getParameter(desc, e, "color") + ",";
            string::size_type start = 0;
            string::size_type index;
            for (int i = 0; i < 3; i++) {
                index = c.find(',', start);
                color[i] = (float) atof(c.substr(start, index - start).c_str()) / 255;
                start = index + 1;
            }
        }

        ptr<Program> layerProgram = manager->loadResource(getParameter(desc, e, "renderProg")).cast<Program>();
        init(graphProducer, layerProgram, displayLevel, quality, color, deform);
    }

    virtual bool prepareUpdate()
    {
        bool changed = false;
        if (dynamic_cast<Resource*>(layerProgram.get())->changed()) {
            changed = true;
        }

        if (changed) {
            invalidateTiles();
        }
        oldValue = NULL;
        newDesc = NULL;

        return true;
    }
};

extern const char waterOrthoLayer[] = "waterOrthoLayer";

static ResourceFactory::Type<waterOrthoLayer, WaterOrthoLayerResource> WaterOrthoLayerType;

}
