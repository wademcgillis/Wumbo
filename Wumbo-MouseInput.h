#ifndef __Wumbo_MouseInput_h__
#define __Wumbo_MouseInput_h__
#include "Wumbo-InputMethod.h"
#if PLATFORM_PC
#include <SFML/Window.hpp>
#else
namespace sf
{
	typedef unsigned int Event;
	namespace Keyboard
	{
		typedef unsigned char Key;
	};
};
#endif
#include "Wumbo-Types.h"

#include <string>

namespace Wumbo
{
	class MouseInput : public InputMethod
	{
	private:
		bool mousestateDown[3];
		bool mousestatePressed[3];
		bool mousestateReleased[3];
		int mousex, mousey;
	public:
		void initialize();
		void deinitialize();
		void update();
		void onEndOfFrame();
		//
#if PLATFORM_PC
		void handleEvent(sf::Event event);
#endif
		bool isButtonPressed(unsigned int button);
		bool isButtonReleased(unsigned int button);
		bool isButtonDown(unsigned int button);
		
		int x();
		int y();
		
		vec2di getPosition();
	};
};
#endif