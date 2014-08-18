#include "Wumbo-VirtualRenderTarget.h"
namespace Wumbo
{
	VirtualRenderTarget::VirtualRenderTarget()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
		internalwidth = 0;
		internalheight = 0;
	}

	VirtualRenderTarget::VirtualRenderTarget(int _x, int _y, int _width, int _height, int _internalwidth, int _internalheight)
	{
		x = _x;
		y = _y;
		width = _width;
		height = _height;
		internalwidth = _internalwidth;
		internalheight = _internalheight;
	}
	#ifdef VIRTUALRENDERTARGET_PRIVATE
	void VirtualRenderTarget::setX(int _x)
	{
		x = _x;
	}
	void VirtualRenderTarget::setY(int _y)
	{
		y = _y;
	}
	int VirtualRenderTarget::getX()
	{
		return x;
	}
	int VirtualRenderTarget::getY()
	{
		return y;
	}
	void VirtualRenderTarget::setWidth(int w)
	{
		width = w;
	}
	void VirtualRenderTarget::setHeight(int h)
	{
		height = h;
	}
	int VirtualRenderTarget::getWidth()
	{
		return width;
	}
	int VirtualRenderTarget::getHeight()
	{
		return height;
	}
	void VirtualRenderTarget::setInternalWidth(int w)
	{
		internalwidth = w;
	}
	void VirtualRenderTarget::setInternalHeight(int h)
	{
		internalheight = h;
	}
	int VirtualRenderTarget::getInternalWidth()
	{
		return internalwidth;
	}
	int VirtualRenderTarget::getInternalHeight()
	{
		return internalheight;
	}
#endif
};