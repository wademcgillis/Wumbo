#ifndef __Wumbo_Xbox360_h__
#define __Wumbo_Xbox360_h__
#include "Wumbo-Header.h"

#include "Wumbo-InputMethod.h"
#include "Wumbo-Types.h"

#ifndef DISABLE_XINPUT
#if PLATFORM_WINDOWS
#include <Xinput.h>
#endif
#endif

#define MAX_CONTROLLERS 4

namespace Wumbo
{
	class Xbox360Input : public InputMethod
	{
	public:
		static const int DPAD_UP;
		static const int DPAD_DOWN;
		static const int DPAD_LEFT;
		static const int DPAD_RIGHT;
		static const int START;
		static const int BACK;
		static const int LEFT;
		static const int RIGHT;
		static const int A;
		static const int B;
		static const int X;
		static const int Y;
		Xbox360Input();
		
		/*Keyboard(InputManager *man);*/
		virtual void initialize();
		virtual void deinitialize();
		virtual void update();
		virtual void onEndOfFrame();
		
		virtual bool isConnected(int controller);
		virtual vec2df getLeftThumb(int controller);
		virtual vec2df getRightThumb(int controller);
		virtual float getLeftTrigger(int controller);
		virtual float getRightTrigger(int controller);
		
		virtual bool isButtonPressed(int controller, int id);
		virtual bool isButtonReleased(int controller, int id);
		virtual bool isButtonDown(int controller, int id);
		
	};
};
#ifndef DISABLE_XINPUT
#if PLATFORM_WINDOWS
#include "Wumbo-Xbox360Input-Win32.h"
#endif
#endif
#endif
