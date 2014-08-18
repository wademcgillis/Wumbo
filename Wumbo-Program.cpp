#include "Wumbo-Program.h"
#include "Wumbo-Scene.h"
#if PLATFORM_IOS
#include "Wumbo-iOSRoot.h"
#endif

namespace Wumbo
{
	Program::Program()
	{
		scene = NULL;
		newScene = NULL;
		renderer = NULL;
		inputManager = NULL;
		assignedFrameRate = 30;
#if PLATFORM_PC
		sfmlwindow = NULL;
#endif
	}
	void Program::begin()
	{
		printf("Program::begin() -- This method must be overridden by a child class.\n");
	}
	void Program::end()
	{
		printf("Program::end() -- This method must be overridden by a child class.\n");
	}
	void Program::background()
	{
		printf("Program::background() -- This method only called on mobile.\n");
	}
	void Program::update()
	{
		if (sceneHasChanged)
		{
			printf("A new scene has been set\n");
			if (scene != NULL)
				scene->end();
			delete scene;
			scene = NULL;
			sceneHasChanged = false;
			if (newScene != NULL)
			{
				newScene->begin();
				//newScene->update();
				scene = newScene;
				printf("Old scene terminated. new scene has taken its place.\n");
			}
		}
		if (scene != NULL)
			scene->update();
		else
			printf("Program::scene == NULL. Something is wrong!");
	}
	void Program::render()
	{
		if (scene != NULL)
			scene->render();
	}
	Scene *Program::getScene()
	{
		return scene;
	}
	void Program::setScene(Scene *scn)
	{
		printf("setScene(%X)\n",(unsigned int)scn);
		if (scn != scene)
		{
			printf("New scene (%X) differs from old scene (%X) \n",(unsigned int)scn,(unsigned int)scene);
			newScene = scn;
			sceneHasChanged = true;
		}
	}
	Renderer *Program::getRenderer()
	{
		return renderer;
	}
	void Program::setRenderer(Renderer *rend)
	{
		renderer = rend;
	}
	InputManager *Program::getInputManager()
	{
		return inputManager;
	}
	void Program::setInputManager(InputManager *inputman)
	{
		inputManager = inputman;
	}

	void Program::setAssignedFrameRate(int num)
	{
		assignedFrameRate = num;
#if PLATFORM_PC
		sfmlwindow->setFramerateLimit(num);
#elif PLATFORM_IOS
		((GLKViewController*)glkcontroller).preferredFramesPerSecond = num;
#endif
	}
	int Program::getAssignedFrameRate()
	{
		return assignedFrameRate;
	}

#if PLATFORM_PC
	void Program::setSFMLWindow(sf::Window *window)
	{
		sfmlwindow = window;
		sfmlwindow->setFramerateLimit(assignedFrameRate);
	}

	sf::Window *Program::getSFMLWindow()
	{
		return sfmlwindow;
	}
#elif PLATFORM_IOS
	void Program::setGLKViewController(id controller)
	{
		glkcontroller = controller;
	}
	id Program::getGLKViewController()
	{
		return glkcontroller;
	}
	void Program::setAppDelegate(id delegate)
	{
		appdelegate = delegate;
	}
	id Program::getAppDelegate()
	{
		return appdelegate;
	}
#endif
	void Program::addSupportedOrientation(UIInterfaceOrientation orient)
	{
#if PLATFORM_IOS
		[((ViewController*) glkcontroller) addSupportedOrientation:orient];
#endif
	}
	void Program::removeSupportedOrientation(UIInterfaceOrientation orient)
	{
#if PLATFORM_IOS
		[((ViewController*) glkcontroller) removeSupportedOrientation:orient];
#endif
	}
};