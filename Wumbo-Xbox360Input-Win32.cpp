#include "Wumbo-Xbox360Input-Win32.h"
#ifndef DISABLE_XINPUT
#if PLATFORM_WINDOWS
#include <cmath>
namespace Wumbo
{
	Xbox360Input_Win32::Xbox360Input_Win32()
	{
		printf("help\n");
	}
	void Xbox360Input_Win32::initialize()
	{
		Wumbo::InputMethod::initialize();
		for(int i=0;i<MAX_CONTROLLERS;i++)
		{
			fThumbLXPrevious[i] = fThumbLXPrevious[i] = 0;
			fThumbLYPrevious[i] = fThumbLY[i] = 0;
			fThumbRXPrevious[i] = fThumbRX[i] = 0;
			fThumbRYPrevious[i] = fThumbRY[i] = 0;
			fLeftTriggerPrevious[i] = fLeftTrigger[i] = 0;
			fRightTriggerPrevious[i] = fRightTrigger[i] = 0;
			wButtonsPrevious[i] = wButtons[i] = 0;
		}
	}
	void Xbox360Input_Win32::deinitialize()
	{
		Wumbo::InputMethod::deinitialize();
		for(int i=0;i<MAX_CONTROLLERS;i++)
		{
			fThumbLXPrevious[i] = fThumbLXPrevious[i] = 0;
			fThumbLYPrevious[i] = fThumbLY[i] = 0;
			fThumbRXPrevious[i] = fThumbRX[i] = 0;
			fThumbRYPrevious[i] = fThumbRY[i] = 0;
			fLeftTriggerPrevious[i] = fLeftTrigger[i] = 0;
			fRightTriggerPrevious[i] = fRightTrigger[i] = 0;
			wButtonsPrevious[i] = wButtons[i] = 0;
		}
	}
	void Xbox360Input_Win32::update()
	{
        DWORD dwResult;
        XINPUT_STATE state;
        for(int id=0;id<MAX_CONTROLLERS;id++)
        {
			ZeroMemory(&state,sizeof(XINPUT_STATE));
			dwResult = XInputGetState(id,&state);
			if (dwResult == ERROR_SUCCESS)
			{
				bConnected[id] = true;
				//printf("XBOX -- Controller %i connected\n",id);
				if (state.dwPacketNumber != dwPacketNumber[id])
				{
					fLeftTrigger[id] = (float) (state.Gamepad.bLeftTrigger-XINPUT_GAMEPAD_TRIGGER_THRESHOLD < 0 ? 0 : state.Gamepad.bLeftTrigger-XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (float) (255-XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
					fRightTrigger[id] = (float) (state.Gamepad.bRightTrigger-XINPUT_GAMEPAD_TRIGGER_THRESHOLD < 0 ? 0 : state.Gamepad.bRightTrigger-XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (float) (255-XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
					bool neg = false;
					SHORT thumbRawX, thumbRawY;
					SHORT fixedX, fixedY, deadzone;
					fixedX = -1000;
					fixedY = -1000;
					thumbRawX = state.Gamepad.sThumbLX;
					thumbRawY = state.Gamepad.sThumbLY;
					//printf("XBOX -- Controller %i raw left thumb values: %i %i\n",id,state.Gamepad.sThumbLX,state.Gamepad.sThumbLY);
					deadzone = 32767-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
					if (thumbRawX < 0)
						if (thumbRawX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
							fixedX = 0;
					if (thumbRawX > 0)
						if (thumbRawX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
							fixedX = 0;
					if (thumbRawX < 0 && fixedX == -1000)
						fixedX = thumbRawX+XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
					if (thumbRawX > 0 && fixedX == -1000)
						fixedX = thumbRawX-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

					if (thumbRawY < 0)
						if (thumbRawY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
							fixedY = 0;
					if (thumbRawY > 0)
						if (thumbRawY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
							fixedY = 0;
					if (thumbRawY < 0 && fixedY == -1000)
						fixedY = thumbRawY+XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
					if (thumbRawY > 0 && fixedY == -1000)
						fixedY = thumbRawY-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
					if (fixedX != -1000)
						fThumbLX[id] = abs((float)(fixedX)) / (float)(deadzone) * (thumbRawX < 0 ? -1 : 1);
					if (fixedY != -1000)
						fThumbLY[id] = abs((float)(fixedY)) / (float)(deadzone) * (thumbRawY < 0 ? 1 : -1);

					//printf("XBOX -- Controller %i fixed left thumb values: %f %f\n",id,fThumbLX[id],fThumbLY[id]);


					fixedX = -1000;
					fixedY = -1000;
					thumbRawX = state.Gamepad.sThumbRX;
					thumbRawY = state.Gamepad.sThumbRY;
					deadzone = 32767-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
					if (thumbRawX < 0)
						if (thumbRawX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
							fixedX = 0;
					if (thumbRawX > 0)
						if (thumbRawX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
							fixedX = 0;
					if (thumbRawX < 0 && fixedX == -1000)
						fixedX = thumbRawX+XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
					if (thumbRawX > 0 && fixedX == -1000)
						fixedX = thumbRawX-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

					if (thumbRawY < 0)
						if (thumbRawY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
							fixedY = 0;
					if (thumbRawY > 0)
						if (thumbRawY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
							fixedY = 0;
					if (thumbRawY < 0 && fixedY == -1000)
						fixedY = thumbRawY+XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
					if (thumbRawY > 0 && fixedY == -1000)
						fixedY = thumbRawY-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
					if (fixedX != -1000)
						fThumbRX[id] = abs((float)(fixedX)) / (float)(deadzone) * (thumbRawX < 0 ? -1 : 1);
					if (fixedY != -1000)
						fThumbRY[id] = abs((float)(fixedY)) / (float)(deadzone) * (thumbRawY < 0 ? 1 : -1);
					wButtons[id]  = state.Gamepad.wButtons;
				}
				dwPacketNumber[id] = state.dwPacketNumber;
			}
			else
				bConnected[id] = false;
        }
	}
	void Xbox360Input_Win32::onEndOfFrame()
	{
		//printf ("WUMBOOOOOOOOOO");
		for(int i=0;i<MAX_CONTROLLERS;i++)
		{
			fThumbLXPrevious[i] = fThumbLXPrevious[i];
			fThumbLYPrevious[i] = fThumbLY[i];
			fThumbRXPrevious[i] = fThumbRX[i];
			fThumbRYPrevious[i] = fThumbRY[i];
			fLeftTriggerPrevious[i] = fLeftTrigger[i];
			fRightTriggerPrevious[i] = fRightTrigger[i];
			wButtonsPrevious[i] = wButtons[i];
		}
	}
	bool Xbox360Input_Win32::isConnected(int controller)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return false;
		return bConnected[controller];
	}
	vec2df Xbox360Input_Win32::getLeftThumb(int controller)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return vec2df(0,0);
		//printf("Left Thumb: %f %f\n",fThumbLX[controller],fThumbLY[controller]);
		return vec2df(fThumbLX[controller],fThumbLY[controller]);
	}
	vec2df Xbox360Input_Win32::getRightThumb(int controller)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return vec2df(0,0);
		return vec2df(fThumbRX[controller],fThumbRY[controller]);
	}
	float Xbox360Input_Win32::getLeftTrigger(int controller)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return 0.f;
		return fLeftTrigger[controller];
	}
	float Xbox360Input_Win32::getRightTrigger(int controller)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return 0.f;
		return fRightTrigger[controller];
	}
	bool Xbox360Input_Win32::isButtonPressed(int controller, int id)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return false;
		return (wButtons[controller] & id) && !(wButtonsPrevious[controller] & id);
	}
	bool Xbox360Input_Win32::isButtonReleased(int controller, int id)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return false;
		return !(wButtons[controller] & id) && (wButtonsPrevious[controller] & id);
	}
	bool Xbox360Input_Win32::isButtonDown(int controller, int id)
	{
		if (controller < 0 || controller >= MAX_CONTROLLERS)
			return false;
		return (wButtons[controller] & id) > 0;
	}
}
#endif
#endif