#include "Wumbo-MouseInput.h"
#include "Wumbo-Types.h"
#include "Wumbo-Quicky.h"
namespace Wumbo
{
	void MouseInput::initialize()
	{
		//sfKeyToVirtualKey(sf::Keyboard::KeyCount);
		Wumbo::InputMethod::initialize();
		for(int i=0;i<3;i++)
			mousestateDown[i] = mousestatePressed[i] = mousestateReleased[i] = false;
	}
	void MouseInput::deinitialize()
	{
		Wumbo::InputMethod::deinitialize();
		for(int i=0;i<3;i++)
			mousestateDown[i] = mousestatePressed[i] = mousestateReleased[i] = false;
	}
	void MouseInput::update()
	{
		// lol. this method does nothing.
#if PLATFORM_PC
		mousex = sf::Mouse::getPosition(*Wumbo::__quickyProgram->getSFMLWindow()).x;
		mousey = sf::Mouse::getPosition(*Wumbo::__quickyProgram->getSFMLWindow()).y;
#endif
	}
	//
	void MouseInput::onEndOfFrame()
	{
		if (!initialized)
			return;
		for(int i=0;i<3;i++)
			mousestateReleased[i] = mousestatePressed[i] = false;
	}
	
	int MouseInput::x()
	{
		return mousex;
	}
	int MouseInput::y()
	{
		return mousey;
	}
	
	vec2di MouseInput::getPosition()
	{
		return vec2di(mousex,mousey);
	}

#if PLATFORM_PC
	void MouseInput::handleEvent(sf::Event event)
	{
		if (!initialized)
			return;
		if (event.type == sf::Event::MouseButtonPressed)
		{
			mousestateDown[event.mouseButton.button] = true;
			mousestatePressed[event.mouseButton.button] = true;
			mousex = event.mouseButton.x;
			mousey = event.mouseButton.y;
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			printf("release!\n");
			mousestateDown[event.mouseButton.button] = false;
			mousestateReleased[event.mouseButton.button] = true;
			mousex = event.mouseButton.x;
			mousey = event.mouseButton.y;
		}
		if (event.type == sf::Event::MouseMoved)
		{
			mousex = event.mouseMove.x;
			mousey = event.mouseMove.y;
		}
		//printf("MOUSE X: %i Y: %i \n",mousex,mousey);
	}
#endif
	
	bool MouseInput::isButtonPressed(unsigned int key)
	{
		if (!initialized)
			return false;
		if (key < 0 || key > 2)
			return false;
		return mousestatePressed[key];
	}
	bool MouseInput::isButtonReleased(unsigned int key)
	{
		if (!initialized)
			return false;
		if (key < 0 || key > 2)
			return false;
		return mousestateReleased[key];
	}
	bool MouseInput::isButtonDown(unsigned int key)
	{
		if (!initialized)
			return false;
		if (key < 0 || key > 2)
			return false;
		return mousestateDown[key];
	}
};