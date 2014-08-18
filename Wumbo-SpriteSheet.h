#ifndef __Wumbo_SpriteSheet_h__
#define __Wumbo_SpriteSheet_h__

#include "Wumbo-Resource.h"
#include "Wumbo-Texture.h"
#include "Wumbo-Sprite.h"
#include "Wumbo-Types.h"

namespace Wumbo
{
	class SpriteSheet
	{
	private:
		Rectanglei rectangles[1024];
	public:
		Rectanglei get(unsigned int ID);
		void set(unsigned int ID, int x, int y, int width, int height);
		void set(unsigned int ID, Rectanglei rect);
	};
};
#endif