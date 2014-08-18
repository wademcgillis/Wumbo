//
//  GM-Texture.cpp
//  Wumbo
//
//  Created by Wade McGillis on 11/14/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "GM-Texture.h"
#include "Wumbo-Functions.h"
namespace GM
{
	Wumbo::Texture *__GM_TEXTURES[1024];
	int __GM_TEXTURE_NEWEST = 0;
	
	Wumbo::Texture *texture_get_wumbo(int texture)
	{
		if (texture < 0)
			return NULL;
		if (texture >= __GM_TEXTURE_NEWEST)
			return NULL;
		//if (__GM_TEXTURES[texture] == NULL)
		//	return NULL;
		return __GM_TEXTURES[texture];
	}
	int texture_create_from_wumbo(Wumbo::Texture *texture)
	{
		__GM_TEXTURES[__GM_TEXTURE_NEWEST] = texture;
		__GM_TEXTURE_NEWEST++;
		return __GM_TEXTURE_NEWEST-1;
	}
	
	int texture_add()
	{
		__GM_TEXTURES[__GM_TEXTURE_NEWEST] = new Wumbo::Texture();
		__GM_TEXTURE_NEWEST++;
		return __GM_TEXTURE_NEWEST-1;
	}
	
#if PLATFORM_APPLE
	int texture_load(NSString *fname)
#else
	int texture_load(char *fname)
#endif
	{
		__GM_TEXTURES[__GM_TEXTURE_NEWEST] = new Wumbo::Texture();
#if PLATFORM_APPLE
		__GM_TEXTURES[__GM_TEXTURE_NEWEST]->load(fname);
#else
		__GM_TEXTURES[__GM_TEXTURE_NEWEST]->load(fname);
#endif
		__GM_TEXTURE_NEWEST++;
		return __GM_TEXTURE_NEWEST-1;
	}
	
	int texture_exists(int texture)
	{
		if (texture < 0)
			return false;
		if (texture >= __GM_TEXTURE_NEWEST)
			return false;
		if (__GM_TEXTURES[texture] == NULL)
			return false;
		return true;
	}
	
	// modify the texture
	int texture_get_width(int texture)
	{
		if (texture < 0)
			return -1;
		if (texture >= __GM_TEXTURE_NEWEST)
			return -1;
		if (__GM_TEXTURES[texture] == NULL)
			return -1;
		return __GM_TEXTURES[texture]->getWidth();
	}
	int texture_get_height(int texture)
	{
		if (texture < 0)
			return -1;
		if (texture >= __GM_TEXTURE_NEWEST)
			return -1;
		if (__GM_TEXTURES[texture] == NULL)
			return -1;
		return __GM_TEXTURES[texture]->getHeight();
		
	}
};