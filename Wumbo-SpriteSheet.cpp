#include "Wumbo-SpriteSheet.h"

namespace Wumbo
{
	Rectanglei SpriteSheet::get(unsigned int ID)
	{
		if (ID > 1023)
			return Rectanglei(0,0,1,1);
		return rectangles[ID];
	}
	void SpriteSheet::set(unsigned int ID, int x, int y, int width, int height)
	{
		if (ID > 1023)
			return;
		rectangles[ID] = Rectanglei(x,y,width,height);
	}
	void SpriteSheet::set(unsigned int ID, Rectanglei rect)
	{
		if (ID > 1023)
			return;
		rectangles[ID] = rect;
	}
}