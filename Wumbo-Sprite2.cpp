#include "Wumbo-Sprite2.h"
#include "Wumbo-Types.h"
#include "Wumbo-Functions.h"
#include "Wumbo-Header.h"
#if PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <GLKit/GLKit.h>
#endif
#include "Wumbo-Program.h"
#include "Wumbo-Scene.h"
#include "Wumbo-Shader.h"
#include "Wumbo-Hitbox.h"

extern Wumbo::Shader *megaShader;

namespace Wumbo
{
	const unsigned int Sprite2::RECALC_VERTEX = 0x10;
	const unsigned int Sprite2::RECALC_TEXTURE = 0x100;
	const unsigned int Sprite2::RECALC_VBO = 0x1000;
	void Sprite2::recalculate(unsigned int kind)
	{
		switch(kind)
		{
		case RECALC_VERTEX:
			printf("THIS ISN'T NEEDED. YES!\n");
			//recalculateVertices();
			break;
		case RECALC_TEXTURE:
			recalculateTextureCoords();
			break;
		case RECALC_VBO:
			recalculateVBO();
			break;
		}
	}
	bool Sprite2::dirty(unsigned int kind)
	{
		switch(kind)
		{
		case RECALC_VERTEX:
			return vertexDirty;
			//recalculateVertices();
			break;
		case RECALC_TEXTURE:
			return textureDirty;
			break;
		case RECALC_VBO:
			return FALSE;
			break;
		}
		return false;
	}
	Sprite2::Sprite2(Renderer *renderR) : SpriteBase(renderR)
	{
		matrixUniformID = megaShader->getUniformIDFromName("supermatrix");
		shaderAlpha = 1;
		renderer = renderR;
		x = 0.f;
		y = 0.f;
		xscale = 1.f;
		yscale = 1.f;
		width = 0.f;
		height = 0.f;
		texture = NULL;
		flipX = false;
		flipY = false;
		subimage = 0;
		subimagecount = 0;
		vertexDirty = false;
		textureDirty = NULL;//false;
		colorDirty = false;
		rotation = 0;
		color = new GLfloat[16];
		GLfloat bob[] = {1.f,1.f,1.f,1.f,
						1.f,1.f,1.f,1.f,
						1.f,1.f,1.f,1.f,
						1.f,1.f,1.f,1.f};
		blendcolor[0] = blendcolor[1] = blendcolor[2] = blendcolor[3] = 1.f;
		memcpy(color,bob,16*sizeof(GLfloat));
		float verts[] = {
			0, 0, 0,
			1, 0, 0,
			0, 1, 0,
			1, 1, 0
		};
		memcpy(vertices,verts,12*sizeof(float));
		texU = NULL;
		texV = NULL;
		texW = NULL;
		texH = NULL;
		textureDirty = NULL;
		vertexArray = NULL;
		vertexBuffer = NULL;
		texcoords = NULL;

		setSubimageCount(1);
	}
	Sprite2::~Sprite2()
	{
	}

