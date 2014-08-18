#ifndef __Wumbo_Texture_h__
#define __Wumbo_Texture_h__
#include "Wumbo-Header.h"
#include "Wumbo-Resource.h"

#include <string>
#if PLATFORM_IOS
#include <OpenGLES/ES1/gl.h>
#include <UIKit/UIKit.h>
#else
#if PLATFORM_WINDOWS
#include <GL/glew.h>
#include <GL/wglew.h>
#endif
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#endif

#if PLATFORM_APPLE
#import <Foundation/Foundation.h>
#endif

namespace Wumbo
{
	class Texture : public Resource
	{
	private:
		unsigned int *dataPtr;
		GLuint texture;
		int width;
		int height;
		GLuint apparentWidth;
		GLuint apparentHeight;
		int loadType;
		bool textureLoaded;
#if PLATFORM_OSX || PLATFORM_IOS
		NSString *filepath;
#else
		std::string filepath;
		sf::Uint8* filepointer;
		unsigned int filesize;
#endif	
	public:
		Texture();
		Texture(std::string fname);
		
		bool load(std::string fname);
#if PLATFORM_OSX || PLATFORM_IOS
		Texture(NSString *fname);
		bool load(NSString *fname);
#endif
#if PLATFORM_WINDOWS
		Texture(const sf::Uint8* pointer, unsigned int size);
		bool load(const sf::Uint8* pointer, unsigned int size);
#endif
		Texture(GLuint texID, unsigned int width, unsigned int height);
		bool loadLinearFromMemory(unsigned int *source, unsigned int w, unsigned int h);
		
		void setLinearInterpolation(bool linear);

		void reload();
		void unload();
		GLuint getTexture();
		GLuint getWidth();
		GLuint getHeight();
		
		unsigned int get(GLuint x, GLuint y);
		unsigned int getRGBPixel(GLuint x, GLuint y);
		unsigned int getBGRPixel(GLuint x, GLuint y);
		void setBGRPixel(GLuint x, GLuint y, unsigned int color);
		void setRGBPixel(GLuint x, GLuint y, unsigned int color);
		void update(unsigned int *source, unsigned int w, unsigned int h);

		void setApparentSize(GLuint w, GLuint h);
		
		void bind();
	};
};
#endif