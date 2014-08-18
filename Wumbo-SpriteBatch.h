//
//  Wumbo-SpriteBatch.h
//  fjords_ios
//
//  Created by Wade McGillis on 4/19/14.
//  Copyright (c) 2014 Wade McGillis. All rights reserved.
//

#ifndef __fjords_ios__Wumbo_SpriteBatch__
#define __fjords_ios__Wumbo_SpriteBatch__

#include "Wumbo-Header.h"
#if PLATFORM_WINDOWS
#include <GL/glew.h>
#endif

#define MAX_WUMBOBATCH_SPRITECOUNT 6000
#define MAX_WUMBOBATCH_TRIANGLE_COUNT 2*MAX_WUMBOBATCH_SPRITECOUNT

namespace Wumbo
{
	typedef struct
	{
		float x, y, z;
		unsigned char r, g, b, a;
		float u, v;
	} WVertex;
	class SpriteBatch
	{
	private:
		GLuint matrixUniformID;
		float uniformMatrix[16];
		GLuint vertexbuffer;
		GLuint vertexarray;
		WVertex wvertices[3*MAX_WUMBOBATCH_TRIANGLE_COUNT];// 4000 triangles -> 2000 sprites
		unsigned int wvertexcount;
	public:
		SpriteBatch(GLuint matrix_uniform_id);
		void pushsprite(float x, float y, float w, float h, unsigned int color, float alpha,  float uu, float vv, float ww, float hh);
		void pushspriteUVUV(float x1, float y1, float x2, float y2, unsigned int color, float alpha, float u1, float v1, float u2, float v2);
		void pushvertex(float x, float y, float z, unsigned int color, float u, float v);
		void pushvertex(float x, float y, float z, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v);
		void replaceUV(unsigned int index, float u, float v);
		void clear();
		void update();
		void render();
	};
};

#endif /* defined(__fjords_ios__Wumbo_SpriteBatch__) */
