#include "Wumbo-Hitbox.h"
#include <cstdio>
namespace Wumbo
{
	Hitbox::Hitbox()
	{
		x = y = left = top = width = height = 0;
	}
	Hitbox::Hitbox(int left, int top, int width, int height)
	{
		x = 0;
		y = 0;
		this->left = left;
		this->top = top;
		this->width = width;
		this->height = height;
	}
	bool Hitbox::hits(Hitbox other)
	{
		if (y+top > other.y+other.top+other.height-1)
			return false;
		if (x+left > other.x+other.left+other.width-1)
			return false;
		if (x+left+width-1 < other.x+other.left)
			return false;
		if (y+top+height-1 < other.y+other.top)
			return false;
		return true;
	}
}