	void Sprite2::setSubimageCount(unsigned int count)
	{
		float *oldTexU = texU;
		float *oldTexV = texV;
		float *oldTexW = texW;
		float *oldTexH = texH;
		bool *oldTextureDirty = textureDirty;
		GLuint *oldVertexArray = vertexArray;
		GLuint *oldVertexBuffer = vertexBuffer;
		GLfloat *oldTexcoords = texcoords;

		texU = new float[count];
		texV = new float[count];
		texW = new float[count];
		texH = new float[count];
		textureDirty = new bool[count];
		vertexArray = new GLuint[count];
		vertexBuffer = new GLuint[count];
		texcoords = new GLfloat[8*count];

		unsigned int num = subimagecount<count?subimagecount:count;
		if (num > 0)
		{
			//printf("set subimageCount to %i and the old count (%i) was greater than zero so we're copying data!\n",count,subimagecount);
			memcpy(texU,oldTexU,sizeof(float)*num);
			memcpy(texV,oldTexV,sizeof(float)*num);
			memcpy(texW,oldTexW,sizeof(float)*num);
			memcpy(texH,oldTexH,sizeof(float)*num);
			memcpy(textureDirty,oldTextureDirty,sizeof(bool)*num);
			memcpy(vertexArray,oldVertexArray,sizeof(GLfloat)*num);
			memcpy(vertexBuffer,oldVertexBuffer,sizeof(GLfloat)*num);
			memcpy(texcoords,oldTexcoords,8*sizeof(GLfloat)*num);
		}
		if (count > subimagecount)
		{
			//printf("the new count is bigger than the old one so now we have to iterate from %i to %i\n",subimagecount,count);
			for(int i=subimagecount;i<count;i++)
			{
				//printf("generate vertex buffer and array #%i\n",i);
				glGenVertexArrays(1, &vertexArray[i]);
				GL::BindVertexArray(vertexArray[i]);
				glGenBuffers(1, &vertexBuffer[i]);
				glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
				
				glEnableVertexAttribArray(WumboVertexAttribPosition);
				glVertexAttribPointer(WumboVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char*)(0*sizeof(float)));
				glEnableVertexAttribArray(WumboVertexAttribColor);
				glVertexAttribPointer(WumboVertexAttribColor, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char*)(3*sizeof(float)));
				glEnableVertexAttribArray(WumboVertexAttribTexCoord0);
				glVertexAttribPointer(WumboVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 9*sizeof(float), (char*)(7*sizeof(float)));
				GL::BindVertexArray(0);
			}
		}

		if (count < subimagecount)
		{
			//printf("the new count is smaller than the old one so now we have to delete the old extra buffers and arrays\n");
			glDeleteBuffers(count-subimagecount,oldVertexArray+count*sizeof(GLuint));
#if PLATFORM_IOS
			glDeleteVertexArraysOES(count-subimagecount,oldVertexBuffer+count*sizeof(GLuint));
#elif PLATFORM_OSX
			glDeleteVertexArraysAPPLE(count-subimagecount,oldVertexBuffer+count*sizeof(GLuint));
#endif
		}
		delete[] oldTexU;
		delete[] oldTexV;
		delete[] oldTexW;
		delete[] oldTexH;
		delete[] oldTextureDirty;
		delete[] oldVertexArray;
		delete[] oldVertexBuffer;
		delete[] oldTexcoords;

		subimagecount = count;
	}
	unsigned int Sprite2::getSubimageCount()
	{
		return subimagecount;
	}
	
	
	void Sprite2::setActiveSubimage(int sub)
	{ 
		while(sub >= subimagecount)
			sub -= subimagecount;
		while(sub < 0)
			sub += subimagecount;
		subimage = sub;
	}
	int Sprite2::getActiveSubimage()
	{
		return subimage;
	}

	void Sprite2::setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		blendcolor[3] = a / 255.f;
		blendcolor[0] = r / 255.f;
		blendcolor[1] = g / 255.f;
		blendcolor[2] = b / 255.f;
	}

	void Sprite2::setColor(unsigned int colour)
	{
		blendcolor[3] = ((colour & 0xFF000000) >> 24) / 255.f;
		blendcolor[0] = ((colour & 0xFF0000) >> 16) / 255.f;
		blendcolor[1] = ((colour & 0xFF00) >> 8) / 255.f;
		blendcolor[2] = ((colour & 0xFF) >> 0) / 255.f;
	}

	unsigned int Sprite2::getColor()
	{
		return ((int)(blendcolor[3]*255) << 24 | (int)(blendcolor[0]*255) << 16 | (int)(blendcolor[1]*255) << 8 | (int)(blendcolor[2]*255) << 0);
	}

	void Sprite2::setScaleX(float xx)
	{
		xscale = xx;
	}
	float Sprite2::getScaleX()
	{
		return xscale;
	}
	void Sprite2::setScaleY(float yy)
	{
		yscale = yy;
	}
	float Sprite2::getScaleY()
	{
		return yscale;
	}
	void Sprite2::setScale(float xx, float yy)
	{
		xscale = xx;
		yscale = yy;
	}

	void Sprite2::recalculateTextureCoords()
	{
		//GLuint texWidth = texture->getWidth();
		//GLuint texHeight = texture->getHeight();
	    
		GLfloat left, top, right, bottom;
		if (!flipX)
		{
			left = texU[subimage];// - .5f/texWidth;
			right = (texU[subimage]+texW[subimage]);// + .5f/texWidth;
		}
		else
		{
			left = (texU[subimage]+texW[subimage]);// - .5f/texWidth;
			right = texU[subimage];// + .5f/texWidth;
		}
		if (!flipY)
		{
			top = 1-texV[subimage];// - .5f/texHeight;
			bottom = 1-(texV[subimage]+texH[subimage]);// + .5f/texHeight;
		}
		else
		{
			top = 1-(texV[subimage]+texH[subimage]);// - .5f/texHeight;
			bottom = 1-texV[subimage];// + .5f/texHeight;
		}
		GLfloat txc[] = {
				left, top,
				right, top,
				left, bottom,
				right, bottom
			};

		memcpy(&texcoords[8*subimage],txc,8*sizeof(GLfloat));
	}
	
	void Sprite2::recalculateVBO()
	{
		int i = -1;
		int j = -1;

		i = 0;
		j = 0;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[8*subimage + 2 * j], 2*sizeof(float)); // U V
		//printf("U0 V0 %f %f\n",texture->getWidth()*(texcoords[8*subimage + 2 * j]),texture->getWidth()*(texcoords[8*subimage + 2 * j + 1]));
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 1;
		j = 1;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[8*subimage + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 2;
		j = 2;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[8*subimage + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 3;
		j = 1;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[8*subimage + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 4;
		j = 2;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[8*subimage + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 5;
		j = 3;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[8*subimage + 2 * j], 2*sizeof(float)); // U V
		GL::BindVertexArray(vertexArray[subimage]);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[subimage]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		GL::BindVertexArray(0);
	}

	float Sprite2::getAlpha()
	{
		return shaderAlpha;
	}
	void Sprite2::setAlpha(float alpha5)
	{
		shaderAlpha = alpha5;
	}
	
	void Sprite2::draw()
	{
		if (vertexDirty)
		{
			//printf("vertex data was dirty!\n");
			recalculate(RECALC_VERTEX);
		}
		if (textureDirty[subimage])
		{
			//printf("texture data was dirty!\n");
			recalculate(RECALC_TEXTURE);
		}
		if (colorDirty)
		{
			//printf("color data was dirty!\n");
		}
		if (textureDirty[subimage] || vertexDirty || colorDirty)
		{
			//printf("vbo recalculated!\n");
			recalculate(RECALC_VBO);
		}
		colorDirty = false;
		textureDirty[subimage] = false;
		//printf("\t-> textureDirty[%i] %i\n",subimage,textureDirty[subimage]);
		vertexDirty = false;
		uniformMatrix[0] = x-xoffset;//[0][0]
		uniformMatrix[1] = y-yoffset;//[0][1]
		uniformMatrix[2] = xscale*width;//[0][2]
		uniformMatrix[3] = yscale*height;//[1][0]
		uniformMatrix[4] = renderer->getVirtualRenderTarget().internalwidth;//[1][1]
		uniformMatrix[5] = renderer->getVirtualRenderTarget().internalheight;//[1][2]
		uniformMatrix[6] = shaderAlpha;//[2][0]
		uniformMatrix[12] = blendcolor[0];
		uniformMatrix[13] = blendcolor[1];
		uniformMatrix[14] = blendcolor[2];
		uniformMatrix[15] = 1;//blendcolor[3];
		//6
		//7
		//8
		glUniformMatrix4fv(matrixUniformID, 1, GL_FALSE, uniformMatrix);
		GL::BindVertexArray(vertexArray[subimage]);
		glDrawArrays(GL_TRIANGLES, 0, 6);		
	}

	void Sprite2::setXOffset(float xx)
	{
		vertexDirty = true;
		xoffset = xx;
	}
	float Sprite2::getXOffset()
	{
		return xoffset;
	}
	void Sprite2::setYOffset(float yy)
	{
		vertexDirty = true;
		yoffset = yy;
	}
	float Sprite2::getYOffset()
	{
		return yoffset;
	}
	void Sprite2::setOffset(float xx, float yy)
	{
		vertexDirty = true;
		xoffset = xx;
		yoffset = yy;
	}

	void Sprite2::setRotation(float r)
	{
		vertexDirty = true;
		rotation = r;
	}
	float Sprite2::getRotation()
	{
		return rotation;
	}

	void Sprite2::setFlipX(bool xx)
	{
		memset(textureDirty,1,12*sizeof(bool));
		flipX = xx;
	}
	bool Sprite2::getFlipX()
	{
		return flipX;
	}
	void Sprite2::setFlipY(bool yy)
	{
		memset(textureDirty,1,12*sizeof(bool));
		flipY = yy;
	}
	bool Sprite2::getFlipY()
	{
		return flipY;
	}
	void Sprite2::setX(float xx)
	{
		x = xx;
	}
	float Sprite2::getX()
	{
		return x;
	}
	void Sprite2::setY(float yy)
	{
		y = yy;
	}
	float Sprite2::getY()
	{
		return y;
	}
	void Sprite2::setPosition(float xx, float yy)
	{
		x = xx;
		y = yy;
	}
	void Sprite2::setSize(float ww, float hh)
	{
		vertexDirty = true;
		width = ww;
		height = hh;
	}
	void Sprite2::setWidth(float ww)
	{
		vertexDirty = true;
		width = ww;
	}
	float Sprite2::getWidth()
	{
		return width;
	}

	void Sprite2::setHeight(float hh)
	{
		vertexDirty = true;
		height = hh;
	}
	float Sprite2::getHeight()
	{
		return height;
	}

	void Sprite2::setSubrectPixels(int subimg, Rectanglei rect)
	{
		textureDirty[subimg] = true;
		texU[subimg] = (float)rect.x/texture->getWidth();
		texV[subimg] = (float)rect.y/texture->getHeight();
		texW[subimg] = (float)rect.width/texture->getWidth();
		texH[subimg] = (float)rect.height/texture->getHeight();
	}
	
	Rectanglei Sprite2::getSubrectPixels(int subimg)
	{
		Rectanglei rect;
		rect.x = texU[subimg] * texture->getWidth();
		rect.y = texV[subimg] * texture->getHeight();
		rect.width = texW[subimg] * texture->getWidth();
		rect.height = texH[subimg] * texture->getHeight();
		return rect;
	}

	void Sprite2::setSubrectPixels(int subimg, int xx, int yy, int ww, int hh)
	{
		textureDirty[subimg] = true;
		texU[subimg] = (float)xx/texture->getWidth();
		texV[subimg] = (float)yy/texture->getHeight();
		texW[subimg] = (float)ww/texture->getWidth();
		texH[subimg] = (float)hh/texture->getHeight();
	}

	void Sprite2::setSubrect(int subimg, float xx, float yy, float ww, float hh)
	{
		textureDirty[subimg] = true;
		texU[subimg] = xx;
		texV[subimg] = yy;
		texW[subimg] = ww;
		texH[subimg] = hh;
	}

	void Sprite2::setTexU(int subimg, float uu)
	{
		textureDirty[subimg] = true;
		texU[subimg] = uu;
	}
	float Sprite2::getTexU(int subimg)
	{
		return texU[subimg];
	}
	void Sprite2::setTexV(int subimg, float vv)
	{
		textureDirty[subimg] = true;
		texV[subimg] = vv;
	}
	float Sprite2::getTexV(int subimg)
	{
		return texV[subimg];
	}
	void Sprite2::setTexW(int subimg, float ww)
	{
		textureDirty[subimg] = true;
		texW[subimg] = ww;
	}
	float Sprite2::getTexW(int subimg)
	{
		return texW[subimg];
	}
	void Sprite2::setTexH(int subimg, float hh)
	{
		textureDirty[subimg] = true;
		texH[subimg] = hh;
	}
	float Sprite2::getTexH(int subimg)
	{
		return texH[subimg];
	}
	void Sprite2::setTexture(Texture *tex)
	{
		texture = tex;
	}
	Texture *Sprite2::getTexture()
	{
		return texture;
	}

	void Sprite2::setBoundingBox(int left, int right, int top, int bottom)
	{
		boundingBox = Rectanglei(left,top,(right-left)+1,(bottom-top)+1);
	}

	Hitbox Sprite2::getHitbox()
	{
		Hitbox h;
		h.x = 0;
		h.y = 0;
		h.left = boundingBox.x-xoffset;
		h.top = boundingBox.y-yoffset;
		h.width = boundingBox.width;
		h.height = boundingBox.height;

		return h;
	}
};