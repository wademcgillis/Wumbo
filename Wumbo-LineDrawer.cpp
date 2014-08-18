//
//  Wumbo-LineDrawer.cpp
//  fjords_ios
//
//  Created by Wade McGillis on 4/14/14.
//  Copyright (c) 2014 Wade McGillis. All rights reserved.
//

#include "Wumbo-LineDrawer.h"
#include "Wumbo-Shader.h"
#include "Wumbo-Sprite.h"
#include <cmath>
extern Wumbo::Shader *circleShader;

#if PLATFORM_WINDOWS
#define M_PI 3.1415926535f
#endif

void CHECK_GL_ERROR(const char *str)
{
	printf("GLERRORCHECK: %s\n",str);
	GLenum err = glGetError();
	if (err == GL_NO_ERROR)
		printf("NO OPENGL ERROR\n");
	else
	{
		if (err == GL_INVALID_ENUM)
			printf("OpenGL Error = GL_INVALID_ENUM\n");
		if (err == GL_INVALID_VALUE)
			printf("OpenGL Error = GL_INVALID_VALUE\n");
		if (err == GL_INVALID_OPERATION)
			printf("OpenGL Error = GL_INVALID_OPERATION\n");
		if (err == GL_INVALID_FRAMEBUFFER_OPERATION)
			printf("OpenGL Error = GL_INVALID_FRAMEBUFFER_OPERATION\n");
		if (err == GL_OUT_OF_MEMORY)
			printf("OpenGL Error = GL_OUT_OF_MEMORY\n");
		if (err == GL_STACK_UNDERFLOW)
			printf("OpenGL Error = GL_STACK_UNDERFLOW\n");
		if (err == GL_STACK_OVERFLOW)
			printf("OpenGL Error = GL_STACK_OVERFLOW\n");
	}
}




namespace Wumbo
{

	LineDrawer::LineDrawer(Wumbo::Renderer* rend, Wumbo::Shader *shader)
	{
		firsttime = true;
		clear();
		renderer = rend;
		matrixUniformID = shader->getUniformIDFromName("supermatrix");
	}
	LineDrawer::~LineDrawer()
	{
		glDeleteBuffers(1,&vertexBuffer);
#if PLATFORM_IOS
		glDeleteVertexArraysOES(1,&vertexArray);
#elif PLATFORM_OSX
		glDeleteVertexArraysAPPLE(1,&vertexArray);
#else
		glDeleteVertexArrays(1,&vertexArray);
#endif

	}
	void LineDrawer::clear()
	{
		vertexCount = 0;
		bufferOffset = 0;
		indexCount = 0;
	}
	void LineDrawer::addCircle(float x, float y, float r, unsigned int res)
	{
		const float c = 2*M_PI / res;
		unsigned int i = 0;
		//addPoint(x,y);
		for(i = 0; i < res; i++)
		{
			addPoint(x + r * cos(c*i),y + r * sin(c*i));
			addPoint(x + r * cos(c*(i+1)),y + r * sin(c*(i+1)));
		}
		//addPoint(x + r * cos(c*i),y + r * sin(c*i));
	}
	void LineDrawer::addPoint(float x, float y)
	{
		int zero = 0;
		float r = 0;
		float g = 1;
		float b = 1;
		float a = 1;
		
		float u = 41.f/1024.f;
		float v = 1-31.f/1024.f;
		
		memcpy(vertices + bufferOffset + 0,&x,4);
		memcpy(vertices + bufferOffset + 1,&y,4);
		memcpy(vertices + bufferOffset + 2,&zero,4);
		memcpy(vertices + bufferOffset + 3,&r,4);
		memcpy(vertices + bufferOffset + 4,&g,4);
		memcpy(vertices + bufferOffset + 5,&b,4);
		memcpy(vertices + bufferOffset + 6,&a,4);
		
		memcpy(vertices + bufferOffset + 7,&u,4);
		memcpy(vertices + bufferOffset + 8,&v,4);
		bufferOffset += 9;
		vertexCount += 1;
		
		/*if (indexCount > 0)
		{
			memcpy(vertices + bufferOffset + 0,&x,4);
			memcpy(vertices + bufferOffset + 1,&y,4);
			memcpy(vertices + bufferOffset + 2,&zero,4);
			memcpy(vertices + bufferOffset + 3,&white,4);
			bufferOffset += 4;
			vertexCount += 1;
		}
		
		indexCount += 1;*/
	}
	void LineDrawer::recalculate()
	{
		if (firsttime)
		{
			glGenVertexArrays(1, &vertexArray);
			glGenBuffers(1, &vertexBuffer);
			firsttime = false;
		}
		
		GL::BindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		//					THREE VERTICES * THREE VALUES * SIZEOF(FLOAT) -> DATA
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float) * vertexCount, vertices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(WumboVertexAttribPosition);
		//											3 VALS / VERTEX, FLOAT, 3*FLOAT = SIZE OF VERTEX
		glVertexAttribPointer(WumboVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), 0);//(char*)(0*sizeof(float)));
		glEnableVertexAttribArray(WumboVertexAttribColor);
		glVertexAttribPointer(WumboVertexAttribColor, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char*)(sizeof(float)*3));
		glEnableVertexAttribArray(WumboVertexAttribTexCoord0);
		glVertexAttribPointer(WumboVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char*)(sizeof(float)*7));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		GL::BindVertexArray(0);
	}
	void LineDrawer::draw()
	{
		uniformMatrix[0] = 0;//Sprite::STATICXOFFSET;//[0][0]
		uniformMatrix[1] = 0;//Sprite::STATICYOFFSET;//[0][1]
		uniformMatrix[2] = renderer->getVirtualRenderTarget().internalwidth;//[2][0]
		uniformMatrix[3] = renderer->getVirtualRenderTarget().internalheight;//[2][1]
			
		glUniformMatrix4fv(matrixUniformID, 1, GL_FALSE, &uniformMatrix[0]);
		GL::BindVertexArray(vertexArray);
		glDrawArrays(GL_LINES, 0, vertexCount);
	}
}