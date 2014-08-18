#include "Wumbo-Types.h"

namespace Wumbo
{


	template <typename T>
	inline grid<T>::grid()
	{
		width = 1;
		height = 1;
		data = new T[width*height];
	}
	template <typename T>
	inline grid<T>::grid(int w, int h)
	{
		width = w;
		height = h;
		data = new T[w*h];
	}
	template <typename T>
	inline void grid<T>::resize(int w, int h)
	{
		T *olddata = data;
		data = new T[w*h];
		int ww = w > width ? width : w;
		int hh = h > height ? height : h;
		for(int i=0;i<ww;i++)
			for(int j=0;j<hh;j++)
				data[w*j+i] = olddata[width*j+i];
		width = w;
		height = h;
		delete olddata;
	}
	template <typename T>
	inline void grid<T>::set(int x, int y, T t)
	{
		//printf("try to set %i %i %i\n",x,y,(int)t);
		//printf("is (x >= 0)\n");
		if (x >= 0)
		{
			//printf("is (y >= 0)\n");
			if (y >= 0)
			{
				//printf("is (%i < %i)\n",x,width);
				if (x < width)
				{
					//printf("is (%i < %i)\n",y,height);
					if (y < height)
					{
						//printf("set %i %i %i\n",x,y,(int)t);
						data[width*y+x] = t;
					}
				}
			}
		}
	}
	template <typename T>
	inline T grid<T>::get(int x, int y)
	{
		if (x >= 0)
			if (y >= 0)
				if (x < width)
					if (y < height)
						return data[width*y+x];
		return data[0];
	}


	template <typename T>
	inline void grid<T>::setRect(int x, int y, int ww, int hh, T t)
	{
		if (ww < 0)
		{
			x += ww;
			ww = -ww;
		}
		if (hh < 0)
		{
			y += hh;
			hh = -hh;
		}
		bool setted = false;
		if (x >= 0)
			if (y >= 0)
				if (x < width)
					if (y < height)
						if (x+ww <= width)
							if (y+hh <= height)
								for(int i=x;i<x+ww;i++)
									for(int j=y;j<y+hh;j++)
									{
										setted = true;
										data[width*j+i] = t;
									}
		if (setted)
			printf("Set rect: %i %i %i %i\n",x,y,ww,hh);
	}

	template <typename T>
	inline void grid<T>::setRectOnlyWhere(int x, int y, int ww, int hh, T t, T o)
	{
		if (ww < 0)
		{
			x += ww;
			ww = -ww;
		}
		if (hh < 0)
		{
			y += hh;
			hh = -hh;
		}
		bool setted = false;
		if (x >= 0)
			if (y >= 0)
				if (x < width)
					if (y < height)
						if (x+ww <= width)
							if (y+hh <= height)
								for(int i=x;i<x+ww;i++)
									for(int j=y;j<y+hh;j++)
									{
										if (data[width*j+i] == o)
										{
											setted = true;
											data[width*j+i] = t;
										}
									}
		if (setted)
			printf("Set rect: %i %i %i %i\n",x,y,ww,hh);
	}


	template <typename T>
	inline vec2d<T>::vec2d() : x(0), y(0)
	{

	}

	template <typename T>
	inline vec2d<T>::vec2d(T X, T Y) : x(X), y(Y)
	{

	}

	template <typename T>
	inline Rectangle<T>::Rectangle() : x(0), y(0), width(0), height(0)
	{

	}

	template <typename T>
	inline Rectangle<T>::Rectangle(T X, T Y, T W, T H) : x(X), y(Y), width(W), height(H)
	{

	}

	template <typename T>
	inline bool Rectangle<T>::intersects(Rectangle<T> R)
	{
		if (y > R.y+R.height)
			return false;
		if (x > R.x+R.width)
			return false;
		if (x+width < R.x)
			return false;
		if (y+height < R.y)
			return false;
		return true;
	}
};