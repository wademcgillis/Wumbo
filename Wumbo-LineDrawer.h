//
//  Wumbo-LineDrawer.h
//  fjords_ios
//
//  Created by Wade McGillis on 4/14/14.
//  Copyright (c) 2014 Wade McGillis. All rights reserved.
//

#ifndef __fjords_ios__Wumbo_LineDrawer__
#define __fjords_ios__Wumbo_LineDrawer__

#include "Wumbo-Resource.h"
#include "Wumbo-Texture.h"
#include "Wumbo-Renderer.h"
#include "Wumbo-Shader.h" 

namespace Wumbo
{
	class LineDrawer
	{
	private:
		bool firsttime;
		Wumbo::Renderer *renderer;
		GLuint vertexCount;
		GLuint bufferOffset;
		char *vertices[4 * 16 * 256];
		//GLfloat *vertexData;
		GLuint vertexArray;
		GLuint vertexBuffer;
		
		int matrixUniformID;
		int indexCount;
		float uniformMatrix[4 * 4];
	public:
		LineDrawer(Wumbo::Renderer *rend, Wumbo::Shader *shader);
		~LineDrawer();
		void clear();
		void addPoint(float x, float y);//, float r, float g, float b, float a);
		void addCircle(float x, float y, float radius, unsigned int res);//, float r, float g, float b, float a);
		void recalculate();
		void draw();
	};
};

#endif /* defined(__fjords_ios__Wumbo_LineDrawer__) */
