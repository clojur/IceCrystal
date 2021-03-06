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

#include <stdlib.h>

#include "Soul/core/FileLogger.h"
#include "Soul/render/FrameBuffer.h"
#include "Soul/resource/XMLResourceLoader.h"
#include "Soul/scenegraph/SceneManager.h"
#include "Soul/scenegraph/ShowLogTask.h"
#include "Soul/ui/GlfwWindow.h"

#include "LargescaleScene/plugin/TerrainPlugin.h"
#include "LargescaleScene/plugin/ForestPlugin.h"
#include "LargescaleScene/ui/BasicViewHandler.h"
#include "LargescaleScene/ui/twbar/TweakBarManager.h"
#include "LargescaleScene/util/TerrainViewController.h"

using namespace Soul;
using namespace LargescaleScene;

class HelloWorld : public GlfwWindow, public ViewManager
{
public:
	ptr<SceneManager> manager;
	ptr<TerrainViewController> controller;
	ptr<BasicViewHandler> view;
	ptr<EventHandler> ui;

	HelloWorld() : GlfwWindow(Window::Parameters().size(1024, 768))
	{
		FileLogger::File *out = new FileLogger::File("log.html");
		Logger::INFO_LOGGER = new FileLogger("INFO", out, Logger::INFO_LOGGER);
		Logger::WARNING_LOGGER = new FileLogger("WARNING", out, Logger::WARNING_LOGGER);
		Logger::ERROR_LOGGER = new FileLogger("ERROR", out, Logger::ERROR_LOGGER);

		ptr<XMLResourceLoader> resLoader = new XMLResourceLoader();
		resLoader->addPath("../../../Source/App/LargescaleSceneApp/examples/forest/trees1");
		resLoader->addArchive("../../../Source/App/LargescaleSceneApp/examples/forest/trees1/helloworld.xml");

		ptr<ResourceManager> resManager = new ResourceManager(resLoader, 8);

		manager = new SceneManager();
		manager->setResourceManager(resManager);

		manager->setScheduler(resManager->loadResource("defaultScheduler").cast<Scheduler>());
		manager->setRoot(resManager->loadResource("scene").cast<SceneNode>());
		manager->setCameraNode("camera");
		manager->setCameraMethod("draw");

		controller = new TerrainViewController(manager->getCameraNode(), 2500.0);
		view = new BasicViewHandler(true, this, NULL);

		ptr<TweakBarManager> tb = resManager->loadResource("ui").cast<TweakBarManager>();
		tb->setNext(view);
		ui = tb;
	}

	virtual ~HelloWorld()
	{
	}

	virtual void redisplay(double t, double dt)
	{
		ui->redisplay(t, dt);
		GlfwWindow::redisplay(t, dt);

		if (Logger::ERROR_LOGGER != NULL) {
			Logger::ERROR_LOGGER->flush();
		}
	}

	virtual void reshape(int x, int y)
	{
		ptr<FrameBuffer> fb = FrameBuffer::getDefault();
		fb->setDepthTest(true, LESS);
		fb->setViewport(vec4<GLint>(0, 0, x, y));
		ui->reshape(x, y);
		GlfwWindow::reshape(x, y);
		idle(false);
	}

	virtual void idle(bool damaged)
	{
		GlfwWindow::idle(damaged);
		if (damaged) {
			updateResources();
		}
		ui->idle(damaged);
	}

	virtual bool mouseClick(button b, state s, modifier m, int x, int y)
	{
		return ui->mouseClick(b, s, m, x, y);
	}

	virtual bool mouseMotion(int x, int y)
	{
		return ui->mouseMotion(x, y);
	}

	virtual bool mousePassiveMotion(int x, int y)
	{
		return ui->mousePassiveMotion(x, y);
	}

	virtual bool mouseWheel(wheel b, modifier m, int x, int y)
	{
		return ui->mouseWheel(b, m, x, y);
	}

	virtual bool keyTyped(unsigned char c, modifier m, int x, int y)
	{
		if (ui->keyTyped(c, m, x, y)) {
			return true;
		}
		if (c == 27) {
			::exit(0);
		}
		return false;
	}

	virtual bool keyReleased(unsigned char c, modifier m, int x, int y)
	{
		return ui->keyReleased(c, m, x, y);
	}

	virtual bool specialKey(key k, modifier m, int x, int y)
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

	virtual bool specialKeyReleased(key k, modifier m, int x, int y)
	{
		return ui->specialKeyReleased(k, m, x, y);
	}

	virtual ptr<SceneManager> getScene()
	{
		return manager;
	}

	virtual ptr<TerrainViewController> getViewController()
	{
		return controller;
	}

	virtual vec3d getWorldCoordinates(int x, int y)
	{
		vec3d p = manager->getWorldCoordinates(x, y);
		if (abs(p.x) > 100000.0 || abs(p.y) > 100000.0 || abs(p.z) > 100000.0) {
			p = vec3d(NAN, NAN, NAN);
		}
		return p;
	}

	void updateResources()
	{
		BasicViewHandler::Position p;
		view->getPosition(p);
		manager->getResourceManager()->updateResources();
		controller->setNode(manager->getCameraNode());
		view->setPosition(p);
	}

	static void exit() {
		app.cast<HelloWorld>()->manager->getResourceManager()->close();
		Object::exit();
	}

	static static_ptr<Window> app;
};

static_ptr<Window> HelloWorld::app;
//
//int main(int argc, char* argv[])
//{
//	initTerrainPlugin();
//	initForestPlugin();
//	atexit(HelloWorld::exit);
//	HelloWorld::app = new HelloWorld();
//	HelloWorld::app->start();
//	return 0;
//}
