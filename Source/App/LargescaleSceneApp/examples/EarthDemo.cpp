
#include <iostream>
#include <fstream>
#include <cfloat>

#include "tiffio.h"

#include "Soul/core/FileLogger.h"
#include "Soul/render/FrameBuffer.h"
#include "Soul/taskgraph/MultithreadScheduler.h"
#include "Soul/resource/XMLResourceLoader.h"
#include "Soul/resource/ResourceTemplate.h"
#include "Soul/scenegraph/ShowLogTask.h"
#include "Soul/ui/GlfwWindow.h"

#include "LargescaleScene/plugin/TerrainPlugin.h"
#include "LargescaleScene/Plugin/EditPlugin.h"
#include "LargescaleScene/Plugin/OceanPlugin.h"
#include "LargescaleScene/Plugin/ForestPlugin.h"
#include "LargescaleScene/terrain/TerrainNode.h"
#include "LargescaleScene/terrain/TileSampler.h"
#include "LargescaleScene/edit/EditElevationProducer.h"
#include "LargescaleScene/edit/EditOrthoProducer.h"
#include "LargescaleScene/preprocess/atmo/PreprocessAtmo.h"
#include "LargescaleScene/ui/BasicViewHandler.h"
#include "LargescaleScene/ui/EventRecorder.h"
#include "LargescaleScene/ui/twbar/TweakBarManager.h"
#include "LargescaleScene/util/PlanetViewController.h"

using namespace Soul;
using namespace LargescaleScene;

void resetSceneNode(ptr<SceneNode> node)
{
    SceneNode::FieldIterator i = node->getFields();
    while (i.hasNext()) {
        ptr<TileSampler> u = i.next().cast<TileSampler>();
        if (u != NULL) {
            ptr<TileProducer> p = u->get();
            ptr<EditElevationProducer> ep = p.cast<EditElevationProducer>();
            if (ep != NULL) {
                ep->reset();
            }
            ptr<EditOrthoProducer> eo = p.cast<EditOrthoProducer>();
            if (eo != NULL) {
                eo->reset();
            }
        }
    }
    for (unsigned int i = 0; i < node->getChildrenCount(); ++i) {
        resetSceneNode(node->getChild(i));
    }
}

static FileLogger::File *out(NULL);

class EarthDemo : public GlfwWindow, public Recordable, public ViewManager
{
public:
    EarthDemo() : GlfwWindow(Window::Parameters().size(1024, 768)), t(0.0), countDown(0)
    {
    }

    ~EarthDemo()
    {
        delete out;
    }

    void redisplay(double t, double dt)
    {
        if (getViewController()->getNode() != scene->getCameraNode()) {
            updateResources();
        }

        this->t = t;
        ui->redisplay(t, dt);
		GlfwWindow::redisplay(t, dt);

        if (countDown > 0) {
            countDown -= 1;
            if (countDown == 0) {
                ui->specialKey(KEY_F11, EventHandler::CTRL, 0, 0);
            }
        }

        if (Logger::ERROR_LOGGER != NULL) {
            Logger::ERROR_LOGGER->flush();
        }
    }

    void reshape(int x, int y)
    {
        ptr<FrameBuffer> fb = FrameBuffer::getDefault();
        fb->setViewport(vec4<GLint>(0, 0, x, y));
        fb->setPolygonMode(FILL, CULL);
        fb->setDepthTest(true, LESS);

        ui->reshape(x, y);
		GlfwWindow::reshape(x, y);
        idle(false);
    }

    void idle(bool damaged)
    {
        if (damaged) {
            updateResources();
        }
        ui->idle(damaged);
        GlfwWindow::idle(damaged);
    }

    bool mouseClick(button b, state s, modifier m, int x, int y)
    {
        return ui->mouseClick(b, s, m, x, y);
    }

    bool mouseMotion(int x, int y)
    {
        return ui->mouseMotion(x, y);
    }

    bool mousePassiveMotion(int x, int y)
    {
        return ui->mousePassiveMotion(x, y);
    }

