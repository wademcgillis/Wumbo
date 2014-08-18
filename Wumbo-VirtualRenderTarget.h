#ifndef __Wumbo_VirtualRenderTarget_h__
#define __Wumbo_VirtualRenderTarget_h__

//#define VIRTUALRENDERTARGET_PRIVATE

namespace Wumbo
{
	class VirtualRenderTarget
	{
	#ifdef VIRTUALRENDERTARGET_PRIVATE
	private:
		int x;
		int y;
		int width;
		int height;
	#endif
	public:
	#ifndef VIRTUALRENDERTARGET_PRIVATE
		int x;
		int y;
		int width;
		int height;
	#endif
		int internalwidth;
		int internalheight;
		//unsigned int

		VirtualRenderTarget(int x, int y, int width, int height, int internalwidth, int internalheight);
		VirtualRenderTarget();
	#ifdef VIRTUALRENDERTARGET_PRIVATE
		void setX(int x);
		void setY(int y);
		int getX();
		int getY();
		void setWidth(int w);
		void setHeight(int h);
		int getWidth();
		int getHeight();
		void setInternalWidth(int w);
		void setInternalHeight(int h);
		int getInternalWidth();
		int getInternalHeight();
	#endif
	};
};
#endif