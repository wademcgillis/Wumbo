#if 0
#include "Wumbo-Sprite.h"
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
bool flobbo = false;
extern Wumbo::Shader *megaShader;
extern bool magicSuperMode;
namespace Wumbo
{
	Sprite::Sprite(Renderer *renderR)
	{
		/*displaySizeUniformID = megaShader->getUniformIDFromName("displaySize");//,renderer->getVirtualRenderTarget().internalwidth,renderer->getVirtualRenderTarget().internalheight);
		objectPositionUniformID = megaShader->getUniformIDFromName("objectPosition");//,x,y);
		objectScaleUniformID = megaShader->getUniformIDFromName("objectScale");//,xscale,yscale);
		displayOffsetUniformID = megaShader->getUniformIDFromName("displayOffset");*/
		
		matrixUniformID = megaShader->getUniformIDFromName("supermatrix");

		shaderAlpha = 1;

		//printf("HELP\n");
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
		textureDirty = false;
		colorDirty = false;

		rotation = 0;
		//printf("vertexBuffer = %i\nvertexArray = %i\n",vertexBuffer,vertexArray);
		color = new GLfloat[16];
		GLfloat bob[] = {1.f,1.f,1.f,1.f,
						1.f,1.f,1.f,1.f,
						1.f,1.f,1.f,1.f,
						1.f,1.f,1.f,1.f};
		blendcolor[0] = blendcolor[1] = blendcolor[2] = blendcolor[3] = 1.f;
		memcpy(color,bob,16*sizeof(GLfloat));

		texU = NULL;
		texV = NULL;
		texW = NULL;
		texH = NULL;
		textureDirty = NULL;
		vertexArray = NULL;
		vertexBuffer = NULL;
		texcoords = NULL;

		setSubimageCount(1);
//#endif
	}
	Sprite::~Sprite()
	{
	}

	void Sprite::setSubimageCount(unsigned int count)
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
			glDeleteVertexArrays(count-subimagecount,oldVertexBuffer+count*sizeof(GLuint));
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
	unsigned int Sprite::getSubimageCount()
	{
		return subimagecount;
	}
	
	
	void Sprite::setActiveSubimage(int sub)
	{ 
		while(sub >= subimagecount)
			sub -= subimagecount;
		while(sub < 0)
			sub += subimagecount;
		subimage = sub;
	}
	int Sprite::getActiveSubimage()
	{
		return subimage;
	}