    bool mouseWheel(wheel b, modifier m, int x, int y)
    {
        return ui->mouseWheel(b, m, x, y);
    }

    bool keyTyped(unsigned char c, modifier m, int x, int y)
    {
        if (ui->keyTyped(c, m, x, y)) {
            return true;
        }
        if (c == 27) {
            ::exit(0);
        } else if (c == 'i') {
            screenCapture(1);
            return true;
        } else if (c == 'I') {
            screenCapture(5);
            return true;
        }
        return false;
    }

    bool keyReleased(unsigned char c, modifier m, int x, int y)
    {
        return ui->keyReleased(c, m, x, y);
    }

    bool specialKey(key k, modifier m, int x, int y)
    {
        if (ui->specialKey(k, m, x, y)) {
            return true;
        }

        switch (k) {
        case KEY_F1:
            ShowLogTask::enabled = !ShowLogTask::enabled;
            return true;
        case KEY_F5:
            updateResources();
            return true;
        default:
            break;
        }
        return false;
    }

    bool specialKeyReleased(key k, modifier m, int x, int y)
    {
        return ui->specialKeyReleased(k, m, x, y);
    }

    ptr<SceneManager> getScene()
    {
        return scene;
    }

    ptr<TerrainViewController> getViewController()
    {
        if (controller == NULL) {
            if (radius == 0.0) {
                controller = new TerrainViewController(scene->getCameraNode(), 50000.0);
            } else {
                controller = new PlanetViewController(scene->getCameraNode(), radius);
            }
        }
        return controller;
    }

    vec3d getWorldCoordinates(int x, int y)
    {
        vec3d p = scene->getWorldCoordinates(x, y);
        if (controller.cast<PlanetViewController>() != NULL) {
            if (p.length() > controller.cast<PlanetViewController>()->R * dr) {
                p = vec3d(NAN, NAN, NAN);
            }
        } else if (abs(p.x) > 100000.0 || abs(p.y) > 100000.0 || abs(p.z) > 100000.0) {
            p = vec3d(NAN, NAN, NAN);
        }
        return p;
    }

    virtual void saveState()
    {
        view->getPosition(savedPosition);
    }

    virtual void restoreState()
    {
        view->jumpToPosition(savedPosition);
        resetSceneNode(scene->getRoot());
    }

    void updateResources()
    {
        BasicViewHandler::Position p;
        view->getPosition(p);
        scene->getResourceManager()->updateResources();
        getViewController()->setNode(scene->getCameraNode());
        view->setPosition(p);
    }

