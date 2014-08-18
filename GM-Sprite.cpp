//
//  GM-Sprite.cpp
//  Wumbo
//
//  Created by Wade McGillis on 11/14/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "GM-Sprite.h"
#include "GM-Texture.h"
#if PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#endif

namespace GM
{
	Wumbo::Sprite *__GM_SPRITES[1024];
	int __GM_SPRITES_TEXTURE[1024];
	int __GM_SPRITE_NEWEST = 0;
	
	Wumbo::Sprite *sprite_get_wumbo(int sprite)
	{
		if (sprite < 0 || sprite >= __GM_SPRITE_NEWEST)
			return NULL;
		return __GM_SPRITES[sprite];
	}
	
	int sprite_create_from_wumbo(Wumbo::Sprite *sprite)
	{
		__GM_SPRITES[__GM_SPRITE_NEWEST] = sprite;
		Wumbo::Texture *texpointer = sprite->getTexture();
		int texid = -1;
		for(int i=0;i<__GM_SPRITE_NEWEST;i++)
		{
			if (__GM_TEXTURES[__GM_SPRITES_TEXTURE[i]] == texpointer)
			{
				texid = __GM_SPRITES_TEXTURE[i];
				i = __GM_SPRITE_NEWEST;
			}
		}
		if (texid == -1)
			texid = texture_create_from_wumbo(texpointer);
		__GM_SPRITES_TEXTURE[__GM_SPRITE_NEWEST] = texid;
		__GM_SPRITE_NEWEST++;
		return __GM_SPRITE_NEWEST-1;
	}

	int sprite_add()
	{
		__GM_SPRITES[__GM_SPRITE_NEWEST] = new Wumbo::Sprite(__GM_WUMBO_RENDERER);
		__GM_SPRITES_TEXTURE[__GM_SPRITE_NEWEST] = -1;
		__GM_SPRITE_NEWEST++;
		return __GM_SPRITE_NEWEST-1;
	}

	int sprite_create_with_texture(int texture)
	{
		__GM_SPRITES[__GM_SPRITE_NEWEST] = new Wumbo::Sprite(__GM_WUMBO_RENDERER);
		int texID = texture;
		if (texture < 0)
			texID = -1;
		if (texture >= __GM_TEXTURE_NEWEST)
			texID = -1;
		if (__GM_TEXTURES[texture] == NULL)
			texID = -1;
		if (texID != -1)
			__GM_SPRITES[__GM_SPRITE_NEWEST]->setTexture(__GM_TEXTURES[texID]);
		__GM_SPRITES_TEXTURE[__GM_SPRITE_NEWEST] = texID;
		__GM_SPRITE_NEWEST++;
		return __GM_SPRITE_NEWEST-1;
	}
	
