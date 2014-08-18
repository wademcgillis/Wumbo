//
//  Wumbo-Shader.h
//  Wumbo
//
//  Created by Wade McGillis on 11/23/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_ShaderBuilder_h__
#define __Wumbo_ShaderBuilder_h__
#include "Wumbo-Header.h"
#include <iostream>
#if PLATFORM_IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES2/gl.h>
#include <UIKit/UIKit.h>
#endif
#if PLATFORM_APPLE
#import <Foundation/Foundation.h>
#endif

#include <vector>

#include "Wumbo-Shader.h"

namespace Wumbo
{
	class ShaderBuilder
	{
	private:
		std::vector<const char*> attribNames;
		std::vector<GLuint> attribIDs;
		
		std::vector<const char*> uniforms;
		
		std::string vertPath;
		std::string fragPath;
		bool hasVertexShader;
		bool hasFragmentShader;
		
		bool compileShader(GLuint *shader, GLenum type, const char *path);
		bool linkProgram(GLuint program);
		
	public:
		ShaderBuilder();
		bool loadFragmentShader(std::string fname);
		bool loadVertexShader(std::string fname);
		void addAttrib(GLuint attribID, const GLchar *attribName);
		void addUniform(const GLchar *uniformName);
		Shader *generate();
	};
};

#endif /* defined(__Wumbo__Wumbo_Shader__) */
