#ifndef __Wumbo_Types_h__
#define __Wumbo_Types_h__
#include <cstdio>
namespace Wumbo
{

	template <typename T>
	class grid
	{
	private:
		int width;
		int height;
		T *data;
	public:
		grid();
		grid(int w, int h);
		void resize(int w, int h);
		void set(int x, int y, T t);
		T get(int x, int y);
		void setRect(int x, int y, int ww, int hh, T t);
		void setRectOnlyWhere(int x, int y, int ww, int hh, T t, T o);
	};

	template <typename T>
	class vec2d
	{
	public:
		T x;
		T y;

		vec2d();

		vec2d(T x, T y);
	};
	typedef vec2d<int> vec2di;
	typedef vec2d<float> vec2df;

	template <typename T>
	class Rectangle
	{
	public:
		T x;
		T y;
		T width;
		T height;

		Rectangle();

		Rectangle(T x, T y, T width, T height);

		bool intersects(Rectangle R);
	};
	typedef Rectangle<int> Rectanglei;
	typedef Rectangle<float> Rectanglef;
};

#include "Wumbo-Types.inl"
#endif