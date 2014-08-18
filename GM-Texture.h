//
//  GM-Texture.h
//  Wumbo
//
//  Created by Wade McGillis on 11/14/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __GM_Texture_h__
#define __GM_Texture_h__

#include "GM.h"
#include "Wumbo-Texture.h"

namespace GM
{
	extern Wumbo::Texture *__GM_TEXTURES[1024];
	extern int __GM_TEXTURE_NEWEST;
	
	extern Wumbo::Texture *texture_get_wumbo(int texture);
	extern int texture_create_from_wumbo(Wumbo::Texture *texture);
	
	extern int texture_add();
	
#if PLATFORM_APPLE
	extern int texture_load(NSString *fname);
#else
	extern int texture_load(char *fname);
#endif
	extern int texture_exists(int texture);
	
	// modify the texture
	extern int texture_get_width(int texture);
	extern int texture_get_height(int texture);
};

#endif /* defined(__Wumbo__GM_Texture__) */
