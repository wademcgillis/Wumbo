//
//  Wumbo-Shader.h
//  Wumbo
//
//  Created by Wade McGillis on 11/23/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_Shader_h__
#define __Wumbo_Shader_h__
#include "Wumbo-Header.h"
#include <iostream>
#if PLATFORM_IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES2/gl.h>
#else
#if PLATFORM_WINDOWS
#include <GL/glew.h>
#include <GL/wglew.h>
#else
#include <SFML/OpenGL.hpp>
#endif
#endif

#include <map>

namespace Wumbo
{
	class Shader
	{
	private:
		static GLuint currentProgram;
		GLuint program;
		std::map<const GLchar*,GLint> uniforms;
	public:
		void builderSetProgram(GLuint prog);
		void builderAddUniform(const char *uniform);
		
		void makeActive();
		void setActive(bool active);
		bool isActive();
		
		GLuint getProgramID();
		GLint getUniformIDFromName(const GLchar* uniformID);
		
		//GLint getAttribLocation(const GLchar* attribName);
		
		void getUniform_f(const GLchar *uniformID, GLfloat *floats);
		void getUniform_i(const GLchar *uniformID, GLint *ints);
		void getUniform_f(GLint uniformID, GLfloat *floats);
		void getUniform_i(GLint uniformID, GLint *ints);
	};
};

#endif /* defined(__Wumbo__Wumbo_Shader__) */
