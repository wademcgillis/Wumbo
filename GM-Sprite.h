//
//  GM-Sprite.h
//  Wumbo
//
//  Created by Wade McGillis on 11/14/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __GM_Sprite_h__
#define __GM_Sprite_h__

#include "GM.h"
#include "Wumbo-Sprite.h"

namespace GM
{
	extern Wumbo::Sprite *__GM_SPRITES[1024];
	extern int __GM_SPRITES_TEXTURE[1024];
	extern int __GM_SPRITE_NEWEST;
	
	extern Wumbo::Sprite *sprite_get_wumbo(int sprite);
	extern int sprite_create_from_wumbo(Wumbo::Sprite *sprite);
	
	extern int sprite_add();
	
	extern int sprite_create_with_texture(int texture);
	extern int sprite_exists(int sprite);
	extern void sprite_draw(int sprite, int subimage, int x, int y);
	
	extern int sprite_get_texture(int sprite);
	
	extern int sprite_get_subimage_count(int sprite);
	
	// modify the sprite
	extern void sprite_set_size(int sprite, int width, int height);
	
	extern void sprite_set_width(int sprite, int width);
	
	extern void sprite_set_height(int sprite, int height);
	
	extern int sprite_get_width(int sprite);
	extern int sprite_get_height(int sprite);
	
	// sprite subimage
	extern void sprite_set_subimage(int sprite, int subimage, int u, int v, int width, int height);
	
	extern void sprite_set_subimage_u(int sprite, int subimage, int u);
	extern void sprite_set_subimage_v(int sprite, int subimage, int v);
	extern void sprite_set_subimage_width(int sprite, int subimage, int width);
	extern void sprite_set_subimage_height(int sprite, int subimage, int height);
	
	extern int sprite_get_subimage_u(int sprite, int subimage);
	extern int sprite_get_subimage_v(int sprite, int subimage);
	extern int sprite_get_subimage_width(int sprite, int subimage);
	extern int sprite_get_subimage_height(int sprite, int subimage);
};

#endif /* defined(__Wumbo__GM_Sprite__) */
