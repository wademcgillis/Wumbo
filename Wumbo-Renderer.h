#ifndef __Wumbo_Renderer_h__
#define __Wumbo_Renderer_h__

#include "Wumbo-Types.h"
#include "Wumbo-VirtualRenderTarget.h"
#include "Wumbo-RenderTarget.h"
#include <string>
#if PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#else
#endif

#if PLATFORM_IOS
#define glBindVertexArray glBindVertexArrayOES
#elif PLATFORM_OSX
#define glBindVertexArray glBindVertexArrayAPPLE
#else
// Microsoft has it normal.
#endif

#if PLATFORM_IOS
#define glGenVertexArrays glGenVertexArraysOES
#elif PLATFORM_OSX
#define glGenVertexArrays glGenVertexArraysAPPLE
#else
// Microsoft has it normal.
#endif

#if PLATFORM_IOS
#define glDeleteVertexArrays glDeleteVertexArraysOES
#elif PLATFORM_OSX
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#else
// Microsoft has it normal.
#endif


namespace Wumbo
{
	namespace GL
	{
		extern void BindVertexArray(GLuint array);
	};
	
	
	typedef enum {
	WumboVertexAttribPosition,
	WumboVertexAttribNormal,
	WumboVertexAttribColor,
	WumboVertexAttribTexCoord0,
	WumboVertexAttribTexCoord1,
	} WumboVertexAttrib;

	typedef enum {ScreenRotation_DOWN, ScreenRotation_RIGHT, ScreenRotation_UP, ScreenRotation_LEFT} ScreenRotation;
//#endif
	
	class Program;

	class Renderer
	{
	private:
		static bool fullscreen;
		Program *program;
		vec2di displaySize;
		vec2di renderSize;
		std::string title;
		ScreenRotation rotation;
		VirtualRenderTarget virtualRenderTarget;
		RenderTarget *renderTarget;
		int GLversion;
	public:
#if PLATFORM_PC
		static vec2di getScreenSize();
#endif
		static bool getFullscreen();
		static void setFullscreen(bool full);
		void iOSFix();
		
		Renderer(Program* prog, int GL);
		Program *getProgram();

		int getGLVersion();
		
		void forceDisplaySize(int width, int height);
		void setDisplaySize(int width, int height);
		vec2di getDisplaySize();

		void resetVirtualRenderTarget();
		void setVirtualRenderTarget(VirtualRenderTarget targ);
		void setVirtualRenderTargetForced(VirtualRenderTarget targ);
		static VirtualRenderTarget getFullscreenRenderTarget();
		VirtualRenderTarget getVirtualRenderTarget();

		void resetRenderTarget();
		void setRenderTarget(RenderTarget *targ);
		RenderTarget *getRenderTarget();

		void setTitle(std::string);
		void setRotation(ScreenRotation rot);
		ScreenRotation getRotation();

		void centerDisplayOnScreen();
	};
};

#endif