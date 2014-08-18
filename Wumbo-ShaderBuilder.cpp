//
//  Wumbo-Shader.cpp
//  Wumbo
//
//  Created by Wade McGillis on 11/23/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "Wumbo-ShaderBuilder.h"
#include "Wumbo-Functions.h"

namespace Wumbo
{
	ShaderBuilder::ShaderBuilder()
	{
		hasFragmentShader = false;
		hasVertexShader = false;
		vertPath = "";
		fragPath = "";
	}
	bool ShaderBuilder::loadVertexShader(std::string fname)
	{
		vertPath = fname;
		printf("VERTEX SHADER PATH IS %s\n",vertPath.c_str());
		FILE *f = fopen(vertPath.c_str(),"rb");
		if (f == NULL)
		{
			printf("Failed to load vertex shader: %s\n",vertPath.c_str());
			return false;
		}
		fclose(f);
		hasVertexShader = true;
		printf("VERTEX SHADER PATH IS %s\n",vertPath.c_str());
		//printf("\tFRAGMENT SHADER PATH IS %s\n",fragPath.c_str());
		return true;
	}
	bool ShaderBuilder::loadFragmentShader(std::string fname)
	{
		fragPath = fname;
		printf("FRAGMENT SHADER PATH IS %s\n",fragPath.c_str());
		FILE *f = fopen(fragPath.c_str(),"rb");
		if (f == NULL)
		{
			printf("Failed to load fragment shader: %s\n",fragPath.c_str());
			//fclose(f);
			return false;
		}
		fclose(f);
		hasFragmentShader = true;
		//printf("\tVERTEX SHADER PATH IS %s\n",vertPath.c_str());
		printf("FRAGMENT SHADER PATH IS %s\n",fragPath.c_str());
		return true;
	}

	Shader *ShaderBuilder::generate()
	{
		if (!Wumbo::OpenGL2())
		{
			printf("Shaders require OpenGL 2.0 or higher to run. Currently running %f\n",atof((char*)glGetString(GL_VERSION)));
			return NULL;
		}
		printf("LEMONY\n");
		GLuint vertShader = 0, fragShader = 0;
		printf("SNICKET\n");
		GLuint prog = glCreateProgram();
		printf("MUNCHIES\n");
		if (hasVertexShader)
		{
			printf("RIGHT BEFORE compileShader(), VERTEX SHADER PATH IS %s\n",vertPath.c_str());
			if (!compileShader(&vertShader,GL_VERTEX_SHADER,vertPath.c_str()))
			{
				printf("Vertex shader failed to compile.\n");
				return NULL;
			}
		}
		if (hasFragmentShader)
		{
			printf("RIGHT BEFORE compileShader(), FRAGMENT SHADER PATH IS %s\n",fragPath.c_str());
			if (!compileShader(&fragShader,GL_FRAGMENT_SHADER,fragPath.c_str()))
			{
				printf("Fragment shader failed to compile.\n");
				return NULL;
			}
		}
		glAttachShader(prog, vertShader);
		glAttachShader(prog, fragShader);
		for(int i=0;i<attribNames.size();i++)
			glBindAttribLocation(prog, attribIDs.at(i), attribNames.at(i));
		attribNames.clear();
		attribIDs.clear();
		
		if (!linkProgram(prog))
		{
			printf("Shaders failed to link.\n");
			glDeleteShader(vertShader);
			glDeleteShader(fragShader);
			glDeleteProgram(prog);
			return NULL;
		}
		// EVERYTHING OKAY SO FAR
		if (vertShader) {
			glDetachShader(prog, vertShader);
			glDeleteShader(vertShader);
		}
		if (fragShader) {
			glDetachShader(prog, fragShader);
			glDeleteShader(fragShader);
		}
		Shader *newshader = new Shader();
		
		newshader->builderSetProgram(prog);
		for(int i=0;i<uniforms.size();i++)
			newshader->builderAddUniform(uniforms.at(i));
		printf("Shaders are A-OK!\n");
		return newshader;
	}
	void ShaderBuilder::addAttrib(GLuint attribID, const GLchar *attribName)
	{
		//printf("VERTEX SHADER PATH IS %s\n",vertPath.c_str());
		//printf("FRAGMENT SHADER PATH IS %s\n",fragPath.c_str());
		attribIDs.push_back(attribID);
		attribNames.push_back(attribName);
	}
	void ShaderBuilder::addUniform(const GLchar *uniformName)
	{
		//printf("VERTEX SHADER PATH IS %s\n",vertPath.c_str());
		//printf("FRAGMENT SHADER PATH IS %s\n",fragPath.c_str());
		uniforms.push_back(uniformName);
	}
	bool ShaderBuilder::linkProgram(GLuint program)
	{
		GLint status;
		glLinkProgram(program);
		
#if 1//defined(DEBUG)
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar *log = (GLchar *)malloc(logLength);
			glGetProgramInfoLog(program, logLength, &logLength, log);
			printf("Program link log:\n%s", log);
			free(log);
		}
#endif
		
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == 0) {
			return false;
		}
		
		return true;
	}
	bool ShaderBuilder::compileShader(GLuint *shader, GLenum type, const char *path)
	{
		GLint status;
		char *contents;
		int len;
		GLchar *source;
		if (type == GL_VERTEX_SHADER)
			printf("VERTEX SHADER PATH INSIDE compileShader() IS %s\n",path);
		if (type == GL_FRAGMENT_SHADER)
			printf("FRAGMENT SHADER PATH INSIDE compileShader() IS %s\n",path);
		FILE *f = fopen(path,"rb");
		if (f == NULL)
		{
			printf("Failed to open file for reading. %s\n",path);
			return false;
		}
		fseek(f,0,SEEK_END);
		len = ftell(f);
		fseek(f,0,SEEK_SET);
		contents = new char[len+1];
		memset(contents,0,len+1);
		fread(contents, len, 1, f);
		fclose(f);
		source = (GLchar *)contents;
		if (!source) {
			delete source;
			return false;
		}

		/*printf("\n");
		if (type == GL_VERTEX_SHADER)
			printf("VERTEX SHADER CONTENTS: (%s)\n",path);
		if (type == GL_FRAGMENT_SHADER)
			printf("FRAGMENT SHADER CONTENTS: (%s)\n",path);
		printf("\n");
		printf("%s\n",contents);
		printf("\n");*/
		
		*shader = glCreateShader(type);
		glShaderSource(*shader, 1, (const GLchar**)&source, &len);
		glCompileShader(*shader);
		
#if 1//defined(DEBUG)
		GLint logLength;
		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar *log = (GLchar *)malloc(logLength);
			glGetShaderInfoLog(*shader, logLength, &logLength, log);
			printf("Shader compile log:\n%s", log);
			free(log);
		}
#endif
		
		glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
		if (status == 0) {
			glDeleteShader(*shader);
			delete source;
			return false;
		}
		delete source;
		return true;
	}
};