	int sprite_get_texture(int sprite)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		return __GM_SPRITES_TEXTURE[__GM_SPRITE_NEWEST];
	}
	
	void sprite_set_texture(int sprite, int texture)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		if (texture < 0)
			return;
		if (texture >= __GM_TEXTURE_NEWEST)
			return;
		if (__GM_TEXTURES[texture] == NULL)
			return;
		__GM_SPRITES[sprite]->setTexture(__GM_TEXTURES[texture]);
		__GM_SPRITES_TEXTURE[sprite] = texture;
	}
	
	int sprite_exists(int sprite)
	{
		if (sprite < 0)
			return false;
		if (sprite >= __GM_SPRITE_NEWEST)
			return false;
		if (__GM_SPRITES[sprite] == NULL)
			return false;
		return true;
	}
	
	void sprite_draw(int sprite, int subimage, int x, int y)
	{
		GLint t = -1;
		GLuint tt = 0;
		glGetIntegerv(GL_TEXTURE_2D, &t);
		if (__GM_SPRITES_TEXTURE[sprite] != -1)
		{
			//printf("getting\n",t,tt);
			tt = __GM_TEXTURES[__GM_SPRITES_TEXTURE[sprite]]->getTexture();
		}
		//printf("get: %i set: %i\n",t,tt);
		if (t != tt)
		{
			//printf("binding!\n");
			glBindTexture(GL_TEXTURE_2D, tt);
		}
		__GM_SPRITES[sprite]->setPosition(x, y);
		__GM_SPRITES[sprite]->draw();
	}

	int sprite_get_subimage_count(int sprite)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		return 0;
	}

	// modify the sprite
	void sprite_set_size(int sprite, int width, int height)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		__GM_SPRITES[sprite]->setWidth(width);
		__GM_SPRITES[sprite]->setHeight(height);
	}

	void sprite_set_width(int sprite, int width)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		__GM_SPRITES[sprite]->setWidth(width);
	}
	void sprite_set_height(int sprite, int height)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		__GM_SPRITES[sprite]->setHeight(height);
	}

	int sprite_get_width(int sprite)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		return __GM_SPRITES[sprite]->getWidth();
	}
	int sprite_get_height(int sprite)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		return __GM_SPRITES[sprite]->getHeight();
	}

	// sprite subimage
	void sprite_set_subimage(int sprite, int subimage, int u, int v, int width, int height)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		if (subimage < 0)
			return;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
			return;*/
		__GM_SPRITES[sprite]->setSubrectPixels(subimage, u, v, width, height);
	}

	void sprite_set_subimage_u(int sprite, int subimage, int u)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		if (subimage < 0)
			return;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return;*/
		Wumbo::Rectanglei rect;
		rect = __GM_SPRITES[sprite]->getSubrectPixels(subimage);
		rect.x = u;
		__GM_SPRITES[sprite]->setSubrectPixels(subimage,rect);
	}
	void sprite_set_subimage_v(int sprite, int subimage, int v)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		if (subimage < 0)
			return;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return;*/
		Wumbo::Rectanglei rect;
		rect = __GM_SPRITES[sprite]->getSubrectPixels(subimage);
		rect.y = v;
		__GM_SPRITES[sprite]->setSubrectPixels(subimage,rect);
	}
	void sprite_set_subimage_width(int sprite, int subimage, int width)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		if (subimage < 0)
			return;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return;*/
		Wumbo::Rectanglei rect;
		rect = __GM_SPRITES[sprite]->getSubrectPixels(subimage);
		rect.width = width;
		__GM_SPRITES[sprite]->setSubrectPixels(subimage,rect);
	}
	void sprite_set_subimage_height(int sprite, int subimage, int height)
	{
		if (sprite < 0)
			return;
		if (sprite >= __GM_SPRITE_NEWEST)
			return;
		if (__GM_SPRITES[sprite] == NULL)
			return;
		if (subimage < 0)
			return;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return;*/
		Wumbo::Rectanglei rect;
		rect = __GM_SPRITES[sprite]->getSubrectPixels(subimage);
		rect.height = height;
		__GM_SPRITES[sprite]->setSubrectPixels(subimage,rect);
	}

	int sprite_get_subimage_u(int sprite, int subimage)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		if (subimage < 0)
			return -1;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return -1;*/
		return __GM_SPRITES[sprite]->getSubrectPixels(subimage).x;
	}
	int sprite_get_subimage_v(int sprite, int subimage)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		if (subimage < 0)
			return -1;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return -1;*/
		return __GM_SPRITES[sprite]->getSubrectPixels(subimage).y;
	}
	int sprite_get_subimage_width(int sprite, int subimage)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		if (subimage < 0)
			return -1;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return -1;*/
		return __GM_SPRITES[sprite]->getSubrectPixels(subimage).width;
	}
	int sprite_get_subimage_height(int sprite, int subimage)
	{
		if (sprite < 0)
			return -1;
		if (sprite >= __GM_SPRITE_NEWEST)
			return -1;
		if (__GM_SPRITES[sprite] == NULL)
			return -1;
		if (subimage < 0)
			return -1;
		/*if (subimage >= __GM_SPRITES[sprite]->getSubimageCount())
		 return -1;*/
		return __GM_SPRITES[sprite]->getSubrectPixels(subimage).height;
	}
};