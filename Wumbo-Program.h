#ifndef __Wumbo_Program_h__
#define __Wumbo_Program_h__
#include "Wumbo-Header.h"
#if PLATFORM_PC
#include <SFML/Window.hpp>
#elif PLATFORM_IOS
#include "Wumbo-iOSRoot.h"
#endif
#include "Wumbo-Platform_iOS.h"
namespace Wumbo
{
	class Scene;
	class Renderer;
	class InputManager;

	class Program
	{
	protected:
		Scene *scene;
		Scene *newScene;
		Renderer *renderer;
		InputManager *inputManager;
		bool sceneHasChanged;
		int assignedFrameRate;
#if PLATFORM_PC
		sf::Window *sfmlwindow;
#elif PLATFORM_IOS
		id glkcontroller;
		id appdelegate;
#endif
	public:
		Program();
		virtual void begin();
		virtual void update();
		virtual void render();
		virtual void end();
		virtual void background();
		Scene *getScene();
		void setScene(Scene *scn);
		Renderer *getRenderer();
		void setRenderer(Renderer *rend);
		InputManager *getInputManager();
		void setInputManager(InputManager *inputman);

		void setAssignedFrameRate(int num);
		int getAssignedFrameRate();

#if PLATFORM_PC
		void setSFMLWindow(sf::Window *window);
		sf::Window *getSFMLWindow();
#else
		void setGLKViewController(id controller);
		id getGLKViewController();
		void setAppDelegate(id delegate);
		id getAppDelegate();
#endif
		void addSupportedOrientation(UIInterfaceOrientation orient);
		void removeSupportedOrientation(UIInterfaceOrientation orient);
	};
};
#endif