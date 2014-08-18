#ifndef __Wumbo_Header_h__
#define __Wumbo_Header_h__

#define DEBUGMSG(msg)	printf("DEBUG %s:%i -- %s\n",__FILE__,__LINE__,msg);
#define FUNCMSG(msg)	printf("FUNC  %s:%i -- %s\n",__FILE__,__LINE__,msg);
#define FATALMSG(msg)	printf("FATAL %s:%i -- %s\n",__FILE__,__LINE__,msg);

// Individual Apple Platform Preprocessor Defines
#define PLATFORM_IOS (defined (__IPHONE_OS_VERSION_MIN_REQUIRED))
#define PLATFORM_OSX (defined (__APPLE__) && !PLATFORM_IOS)
// Apple Platform Preprocessor Define
#define PLATFORM_APPLE (defined (__APPLE__))
// Individual Apple Platform Preprocessor Defines
#define PLATFORM_WINDOWS (defined (_WIN32))
// Microsoft Platform Preprocessor Define
#define PLATFORM_MICROSOFT (defined (_WIN32))
// Personal Computer Preprocessor Define
#define PLATFORM_PC !PLATFORM_IOS

#if !PLATFORM_APPLE
typedef int GLint;
typedef char GLchar;
#endif
#if PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if PLATFORM_APPLE
#include <Foundation/Foundation.h>
#endif

#include <stdio.h>
#include <string>

extern void TFDEBUG(std::string);

namespace Wumbo
{
	extern bool OpenGL2();
};

namespace std
{
	template<typename T>
	class Xvector
	{
	private:
		T* data;
		unsigned int _size;
	public:
		Xvector()
		{
			data = (T*)malloc(sizeof(T)*4096);
			_size = 0;
		}
		unsigned int begin()
		{
			return 0;
		}
		void erase(unsigned int position)
		{
			_size--;
			for(unsigned int i=position;i<_size;i++)
				data[i] = data[i+1];
		}
		void push_back(T element)
		{
			data[_size] = element;
			_size++;
		}
		T at(unsigned int position)
		{
			if (position < _size)
				return data[position];
		}
		unsigned int size()
		{
			return _size;
		}
		T operator[](unsigned int position)
		{
			if (position < _size)
				return data[position];
		}
		void clear()
		{
			_size = 0;
		}
		T back()
		{
			if (_size > 0)
				return data[_size-1];
		}
		void pop_back()
		{
			if (_size > 0)
				_size--;
		}
		void insert(unsigned int position, T element)
		{
			if (position > _size)
			{
				position = _size;
				_size++;
			}
			for(unsigned int i=_size;i>position;i--)
				data[i] = data[i-1];
			data[position] = element;
		}
		~Xvector()
		{
			//dealloc(data);
		}
	};
};


#endif