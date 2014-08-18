//
//  Wumbo-SpriteBatch.cpp
//  fjords_ios
//
//  Created by Wade McGillis on 4/19/14.
//  Copyright (c) 2014 Wade McGillis. All rights reserved.
//

#include "Wumbo-SpriteBatch.h"
#include "Wumbo-Renderer.h"
#include "Wumbo-Quicky.h"
#include "Wumbo-Shader.h"
#if PLATFORM_PC
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>

#define fmin(a,b) (a)<(b)?(a):(b)
#define fmax(a,b) (a)>(b)?(a):(b)
#endif

namespace Wumbo
{
	inline WVertex MakeWVertex(float X, float Y, float Z, float R, float G, float B, float A, float U, float V)
	{
		WVertex v;
		v.x = X;
		v.y = Y;
		v.z = Z;
		v.r = R;
		v.g = G;
		v.b = B;
		v.a = A;
		v.u = U;
		v.v = V;
		return v;
	}
	
	SpriteBatch::SpriteBatch(GLuint matrix_uniform_id)
	{
		matrixUniformID = matrix_uniform_id;
		printf("matrixUniformID = %i\n",matrixUniformID);
		uniformMatrix[0] = 0;
		uniformMatrix[1] = 0;

		memset(&wvertices,1,sizeof(wvertices));
		wvertexcount = 3*MAX_WUMBOBATCH_TRIANGLE_COUNT;
		if (Wumbo::OpenGL2())
		{
			glGenVertexArrays(1, &vertexarray);
			GL::BindVertexArray(vertexarray);
			glGenBuffers(1, &vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(WVertex)*wvertexcount, wvertices, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(WumboVertexAttribPosition);
			glVertexAttribPointer(WumboVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(WVertex), (char*)(0));
			glEnableVertexAttribArray(WumboVertexAttribColor);
			glVertexAttribPointer(WumboVertexAttribColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(WVertex), (char*)(12));
			glEnableVertexAttribArray(WumboVertexAttribTexCoord0);
			glVertexAttribPointer(WumboVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(WVertex), (char*)(16));
			//glEnableVertexAttribArray(Wumbo::WumboVertexAttribNormal);
			//glVertexAttribPointer(Wumbo::WumboVertexAttribNormal, 3, GL_FLOAT, GL_TRUE, sizeof(LD29Vertex), (char*)(24));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			GL::BindVertexArray(0);
		}
		wvertexcount = 0;
		//glMapBufferOES(vertexbuffer, GL_WRITE_ONLY_OES);
	}
	void SpriteBatch::pushsprite(float x1, float y1, float x2, float y2, unsigned int color, float alpha, float uu, float vv, float ww, float hh)
	{
		if (fmax(x1,x2) < 0)
			return;
		if (fmax(y1,y2) < 0)
			return;
		if (fmin(x1,x2) > 320)
			return;
		if (fmin(y1,y2) > 480)
			return;
		/*if (alpha < 1)
		{
			//printf("\tSpriteBatch::pushsprite()\n");
			printf("     alpha = %f\n",alpha);
			printf("     color = %X\n",color);
			printf("alphashift = %X\n",((int)(alpha * ((0xFF000000 & color) >> 24))) << 24);
		}*/
		color = (((int)(alpha * ((0xFF000000 & color) >> 24))) << 24) | (0x00FFFFFF & color);
		//if (alpha < 1)
		//	printf("  newcolor = %X\n",color);
		pushvertex(x1, y1, 0, color, uu,vv);
		pushvertex(x2, y1, 0, color, uu+ww,vv);
		pushvertex(x1, y2, 0, color, uu,vv+hh);
		
		pushvertex(x2, y1, 0, color, uu+ww,vv);
		pushvertex(x2, y2, 0, color, uu+ww,vv+hh);
		pushvertex(x1, y2, 0, color, uu,vv+hh);
	}
	void SpriteBatch::pushspriteUVUV(float x1, float y1, float x2, float y2, unsigned int color, float alpha, float u1, float v1, float u2, float v2)
	{
		/*if (alpha < 1)
		 {
		 //printf("\tSpriteBatch::pushsprite()\n");
		 printf("     alpha = %f\n",alpha);
		 printf("     color = %X\n",color);
		 printf("alphashift = %X\n",((int)(alpha * ((0xFF000000 & color) >> 24))) << 24);
		 }*/
		color = (((int)(alpha * ((0xFF000000 & color) >> 24))) << 24) | (0x00FFFFFF & color);
		//if (alpha < 1)
		//	printf("  newcolor = %X\n",color);
		pushvertex(x1, y1, 0, color, u1,v1);
		pushvertex(x2, y1, 0, color, u2,v1);
		pushvertex(x1, y2, 0, color, u1,v2);
		
		pushvertex(x2, y1, 0, color, u2,v1);
		pushvertex(x2, y2, 0, color, u2,v2);
		pushvertex(x1, y2, 0, color, u1,v2);
	}
	void SpriteBatch::pushvertex(float x, float y, float z, unsigned int color, float u, float v)
	{
		wvertices[wvertexcount] = MakeWVertex(x,y,z,(color >> 16) & 0xFF,(color >> 8) & 0xFF,(color >> 0) & 0xFF,(color >> 24) & 0xFF,u,-v);
		wvertexcount++;
	}
	void SpriteBatch::pushvertex(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v)
	{
		wvertices[wvertexcount] = MakeWVertex(x,y,z,r,g,b,a,u,-v);
		wvertexcount++;
	}
	void SpriteBatch::replaceUV(unsigned int index, float u, float v)
	{
		wvertices[index].u = u;
		wvertices[index].v = v;
	}
	void SpriteBatch::clear()
	{
		wvertexcount = 0;
		//printf("SpriteBatch::clear()\n");
	}
	void SpriteBatch::update()
	{
		//printf("SpriteBatch::update()\n");
		if (Wumbo::OpenGL2())
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(WVertex)*wvertexcount, wvertices);//, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	void SpriteBatch::render()
	{
		//printf("SpriteBatch::render() : (%i,%i)\n",Wumbo::__quickyRenderer->getVirtualRenderTarget().internalwidth,Wumbo::__quickyRenderer->getVirtualRenderTarget().internalheight);
		//printf("\t%i vertices\n",wvertexcount);
		if (Wumbo::OpenGL2())
		{
			uniformMatrix[2] = Wumbo::__quickyRenderer->getVirtualRenderTarget().internalwidth/2;//2.f;//[2][0]
			uniformMatrix[3] = Wumbo::__quickyRenderer->getVirtualRenderTarget().internalheight/2;///2.f;//[2][
			glUniformMatrix4fv(matrixUniformID, 1, GL_FALSE, &uniformMatrix[0]);
			GL::BindVertexArray(vertexarray);
			glDrawArrays(GL_TRIANGLES, 0, wvertexcount);
			GL::BindVertexArray(0);
		}
		else
		{
#if !PLATFORM_IOS // fix this later
			glVertexPointer(3, GL_FLOAT, sizeof(WVertex), ((char*)wvertices));
			glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(WVertex), ((char*)wvertices) + 12);
			glTexCoordPointer(2, GL_FLOAT, sizeof(WVertex), ((char*)wvertices) + 16);
			//glNormalPointer(GL_FLOAT, sizeof(LD29Vertex), ((char*)vertices) + 24);
			glLoadMatrixf(glm::value_ptr(glm::ortho(0.f,(float)Wumbo::__quickyRenderer->getVirtualRenderTarget().internalwidth,(float)Wumbo::__quickyRenderer->getVirtualRenderTarget().internalheight,0.f)));
			glDrawArrays(GL_TRIANGLES,0,wvertexcount);
			glLoadMatrixf(glm::value_ptr(glm::mat4(1.f)));
#endif
		}
	}
};