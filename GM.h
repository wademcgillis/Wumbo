//
//  GM.h
//  Wumbo
//
//  Created by Wade McGillis on 11/14/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __GM_h__
#define __GM_h__

#include "GM-Sprite.h"
#include "Wumbo-Program.h"
#include "Wumbo-InputManager.h"

namespace GM
{
	extern Wumbo::Program *__GM_WUMBO_PROGRAM;
	extern Wumbo::Renderer *__GM_WUMBO_RENDERER;
	extern Wumbo::InputManager *__GM_WUMBO_INPUTMANAGER;
	
	extern void gm_wumbo_init(Wumbo::Program *prog);

	extern float point_distance(float x1, float y1, float x2, float y2);
	
	extern unsigned int string_token_count(std::string arg, std::string token);
	
	extern std::string string_tokenize(std::string arg, std::string token, unsigned int number);
	
	inline unsigned int string_length(std::string str)
	{
		return str.length();
	}
	extern std::string string_char_at(std::string str, unsigned int index);
	extern std::string string_delete(std::string str, unsigned int index, unsigned int count);
	extern std::string string_lower(std::string str);
	extern std::string string_upper(std::string str);
	extern unsigned int string_count(std::string str, std::string substr);
};

#include "GM-Sprite.h"
#include "GM-Texture.h"
#include "GM-Ini.h"
#include "GM-Keyboard.h"

#endif /* defined(__Wumbo__GM__) */