    void screenCapture(int zoom)
    {
        ptr<FrameBuffer> fb = FrameBuffer::getDefault();

        vec4<GLint> vp = fb->getViewport();
        int w = vp.z;
        int h = vp.w;
        char *buf = new char[w * h * 3];
        char *ibuf = new char[w * h * 3];

        char name[256];
        char stime[256];
        Timer::getDateTimeString(stime, 256);
        sprintf(name, "image.%s.tiff", stime);

        TIFF* f = TIFFOpen(name, "wb");
        TIFFSetField(f, TIFFTAG_IMAGEWIDTH, w * zoom);
        TIFFSetField(f, TIFFTAG_IMAGELENGTH, h * zoom);
        if (zoom > 1) {
            TIFFSetField(f, TIFFTAG_TILEWIDTH, w);
            TIFFSetField(f, TIFFTAG_TILELENGTH, h);
        }
        TIFFSetField(f, TIFFTAG_SAMPLESPERPIXEL, 3);
        TIFFSetField(f, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(f, TIFFTAG_COMPRESSION, COMPRESSION_DEFLATE);
        TIFFSetField(f, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(f, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(f, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

        if (zoom == 1) {
            fb->readPixels(0, 0, w, h, RGB, UNSIGNED_BYTE, Buffer::Parameters(), CPUBuffer(buf));
            for (int l = 0; l < h; ++l) {
                memcpy(ibuf + 3 * (h - 1 - l) * w, buf + 3 * l * w, 3 * w);
            }
            TIFFWriteEncodedStrip(f, 0, ibuf, w * h * 3);
        } else {
            for (int j = 0; j < zoom; ++j) {
                float bottom = -1.0f + j * 2.0f / zoom;
                float top = -1.0f + (j + 1) * 2.0f / zoom;
                for (int i = 0; i < zoom; ++i) {
                    float left = -1.0f + i * 2.0f / zoom;
                    float right = -1.0f + (i + 1) * 2.0f / zoom;
                    getViewController()->setProjection(0.0f, 0.0f, vec4f(left, right, bottom, top));
                    fb->clear(true, false, true);
                    scene->update(t, 0.0);
                    scene->draw();

                    fb->readPixels(0, 0, w, h, RGB, UNSIGNED_BYTE, Buffer::Parameters(), CPUBuffer(buf));
                    for (int l = 0; l < h; ++l) {
                        memcpy(ibuf + 3 * (h - 1 - l) * w, buf + 3 * l * w, 3 * w);
                    }
                    TIFFWriteTile(f, ibuf, i * w, (zoom - 1 - j) * h, 0, 0);

                    GlfwWindow::redisplay(0.0, 0.0);
                }
            }
        }

        TIFFClose(f);
        delete[] buf;
        delete[] ibuf;
    }

    void replay(const string &events)
    {
        ptr<EventRecorder> recorder = ui.cast<EventRecorder>();
        if (recorder != NULL) {
            recorder->setEventFile(events.c_str());
            countDown = 128;
        }
    }

protected:
    ptr<SceneManager> scene;

    ptr<EventHandler> ui;

    ptr<BasicViewHandler> view;

    ptr<TerrainViewController> controller;

    float radius;

    float dr;

    void swap(ptr<EarthDemo> o)
    {
        std::swap(scene, o->scene);
        std::swap(ui, o->ui);
        std::swap(view, o->view);
        std::swap(controller, o->controller);
        std::swap(radius, o->radius);
        std::swap(dr, o->dr);
        std::swap(savedPosition, o->savedPosition);
        std::swap(t, o->t);
        std::swap(countDown, o->countDown);
    }

private:
    BasicViewHandler::Position savedPosition;

    double t;

    int countDown; ///< number of frames before starting auto replay of events
};

class LargescaleSceneDemoResource : public ResourceTemplate<100, EarthDemo>
{
public:
    LargescaleSceneDemoResource(ptr<ResourceManager> manager, const string &name, ptr<ResourceDescriptor> desc, const TiXmlElement *e = NULL) :
        ResourceTemplate<100, EarthDemo> (manager, name, desc)
    {
        e = e == NULL ? desc->descriptor : e;
        checkParameters(desc, e, "name,ui,view,radius,dr,");

        scene = new SceneManager();
        scene->setResourceManager(manager);
        scene->setScheduler(manager->loadResource("defaultScheduler").cast<Scheduler>());
        scene->setRoot(manager->loadResource("scene").cast<SceneNode>());
        scene->setCameraNode("camera");
        scene->setCameraMethod("draw");
        ui = manager->loadResource(getParameter(desc, e, "ui")).cast<EventHandler>();
        view = manager->loadResource(getParameter(desc, e, "view")).cast<BasicViewHandler>();
        radius = 0.0f;
        dr = 1.1f;
        if (e->Attribute("radius") != NULL) {
            getFloatParameter(desc, e, "radius", &radius);
        }
        if (e->Attribute("dr") != NULL) {
            getFloatParameter(desc, e, "dr", &dr);
        }
    }

    virtual void doRelease()
    {
        if (manager != NULL) {
            manager->close();
        }
        delete this;
    }
};

extern const char LargescaleSceneDemo[] = "LargescaleSceneDemo";

static ResourceFactory::Type<LargescaleSceneDemo, LargescaleSceneDemoResource> LargescaleSceneDemoType;

static static_ptr<Window> app;

void initLargescaleSceneDemo(const string &archive, const string &data, const string &events)
{
    preprocessAtmo(AtmoParameters(), "textures/atmo");

    if (events.length() > 0) {
        char log[256];
        sprintf(log, "build/logtest-%s.html", events.c_str());
        out = new FileLogger::File(log);
        Logger::INFO_LOGGER = new FileLogger("INFO", out, NULL);
        Logger::WARNING_LOGGER = new FileLogger("WARNING", out, NULL);
        Logger::ERROR_LOGGER = new FileLogger("ERROR", out, NULL);
    } else {
        out = new FileLogger::File("log.html");
        Logger::INFO_LOGGER = new FileLogger("INFO", out, Logger::INFO_LOGGER);
        Logger::WARNING_LOGGER = new FileLogger("WARNING", out, Logger::WARNING_LOGGER);
        Logger::ERROR_LOGGER = new FileLogger("ERROR", out, Logger::ERROR_LOGGER);
    }

	ptr<XMLResourceLoader> resLoader = new XMLResourceLoader();
	if (archive[0] == '/') {
		resLoader->addArchive(archive);
		resLoader->addPath(archive.substr(0, archive.find_last_of('/')));
	}
	else {
		resLoader->addArchive("archives/" + archive);
	}
	resLoader->addPath("textures/atmo");
	resLoader->addPath("textures/clouds");
	resLoader->addPath("textures/rivers");
	resLoader->addPath("textures/roads");
	resLoader->addPath("textures/trees");
	resLoader->addPath("textures");
	resLoader->addPath("shaders/atmo");
	resLoader->addPath("shaders/camera");
	resLoader->addPath("shaders/clouds");
	resLoader->addPath("shaders/earth");
	resLoader->addPath("shaders/elevation");
	resLoader->addPath("shaders/ocean");
	resLoader->addPath("shaders/ortho");
	resLoader->addPath("shaders/rivers");
	resLoader->addPath("shaders/terrain");
	resLoader->addPath("shaders/trees");
	resLoader->addPath("shaders/util");
	resLoader->addPath("shaders");
	resLoader->addPath("meshes");
	resLoader->addPath("methods");
	resLoader->addPath("ui");
	resLoader->addPath(data);

	ptr<ResourceManager> resManager = new ResourceManager(resLoader, 8);


//    ptr<MultithreadScheduler> s = resManager->loadResource("defaultScheduler").cast<MultithreadScheduler>();
//    s->monitorTask("CreateCPUElevationTile");
//    s->monitorTask("CreateElevationTile");
//    s->monitorTask("CreateGraphTile");
//    s->monitorTask("CreateNormalTile");
//    s->monitorTask("CreateOrthoGPUTile");
//    s->monitorTask("CreateResidualTile");
//    s->monitorTask("DrawMesh");
//    s->monitorTask("DrawTerrain");
//    s->monitorTask("DrawTweakBar");
//    s->monitorTask("GetCurveDatasTask");
//    s->monitorTask("SetProgram");
//    s->monitorTask("SetState");
//    s->monitorTask("SetTransforms");
//    s->monitorTask("ShowInfo");

    app = resManager->loadResource("window").cast<Window>();
    if (events.length() > 0) {
        app.cast<EarthDemo>()->replay(events);
    }
}

int main(int argc, char *argv[])
{
    initTerrainPlugin();
    initEditPlugin();
    initOceanPlugin();
    initForestPlugin();

    atexit(Object::exit);
    if (argc == 3) {
        initLargescaleSceneDemo(argv[1], argv[2], "");
    } else {
        initLargescaleSceneDemo(argv[1], argv[2], argv[3]);
    }
	//initLargescaleSceneDemo("", "F:/EngineProject/proland-4.0/data", "");
    app->start();
    return 0;

	//assert(argc > 2);
	//initTerrainPlugin();
	//initEditPlugin();
	//initOceanPlugin();
	//initForestPlugin();

	//atexit(Object::exit);
	//if (argc == 3) {
 //       initLargescaleSceneDemo(argv[1], argv[2], "");
	//}
	//else {
 //       initLargescaleSceneDemo(argv[1], argv[2], argv[3]);
	//}
	//app->start();
	//return 0;
}
