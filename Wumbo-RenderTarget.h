#ifndef __Wumbo_RenderTarget_h__
#define __Wumbo_RenderTarget_h__
#include "Wumbo-Texture.h"

#if PLATFORM_WINDOWS
#include <GL/glew.h>
#include <GL/wglew.h>
#elif PLATFORM_IOS
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#elif PLATFORM_APPLE
#import <OpenGL/gl.h>
#endif
namespace Wumbo
{
	class Renderer;

	class RenderTarget
	{
	private:
		GLuint renderID;
		GLuint textureID;
		GLuint frameID;
		Texture *texture;
		bool initialized;
		unsigned int width;
		unsigned int height;
		bool GL2;
	public:
		RenderTarget(Renderer* rend, unsigned int width, unsigned int height);
		RenderTarget();
		void initialize(Renderer* rend, unsigned int width, unsigned int height);
		Texture *getTexture();

		unsigned int getWidth();
		unsigned int getHeight();

		void bind();
	};
};
#endif