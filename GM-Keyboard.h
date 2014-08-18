#ifndef __GM_Keyboard_h__
#define __GM_Keyboard_h__
#include "Wumbo-Platforms.h"
#include "Wumbo-KeyboardInput.h"
#if PLATFORM_PC
#include <SFML/System.hpp>
#else
// ??? iOS
#endif
namespace GM
{
	const unsigned int vk_return = Wumbo::Key::Return;
	const unsigned int vk_space = Wumbo::Key::Space;
	const unsigned int vk_backspace = Wumbo::Key::Backspace;
	const unsigned int vk_escape = Wumbo::Key::Escape;
	const unsigned int vk_anykey = -1;
#if PLATFORM_PC
	const unsigned int vk_left = Wumbo::Key::Left;
	const unsigned int vk_right = Wumbo::Key::Right;
	const unsigned int vk_up = Wumbo::Key::Down;
	const unsigned int vk_down = Wumbo::Key::Up;
	const unsigned int vk_a = Wumbo::Key::A;
	const unsigned int vk_k = Wumbo::Key::K;
#else
	const unsigned int vk_left = -1;
	const unsigned int vk_right = -1;
	const unsigned int vk_up = -1;
	const unsigned int vk_down = -1;
	const unsigned int vk_a = -1;
	const unsigned int vk_k = -1;
#endif
	bool keyboard_check(unsigned int key);
	bool keyboard_check_pressed(unsigned int key);
	bool keyboard_check_released(unsigned int key);
};
#endif