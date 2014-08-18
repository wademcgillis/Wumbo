#include "Wumbo-Xbox360Input.h"

#if !PLATFORM_WINDOWS
#define DEFINE_XINPUT_BUTTONS 1
#else
#ifdef DISABLE_XINPUT
#define DEFINE_XINPUT_BUTTONS 1
#endif
#endif


#if DEFINE_XINPUT_BUTTONS
const int XINPUT_GAMEPAD_DPAD_UP = 0;
const int XINPUT_GAMEPAD_DPAD_DOWN = 0;
const int XINPUT_GAMEPAD_DPAD_LEFT = 0;
const int XINPUT_GAMEPAD_DPAD_RIGHT = 0;
const int XINPUT_GAMEPAD_START = 0;
const int XINPUT_GAMEPAD_BACK = 0;
const int XINPUT_GAMEPAD_LEFT_SHOULDER = 0;
const int XINPUT_GAMEPAD_RIGHT_SHOULDER = 0;
const int XINPUT_GAMEPAD_A = 0;
const int XINPUT_GAMEPAD_B = 0;
const int XINPUT_GAMEPAD_X = 0;
const int XINPUT_GAMEPAD_Y = 0;
#endif

namespace Wumbo
{
	const int Xbox360Input::DPAD_UP = XINPUT_GAMEPAD_DPAD_UP;
	const int Xbox360Input::DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN;
	const int Xbox360Input::DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT;
	const int Xbox360Input::DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT;
	const int Xbox360Input::START = XINPUT_GAMEPAD_START;
	const int Xbox360Input::BACK = XINPUT_GAMEPAD_BACK;
	const int Xbox360Input::LEFT = XINPUT_GAMEPAD_LEFT_SHOULDER;
	const int Xbox360Input::RIGHT = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	const int Xbox360Input::A = XINPUT_GAMEPAD_A;
	const int Xbox360Input::B = XINPUT_GAMEPAD_B;
	const int Xbox360Input::X = XINPUT_GAMEPAD_X;
	const int Xbox360Input::Y = XINPUT_GAMEPAD_Y;
	
	Xbox360Input::Xbox360Input()
	{
	}
	void Xbox360Input::initialize()
	{
		Wumbo::InputMethod::initialize();
	}
	void Xbox360Input::deinitialize()
	{
		Wumbo::InputMethod::deinitialize();
	}
	void Xbox360Input::update()
	{

	}
	void Xbox360Input::onEndOfFrame()
	{
		
	}
	bool Xbox360Input::isConnected(int controller)
	{
		return false;
	}
	vec2df Xbox360Input::getLeftThumb(int controller)
	{
		return vec2df(0,0);
	}
	vec2df Xbox360Input::getRightThumb(int controller)
	{
		return vec2df(0,0);
	}
	float Xbox360Input::getLeftTrigger(int controller)
	{
		return 0.f;
	}
	float Xbox360Input::getRightTrigger(int controller)
	{
		return 0.f;
	}
	bool Xbox360Input::isButtonPressed(int controller, int id)
	{
		return false;
	}
	bool Xbox360Input::isButtonReleased(int controller, int id)
	{
		return false;
	}
	bool Xbox360Input::isButtonDown(int controller, int id)
	{
		return false;
	}
}