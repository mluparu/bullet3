/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2015 Google Inc. http://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/



#include "../CommonInterfaces/CommonExampleInterface.h"
#include "../CommonInterfaces/CommonGUIHelperInterface.h"
#include "../Utils/b3Clock.h"




#include "../OpenGLWindow/SimpleOpenGL3App.h"
#include <stdio.h>
#include "../ExampleBrowser/OpenGLGuiHelper.h"

using namespace std::chrono_literals;

CommonExampleInterface*    example;
int gSharedMemoryKey=-1;

b3MouseMoveCallback prevMouseMoveCallback = 0;
static void OnMouseMove( float x, float y)
{
	bool handled = false; 
	handled = example->mouseMoveCallback(x,y); 	 
	if (!handled)
	{
		if (prevMouseMoveCallback)
			prevMouseMoveCallback (x,y);
	}
}

b3MouseButtonCallback prevMouseButtonCallback  = 0;
static void OnMouseDown(int button, int state, float x, float y) {
	bool handled = false;

	handled = example->mouseButtonCallback(button, state, x,y); 
	if (!handled)
	{
		if (prevMouseButtonCallback )
			prevMouseButtonCallback (button,state,x,y);
	}
}

class LessDummyGuiHelper : public DummyGUIHelper
{
	CommonGraphicsApp* m_app;
public:
	virtual CommonGraphicsApp* getAppInterface()
	{
		return m_app;
	}

	LessDummyGuiHelper(CommonGraphicsApp* app)
		:m_app(app)
	{
	}
};

constexpr int factorial(int x) {
	if (x > 1)
		return x * factorial(x - 1);
	else
		return x;
}

constexpr int SECONDS_TO_WAIT = factorial(3);
#ifndef IN_GTEST
constexpr int SECONDS_TO_EXIT = factorial(4);
#else
constexpr int SECONDS_TO_EXIT = factorial(2);
#endif

int runSimulation(int argc, char* argv[])
{
	SimpleOpenGL3App* app = new SimpleOpenGL3App("Bullet Standalone Example", 1024, 768, true);

	prevMouseButtonCallback = app->m_window->getMouseButtonCallback();
	prevMouseMoveCallback = app->m_window->getMouseMoveCallback();

	app->m_window->setMouseButtonCallback((b3MouseButtonCallback)OnMouseDown);
	app->m_window->setMouseMoveCallback((b3MouseMoveCallback)OnMouseMove);

	OpenGLGuiHelper gui(app, false);
	//LessDummyGuiHelper gui(app);
	//DummyGUIHelper gui;
	CommonExampleOptions options(&gui);


	example = StandaloneExampleCreateFunc(options);
	example->processCommandLineArgs(argc, argv);

	example->initPhysics();
	example->resetCamera();

	b3Clock stepClock, clock;
	btScalar dtElapsed;

	do
	{
		app->m_instancingRenderer->init();
		app->m_instancingRenderer->updateCamera(app->getUpAxis());

		btScalar dtSec = btScalar(stepClock.getTimeInSeconds());
		dtElapsed = btScalar(clock.getTimeInSeconds());

		if (dtSec<0.1)
			dtSec = 0.1;
		example->stepSimulation(dtSec);
		stepClock.reset();
		
		example->renderScene();
		if (dtElapsed > SECONDS_TO_WAIT)
			example->drawText3D("Hello!", 0, 1, 0, 2);

		DrawGridData dg;
		dg.upAxis = app->getUpAxis();
		app->drawGrid(dg);

		app->swapBuffer();
	} while (!app->m_window->requestedExit() && dtElapsed <= SECONDS_TO_EXIT);

	example->exitPhysics();
	delete example;
	delete app;

	dtElapsed = btScalar(clock.getTimeInSeconds());
	return static_cast<int>(dtElapsed);
}

#ifndef IN_GTEST
int main(int argc, char* argv[])
{
	return runSimulation(argc, argv);
}
#endif
