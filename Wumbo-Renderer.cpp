#include "Wumbo-Header.h"
#if PLATFORM_WINDOWS
#include <GL/glew.h>
#include <GL/wglew.h>
#endif
#include "Wumbo-Renderer.h"
#include "Wumbo-Program.h"
#include "Wumbo-VirtualRenderTarget.h"
#include "Wumbo-RenderTarget.h"
#if PLATFORM_IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif

namespace Wumbo
{
	namespace GL
	{
		GLuint Wumbo_BindVertexArrayValue = 0;
		void BindVertexArray(GLuint array)
		{
			if (Wumbo_BindVertexArrayValue == array)
				return;
			//printf("Different Vertex Array\n");
			Wumbo_BindVertexArrayValue = array;
			glBindVertexArray(Wumbo_BindVertexArrayValue);
		}
	};
	
	bool OpenGL2()
	{
		static bool set = false;
		static bool GL2 = false;
		if (!set)
		{
			set = true;
#if PLATFORM_PC
			char* vers = (char*)glGetString(GL_VERSION);
			GL2 = (!(atof(vers) < 2.0f));
#else
			std::string vers = (char*)glGetString(GL_VERSION);
			NSLog(@"\n\n\n\n\n\n\n\n%s\n\n\n\n\n\n\n\n\n",vers.c_str());
			UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"OpenGL String"
															message:[NSString stringWithUTF8String:(const char*)glGetString(GL_VERSION)]
														   delegate:nil
												  cancelButtonTitle:@"OK"
												  otherButtonTitles:nil];
			[alert show];
			[alert release];
			GL2 = (vers.find("OpenGL ES 2.0") != std::string::npos);
#endif
		}
		return GL2;
	}

	extern void removeBind();
	bool Renderer::fullscreen = false;

	void Renderer::iOSFix()
	{
#if PLATFORM_IOS
		//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &iOSRenderBufferZero);
#endif
	}
	Renderer::Renderer(Program* prog, int GL)
	{
		GLversion = GL;
		program = prog;
		rotation = ScreenRotation_DOWN;
		renderTarget = NULL;
		fullscreen = false;
#if PLATFORM_IOS
		//glGetIntegerv(GL_FRAMEBUFFER_BINDING, &iOSRenderBufferZero);
#endif
	}
	Program *Renderer::getProgram()
	{
		return program;
	}
	int Renderer::getGLVersion()
	{
		return GLversion;
	}
	void Renderer::setFullscreen(bool full)
	{
		fullscreen = full;
	}
	bool Renderer::getFullscreen()
	{
		return fullscreen;
	}
	void Renderer::forceDisplaySize(int width, int height)
	{
		printf("set display size: %u %u\n",width,height);
#if PLATFORM_IOS
		width *= [UIScreen mainScreen].scale;
		height *= [UIScreen mainScreen].scale;
#endif
		displaySize = vec2di(width,height);
	}
	void Renderer::setDisplaySize(int width, int height)
	{
		printf("set display size: %u %u\n",width,height);
#if PLATFORM_IOS
		width *= [UIScreen mainScreen].scale;
		height *= [UIScreen mainScreen].scale;
#endif
		displaySize = vec2di(width,height);
#if PLATFORM_PC
		program->getSFMLWindow()->setSize(sf::Vector2u(width,height));
#endif
	}
#if PLATFORM_PC
	vec2di Renderer::getScreenSize()
	{
		vec2d<int> bob(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height);
		return bob;
	}
