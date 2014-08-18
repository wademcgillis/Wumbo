#include "GM.h"

namespace GM
{
	bool keyboard_check(unsigned int key)
	{
		return __GM_WUMBO_INPUTMANAGER->getKeyboardState()->isKeyDown(key);
	}
	bool keyboard_check_pressed(unsigned int key)
	{
		return __GM_WUMBO_INPUTMANAGER->getKeyboardState()->isKeyPressed(key);
	}
	bool keyboard_check_released(unsigned int key)
	{
		return __GM_WUMBO_INPUTMANAGER->getKeyboardState()->isKeyReleased(key);
	}
};