	void Sprite::setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
		//if (abs(r/255.f - blendcolor[0]) < 1.f/255.f && abs(g/255.f - blendcolor[1]) < 1.f/255.f && abs(b/255.f - blendcolor[2]) < 1.f/255.f && abs(a/255.f - blendcolor[3]) < 1.f/255.f)
		//	return;
		blendcolor[3] = a / 255.f;
		blendcolor[0] = r / 255.f;
		blendcolor[1] = g / 255.f;
		blendcolor[2] = b / 255.f;
		//colorDirty = true;
		/*color[0] = color[4] = color[8] = color[12] = r / 255.f;
		color[1] = color[5] = color[9] = color[13] = g / 255.f;
		color[2] = color[6] = color[10] = color[14] = b / 255.f;
		color[3] = color[7] = color[11] = color[15] = a / 255.f;*/
	}

	void Sprite::setColor(unsigned int colour)
	{
		//if (colour == getColor())
		//	return;
		blendcolor[3] = ((colour & 0xFF000000) >> 24) / 255.f;
		blendcolor[0] = ((colour & 0xFF0000) >> 16) / 255.f;
		blendcolor[1] = ((colour & 0xFF00) >> 8) / 255.f;
		blendcolor[2] = ((colour & 0xFF) >> 0) / 255.f;
		//colorDirty = true;
		/*color[3] = color[7] = color[11] = color[15] = ((colour & 0xFF000000) >> 24) / 255.f;
		color[0] = color[4] = color[8] = color[12] = ((colour & 0xFF0000) >> 16) / 255.f;
		color[1] = color[5] = color[9] = color[13] = ((colour & 0xFF00) >> 8) / 255.f;
		color[2] = color[6] = color[10] = color[14] = ((colour & 0xFF)) / 255.f;*/
	}

	unsigned int Sprite::getColor()
	{
		return ((int)(blendcolor[3]*255) << 24 | (int)(blendcolor[0]*255) << 16 | (int)(blendcolor[1]*255) << 8 | (int)(blendcolor[2]*255) << 0);
	}

	void Sprite::setScaleX(float xx)
	{
		xscale = xx;
	}
	float Sprite::getScaleX()
	{
		return xscale;
	}
	void Sprite::setScaleY(float yy)
	{
		yscale = yy;
	}
	float Sprite::getScaleY()
	{
		return yscale;
	}
	void Sprite::setScale(float xx, float yy)
	{
		xscale = xx;
		yscale = yy;
	}

	void Sprite::recalculateTextureCoords()
	{
		GLuint texWidth = texture->getWidth();
		GLuint texHeight = texture->getHeight();
	    
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
	
	void Sprite::recalculateVerticesWithRotation()
	{
		float tempx = x;
		float tempy = y;
		const GLfloat xpx = 1.f/(renderer->getVirtualRenderTarget().internalwidth/2);
		const GLfloat ypx = 1.f/(renderer->getVirtualRenderTarget().internalheight/2);
		//printf("x %f\ty %f\n",xpx,ypx);
		//tempx -= view_xview;//gameEngine->GetViewX();
		//tempy -= gameEngine->GetViewY();
		GLfloat xxx1 = 0.f, xxx2 = 0.f, xxx3 = 0.f, xxx4 = 0.f, yyy1 = 0.f, yyy2 = 0.f, yyy3 = 0.f, yyy4 = 0.f;
		if (renderer->getRotation() == ScreenRotation_DOWN)
		{
			vec2df topleft = rotatePointDEG(tempx-xoffset,tempy-yoffset,tempx,tempy,rotation);
			vec2df topright = rotatePointDEG(tempx-xoffset+width,tempy-yoffset,tempx,tempy,rotation);
			vec2df bottomright = rotatePointDEG(tempx-xoffset+width,tempy-yoffset+height,tempx,tempy,rotation);
			vec2df bottomleft = rotatePointDEG(tempx-xoffset,tempy-yoffset+height,tempx,tempy,rotation);

			xxx1 = -1+topleft.x*xpx;
			yyy1 = 1-topleft.y*ypx;
			xxx2 = -1+topright.x*xpx;
			yyy2 = 1-topright.y*ypx;
			xxx3 = -1+bottomleft.x*xpx;
			yyy3 = 1-bottomleft.y*ypx;
			xxx4 = -1+bottomright.x*xpx;
			yyy4 = 1-bottomright.y*ypx;
		}
		else if (renderer->getRotation() == ScreenRotation_RIGHT)
		{
			xxx1 = 1-tempy*ypx;
			yyy1 = 1-tempx*xpx;
			xxx2 = 1-tempy*ypx;
			yyy2 = 1-tempx*xpx-width*xscale*xpx;
			xxx3 = 1-tempy*ypx-height*yscale*ypx;
			yyy3 = 1-tempx*xpx;
			xxx4 = 1-tempy*ypx-height*yscale*ypx;
			yyy4 = 1-tempx*xpx-width*xscale*xpx;
		}
		else if (renderer->getRotation() == ScreenRotation_LEFT)
		{
			xxx1 = -1+tempy*ypx;
			yyy1 = -1+tempx*xpx;
			xxx2 = -1+tempy*ypx;
			yyy2 = -1+tempx*xpx+width*xscale*xpx,
			xxx3 = -1+tempy*ypx+height*yscale*ypx;
			yyy3 = -1+tempx*xpx;
			xxx4 = -1+tempy*ypx+height*yscale*ypx;
			yyy4 = -1+tempx*xpx+width*xscale*xpx;
		}
		else if (renderer->getRotation() == ScreenRotation_UP)
		{
			xxx1 = 1-tempx*xpx;
			yyy1 = -1+tempy*ypx;
			xxx2 = 1-tempx*xpx-width*xscale*xpx;
			yyy2 = -1+tempy*ypx;
			xxx3 = 1-tempx*xpx;
			yyy3 = -1+tempy*ypx+height*yscale*ypx;
			xxx4 = 1-tempx*xpx-width*xscale*xpx;
			yyy4 = -1+tempy*ypx+height*yscale*ypx;
		}
	    
		float verts[] = {
			xxx1, yyy1,
			xxx2, yyy2,
			xxx3, yyy3,
			xxx4, yyy4
		};

		//printf("SPRITE POSITION1: %f, %f\n",xxx1,yyy1);
		//printf("SPRITE POSITION2: %f, %f\n",xxx2,yyy2);
		//printf("SPRITE POSITION3: %f, %f\n",xxx3,yyy3);
		//printf("SPRITE POSITION4: %f, %f\n",xxx4,yyy4);
	    
		memcpy(vertices,verts,8*sizeof(float));
	}
	void Sprite::recalculateVertices()
	{
		if (rotation < -.001 || rotation > .001)
		{
			recalculateVerticesWithRotation();
			return;
		}
		GLfloat xxx1 = 0.f, xxx2 = width, xxx3 = 0.f, xxx4 = width, yyy1 = 0.f, yyy2 = 0.f, yyy3 = height, yyy4 = height;
#if !USE_VBOS
		float tempx = x;
		float tempy = y;

		const GLfloat xpx = 1.f/(renderer->getVirtualRenderTarget().internalwidth/2);
		const GLfloat ypx = 1.f/(renderer->getVirtualRenderTarget().internalheight/2);
		//printf("x %f\ty %f\n",xpx,ypx);
		//tempx -= view_xview;//gameEngine->GetViewX();
		//tempy -= gameEngine->GetViewY();
		

		if (renderer->getRotation() == ScreenRotation_DOWN)
		{
			xxx1 = -1+tempx*xpx;
			yyy1 = 1-tempy*ypx;
		 
			xxx2 = -1+tempx*xpx+width*xscale*xpx;
			yyy2 = 1-tempy*ypx;
		 
			xxx3 = -1+tempx*xpx;
			yyy3 = 1-tempy*ypx-height*yscale*ypx;
		 
			xxx4 = -1+tempx*xpx+width*xscale*xpx;
			yyy4 = 1-tempy*ypx-height*yscale*ypx;
		}
		else if (renderer->getRotation() == ScreenRotation_RIGHT)
		{
			xxx1 = 1-tempy*ypx;
			yyy1 = 1-tempx*xpx;
			xxx2 = 1-tempy*ypx;
			yyy2 = 1-tempx*xpx-width*xscale*xpx;
			xxx3 = 1-tempy*ypx-height*yscale*ypx;
			yyy3 = 1-tempx*xpx;
			xxx4 = 1-tempy*ypx-height*yscale*ypx;
			yyy4 = 1-tempx*xpx-width*xscale*xpx;
		}
		else if (renderer->getRotation() == ScreenRotation_LEFT)
		{
			xxx1 = -1+tempy*ypx;
			yyy1 = -1+tempx*xpx;
			xxx2 = -1+tempy*ypx;
			yyy2 = -1+tempx*xpx+width*xscale*xpx,
			xxx3 = -1+tempy*ypx+height*yscale*ypx;
			yyy3 = -1+tempx*xpx;
			xxx4 = -1+tempy*ypx+height*yscale*ypx;
			yyy4 = -1+tempx*xpx+width*xscale*xpx;
		}
		else if (renderer->getRotation() == ScreenRotation_UP)
		{
			xxx1 = 1-tempx*xpx;
			yyy1 = -1+tempy*ypx;
			xxx2 = 1-tempx*xpx-width*xscale*xpx;
			yyy2 = -1+tempy*ypx;
			xxx3 = 1-tempx*xpx;
			yyy3 = -1+tempy*ypx+height*yscale*ypx;
			xxx4 = 1-tempx*xpx-width*xscale*xpx;
			yyy4 = -1+tempy*ypx+height*yscale*ypx;
		}
#endif
		float verts[] = {
			xxx1, yyy1, 0,
			xxx2, yyy2, 0,
			xxx3, yyy3, 0,
			xxx4, yyy4, 0
		};

		//printf("SPRITE POSITION1: %f, %f\n",xxx1,yyy1);
		//printf("SPRITE POSITION2: %f, %f\n",xxx2,yyy2);
		//printf("SPRITE POSITION3: %f, %f\n",xxx3,yyy3);
		//printf("SPRITE POSITION4: %f, %f\n",xxx4,yyy4);
	    
		memcpy(vertices,verts,12*sizeof(float));
	}
	void Sprite::recalculateVBO()
	{
		//NSLog(@"recalculate VBO");
#if USE_VBOS
		int i = -1;
		int j = -1;
	
#if TRU_VBO
		i = 0;
		j = 0;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[0 + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 1;
		j = 1;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[0 + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 2;
		j = 2;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[0 + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		i = 3;
		j = 3;
		memcpy(&vertexData[0 + 9*i], &vertices[0 + 3 * j], 3*sizeof(float)); // X Y Z
		memcpy(&vertexData[3 + 9*i], &color[0 + 4 * j], 4*sizeof(float)); // R G B A
		memcpy(&vertexData[7 + 9*i], &texcoords[0 + 2 * j], 2*sizeof(float)); // U V
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
#else
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
#endif
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		
		//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
		
		//glGenVertexArraysOES(1, &vertexArray);
#if !TRU_VBO
		GL::BindVertexArray(vertexArray[subimage]);
#endif
		//glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[subimage]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		/*glEnableVertexAttribArray(GLKVertexAttribPosition);
		glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 24, 0);
		glEnableVertexAttribArray(GLKVertexAttribColor);
		glVertexAttribPointer(GLKVertexAttribColor, 4, GL_FLOAT, GL_FALSE, 24, (char*)3);
		glEnableVertexAttribArray(GLKVertexAttribTexCoord0);
		glVertexAttribPointer(GLKVertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 24, (char*)7);*/
		
#if !TRU_VBO
		GL::BindVertexArray(0);//vertexArray[subimage]);
#endif
#endif
	}

	void Sprite::setVertices(GLfloat xxx1, GLfloat yyy1, GLfloat xxx2, GLfloat yyy2, GLfloat xxx3, GLfloat yyy3, GLfloat xxx4, GLfloat yyy4)
	{
		vertices[0] = xxx1;
		vertices[1] = yyy1;
		vertices[3] = xxx2;
		vertices[4] = yyy2;
		vertices[6] = xxx3;
		vertices[7] = yyy3;
		vertices[9] = xxx4;
		vertices[10] = yyy4;
	}
#if PLATFORM_IOS
#include <QuartzCore/QuartzCore.h>
#endif
//#include "Wumbo-iOSRoot.h"
	/*
	uniform vec2 displaySize;
	uniform vec2 objectPosition;
	uniform vec2 displayOffset;
	 */
	
		float Sprite::getAlpha()
		{
			return shaderAlpha;
		}
		void Sprite::setAlpha(float alpha5)
		{
			shaderAlpha = alpha5;
		}
	
	void Sprite::draw(int dispOffX, int dispOffY)
	{
#if PLATFORM_IOS
		double before = CACurrentMediaTime();
#endif
		if (vertexDirty)
		{
			//printf("vertex data was dirty!\n");
			recalculateVertices();
		}
		if (textureDirty[subimage])
		{
			//printf("texture data was dirty!\n");
			recalculateTextureCoords();
		}
		if (colorDirty)
		{
			//printf("color data was dirty!\n");
		}
		//printf("texDirty size: %i/%i = %i... subimage: %i\n",sizeof(textureDirty),sizeof(bool),sizeof(textureDirty)/sizeof(bool),subimage);
		//printf("textureDirty[%i] %i\n",subimage,textureDirty[subimage]);
		if (textureDirty[subimage] || vertexDirty || colorDirty)
		{
			//printf("vbo recalculated!\n");
			recalculateVBO();
		}
		colorDirty = false;
		textureDirty[subimage] = false;
		//printf("\t-> textureDirty[%i] %i\n",subimage,textureDirty[subimage]);
		vertexDirty = false;
#if USE_VBOS
		uniformMatrix[0] = x-dispOffX-xoffset;//[0][0]
		uniformMatrix[1] = y-dispOffY-yoffset;//[0][1]

		uniformMatrix[2] = xscale;//*width;//[0][2]
		uniformMatrix[3] = yscale;//*height;//[1][0]

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
		/*glUniform2f(displaySizeUniformID,renderer->getVirtualRenderTarget().internalwidth,renderer->getVirtualRenderTarget().internalheight);
		glUniform2f(objectPositionUniformID,x,y);
		glUniform2f(objectScaleUniformID,xscale,yscale);*/
#if TRU_VBO
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLE_STRIP, sizeof(indices)/sizeof(GLubyte), GL_UNSIGNED_BYTE, (void*)0);
#else
		GL::BindVertexArray(vertexArray[subimage]);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
		//glUniform2f(displayOffsetUniformID,0,0);
#else
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
		glColorPointer(4, GL_FLOAT, 0, color);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#endif
#if PLATFORM_IOS
		double after = CACurrentMediaTime();
#endif
		
		//NSLog(@"Time: %f",(after-before)*1000000);
	}

	void Sprite::setXOffset(float xx)
	{
		vertexDirty = true;
		xoffset = xx;
	}
	float Sprite::getXOffset()
	{
		return xoffset;
	}
	void Sprite::setYOffset(float yy)
	{
		vertexDirty = true;
		yoffset = yy;
	}
	float Sprite::getYOffset()
	{
		return yoffset;
	}
	void Sprite::setOffset(float xx, float yy)
	{
		vertexDirty = true;
		xoffset = xx;
		yoffset = yy;
	}

	void Sprite::setRotation(float r)
	{
		vertexDirty = true;
		rotation = r;
	}
	float Sprite::getRotation()
	{
		return rotation;
	}

	void Sprite::setFlipX(bool xx)
	{
		memset(textureDirty,1,12*sizeof(bool));
		flipX = xx;
	}
	bool Sprite::getFlipX()
	{
		return flipX;
	}
	void Sprite::setFlipY(bool yy)
	{
		memset(textureDirty,1,12*sizeof(bool));
		flipY = yy;
	}
	bool Sprite::getFlipY()
	{
		return flipY;
	}
	void Sprite::setX(float xx)
	{
#if !USE_VBOS
		vertexDirty = true;
#endif
		x = xx;
	}
	float Sprite::getX()
	{
		return x;
	}
	void Sprite::setY(float yy)
	{
#if !USE_VBOS
		vertexDirty = true;
#endif
		y = yy;
	}
	float Sprite::getY()
	{
		return y;
	}
	void Sprite::setPosition(float xx, float yy)
	{
#if !USE_VBOS
		vertexDirty = true;
#endif
		x = xx;
		y = yy;
	}
	void Sprite::setSize(float ww, float hh)
	{
		vertexDirty = true;
		width = ww;
		height = hh;
	}
	void Sprite::setWidth(float ww)
	{
		vertexDirty = true;
		width = ww;
	}
	float Sprite::getWidth()
	{
		return width;
	}

	void Sprite::setHeight(float hh)
	{
		vertexDirty = true;
		height = hh;
	}
	float Sprite::getHeight()
	{
		return height;
	}

	void Sprite::setSubrectPixels(int subimg, Rectanglei rect)
	{
		textureDirty[subimg] = true;
		texU[subimg] = (float)rect.x/texture->getWidth();
		texV[subimg] = (float)rect.y/texture->getHeight();
		texW[subimg] = (float)rect.width/texture->getWidth();
		texH[subimg] = (float)rect.height/texture->getHeight();
	}
	
	Rectanglei Sprite::getSubrectPixels(int subimg)
	{
		Rectanglei rect;
		rect.x = texU[subimg] * texture->getWidth();
		rect.y = texV[subimg] * texture->getHeight();
		rect.width = texW[subimg] * texture->getWidth();
		rect.height = texH[subimg] * texture->getHeight();
		return rect;
	}

	void Sprite::setSubrectPixels(int subimg, int xx, int yy, int ww, int hh)
	{
		textureDirty[subimg] = true;
		texU[subimg] = (float)xx/texture->getWidth();
		texV[subimg] = (float)yy/texture->getHeight();
		texW[subimg] = (float)ww/texture->getWidth();
		texH[subimg] = (float)hh/texture->getHeight();
	}

	void Sprite::setSubrect(int subimg, float xx, float yy, float ww, float hh)
	{
		textureDirty[subimg] = true;
		texU[subimg] = xx;
		texV[subimg] = yy;
		texW[subimg] = ww;
		texH[subimg] = hh;
	}

	void Sprite::setTexU(int subimg, float uu)
	{
		textureDirty[subimg] = true;
		texU[subimg] = uu;
	}
	float Sprite::getTexU(int subimg)
	{
		return texU[subimg];
	}
	void Sprite::setTexV(int subimg, float vv)
	{
		textureDirty[subimg] = true;
		texV[subimg] = vv;
	}
	float Sprite::getTexV(int subimg)
	{
		return texV[subimg];
	}
	void Sprite::setTexW(int subimg, float ww)
	{
		textureDirty[subimg] = true;
		texW[subimg] = ww;
	}
	float Sprite::getTexW(int subimg)
	{
		return texW[subimg];
	}
	void Sprite::setTexH(int subimg, float hh)
	{
		textureDirty[subimg] = true;
		texH[subimg] = hh;
	}
	float Sprite::getTexH(int subimg)
	{
		return texH[subimg];
	}
	void Sprite::setTexture(Texture *tex)
	{
		texture = tex;
	}
	Texture *Sprite::getTexture()
	{
		return texture;
	}

	void Sprite::setBoundingBox(int left, int right, int top, int bottom)
	{
		boundingBox = Rectanglei(left,top,(right-left)+1,(bottom-top)+1);
	}

	Hitbox Sprite::getHitbox()
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
#endif