#endif
	vec2di Renderer::getDisplaySize()
	{
#if PLATFORM_IOS
		vec2di ret = displaySize;
		ret.x /= [UIScreen mainScreen].scale;
		ret.y /= [UIScreen mainScreen].scale;
		return ret;
#else
		return displaySize;
#endif
	}
	void Renderer::setTitle(std::string str)
	{
		title = str;
#if PLATFORM_PC
		program->getSFMLWindow()->setTitle(str);
#endif
	}
	void Renderer::setVirtualRenderTargetForced(VirtualRenderTarget targ)
	{
		virtualRenderTarget = targ;
		glScissor(virtualRenderTarget.x,virtualRenderTarget.y,virtualRenderTarget.width,virtualRenderTarget.height);
		glViewport(virtualRenderTarget.x,virtualRenderTarget.y,virtualRenderTarget.width,virtualRenderTarget.height);
	}
	void Renderer::setVirtualRenderTarget(VirtualRenderTarget targ)
	{
		//printf("Renderer::setVirtualRenderTarget(%i,%i,%i,%i)\n",targ.x,targ.y,targ.width,targ.height);
		virtualRenderTarget = targ;
		if (renderTarget != NULL)
		{
			//
			//printf("%u - (%i + %i) = %i\n",renderTarget->getHeight(),virtualRenderTarget.y,virtualRenderTarget.height,renderTarget->getHeight()-(virtualRenderTarget.y+virtualRenderTarget.height));
#if 1
			glScissor(virtualRenderTarget.x,renderTarget->getHeight()-(virtualRenderTarget.y+virtualRenderTarget.height),virtualRenderTarget.width,virtualRenderTarget.height);
			glViewport(virtualRenderTarget.x,renderTarget->getHeight()-(virtualRenderTarget.y+virtualRenderTarget.height),virtualRenderTarget.width,virtualRenderTarget.height);
#else
			glScissor(virtualRenderTarget.x,/*renderTarget->getHeight()-(*/virtualRenderTarget.y/*+virtualRenderTarget.height)*/,virtualRenderTarget.width,virtualRenderTarget.height);
			glViewport(virtualRenderTarget.x,/*renderTarget->getHeight()-(*/virtualRenderTarget.y/*+virtualRenderTarget.height)*/,virtualRenderTarget.width,virtualRenderTarget.height);
#endif
		}
		else
		{
#if PLATFORM_IOS
			virtualRenderTarget.x *= (int)[UIScreen mainScreen].scale;
			virtualRenderTarget.y *= (int)[UIScreen mainScreen].scale;
			virtualRenderTarget.width *= (int)[UIScreen mainScreen].scale;
			virtualRenderTarget.height *= (int)[UIScreen mainScreen].scale;
#endif
			glScissor(virtualRenderTarget.x,displaySize.y-(virtualRenderTarget.y+virtualRenderTarget.height),virtualRenderTarget.width,virtualRenderTarget.height);
			glViewport(virtualRenderTarget.x,displaySize.y-(virtualRenderTarget.y+virtualRenderTarget.height),virtualRenderTarget.width,virtualRenderTarget.height);
		}
	}
	void Renderer::resetVirtualRenderTarget()
	{
#if PLATFORM_IOS
		virtualRenderTarget = VirtualRenderTarget(0,0,displaySize.x,displaySize.y,displaySize.x/[UIScreen mainScreen].scale,displaySize.y/[UIScreen mainScreen].scale);
#else
		virtualRenderTarget = VirtualRenderTarget(0,0,displaySize.x,displaySize.y,displaySize.x,displaySize.y);
#endif
		glScissor(0,0,displaySize.x,displaySize.y);
		glViewport(0,0,virtualRenderTarget.width,virtualRenderTarget.height);
	}
	VirtualRenderTarget Renderer::getFullscreenRenderTarget()
	{
#if PLATFORM_PC
		return VirtualRenderTarget(0,0,sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height,sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height);
#elif PLATFORM_IOS
		return Wumbo::VirtualRenderTarget(0,0,0,0,0,0);
#endif
	}
	VirtualRenderTarget Renderer::getVirtualRenderTarget()
	{
#if PLATFORM_IOS
		VirtualRenderTarget ret = virtualRenderTarget;
		if (renderTarget == NULL)
		{
			ret.x /= (int)[UIScreen mainScreen].scale;
			ret.y /= (int)[UIScreen mainScreen].scale;
			ret.width /= (int)[UIScreen mainScreen].scale;
			ret.height /= (int)[UIScreen mainScreen].scale;
		}
		return ret;
#else
		return virtualRenderTarget;
#endif
	}

	void Renderer::setRenderTarget(RenderTarget *targ)
	{
		if (GLversion == 1)
		{
			printf("Renderer::setRenderTarget() is not available in OpenGL 1.x\n");
			return;
		}
		renderTarget = targ;
		targ->bind();
		//glScissor(0,0,renderTarget->getWidth(),renderTarget->getHeight());
		//glBindFrameBuffer(GL_FRAMEBUFFER, targ->getID());
	}
	void Renderer::resetRenderTarget()
	{
		if (GLversion == 1)
		{
			printf("Renderer::resetRenderTarget() is not available in OpenGL 1.x\n");
			return;
		}
		renderTarget = NULL;
		removeBind();
		//glScissor(0,0,displaySize.x,displaySize.y);
		//glBindFrameBuffer(GL_FRAMEBUFFER, 0);
	}
	RenderTarget *Renderer::getRenderTarget()
	{
		return renderTarget;
	}



	void Renderer::setRotation(ScreenRotation rot)
	{
		rotation = rot;
	}
	ScreenRotation Renderer::getRotation()
	{
		return rotation;
	}

	void Renderer::centerDisplayOnScreen()
	{
#if PLATFORM_PC
		program->getSFMLWindow()->setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/2-program->getSFMLWindow()->getSize().x/2,sf::VideoMode::getDesktopMode().height/2-program->getSFMLWindow()->getSize().y/2));
#endif
	}
};