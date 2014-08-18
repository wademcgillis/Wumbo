#ifndef __Wumbo_Xbox360_Win32_h__
#define __Wumbo_Xbox360_Win32_h__
#include "Wumbo-Header.h"
#ifndef DISABLE_XINPUT
#if PLATFORM_WINDOWS
#include "Wumbo-Xbox360Input.h"

namespace Wumbo
{
	class Xbox360Input_Win32 : public Xbox360Input
	{
	private:
		DWORD dwPacketNumber[MAX_CONTROLLERS];
		bool bConnected[MAX_CONTROLLERS];
		
		float fThumbLX[MAX_CONTROLLERS];
		float fThumbLY[MAX_CONTROLLERS];
		float fThumbRX[MAX_CONTROLLERS];
		float fThumbRY[MAX_CONTROLLERS];
		float fLeftTrigger[MAX_CONTROLLERS];
		float fRightTrigger[MAX_CONTROLLERS];
		WORD wButtons[MAX_CONTROLLERS];
		
		float fThumbLXPrevious[MAX_CONTROLLERS];
		float fThumbLYPrevious[MAX_CONTROLLERS];
		float fThumbRXPrevious[MAX_CONTROLLERS];
		float fThumbRYPrevious[MAX_CONTROLLERS];
		float fLeftTriggerPrevious[MAX_CONTROLLERS];
		float fRightTriggerPrevious[MAX_CONTROLLERS];
		WORD wButtonsPrevious[MAX_CONTROLLERS];
	public:
		Xbox360Input_Win32();
		
		/*Keyboard(InputManager *man);*/
		void initialize();
		void deinitialize();
		void update();
		void onEndOfFrame();
		
		bool isConnected(int controller);
		vec2df getLeftThumb(int controller);
		vec2df getRightThumb(int controller);
		float getLeftTrigger(int controller);
		float getRightTrigger(int controller);
		
		bool isButtonPressed(int controller, int id);
		bool isButtonReleased(int controller, int id);
		bool isButtonDown(int controller, int id);
		
	};
};
#endif // PLATFORM_WINDOWS
#endif
#endif