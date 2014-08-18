#ifndef __Wumbo_Hitbox_h__
#define __Wumbo_Hitbox_h__
namespace Wumbo
{
	class Hitbox
	{
	public:
		int x;
		int y;
		int left;
		int top;
		int width;
		int height;
		Hitbox();
		Hitbox(int left, int top, int width, int height);
		bool hits(Hitbox h);
	};
};
#endif