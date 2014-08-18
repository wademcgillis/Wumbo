#include "Wumbo-Sprite.h"
#include "Wumbo-Types.h"
#include "Wumbo-Functions.h"
#include "Wumbo-Header.h"
#if PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <GLKit/GLKit.h>
#else
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#endif
#include "Wumbo-Program.h"
#include "Wumbo-Scene.h"
#include "Wumbo-Shader.h"
#include "Wumbo-Hitbox.h"

extern Wumbo::Shader *megaShader;
int SUPER_XOFFSET = 0;
int SUPER_YOFFSET = 0;

namespace Wumbo
{
	int Sprite::STATICXOFFSET = 0;
	int Sprite::STATICYOFFSET = 0;

	const unsigned int Sprite::RECALC_VERTEX = 0x10;
	const unsigned int Sprite::RECALC_TEXTURE = 0x100;
	const unsigned int Sprite::RECALC_VBO = 0x1000;
	
	Wumbo::SpriteBatch *Sprite::static_batch = NULL;
	bool Sprite::static_batching = false;
	
	void Sprite::SetBatcher(Wumbo::SpriteBatch *batcher)
	{
		static_batch = batcher;
	}
	Wumbo::SpriteBatch *Sprite::GetBatcher()
	{
		return static_batch;
	}
	void Sprite::BeginBatch()
	{
		if (static_batch == NULL)
			return;
		//printf("\t\tBEGIN THE BATCH\n");
		static_batching = true;
		static_batch->clear();
	}
	void Sprite::EndBatch()
	{
		if (static_batch == NULL)
			return;
		//printf("\t\tEND THE BATCH\n");
		static_batching = false;
		static_batch->update();
	}
	void Sprite::RenderBatch()
	{
		if (static_batch == NULL)
			return;
		//printf("\t\tRENDER THE BATCH\n");
		static_batch->render();
	}
	void Sprite::recalculate(unsigned int kind)
	{
		if (GL2)
		{
			switch(kind)
			{
			case RECALC_VERTEX:
				printf("THIS ISN'T NEEDED. YES!\n");
				recalculateVertices();
				break;
			case RECALC_TEXTURE:
				recalculateTextureCoords();
				break;
			case RECALC_VBO:
				recalculateVBO();
				break;
			}
		} // end GL2
		else
		{
		}
	}
	Sprite::Sprite(Renderer *renderR)
	{
		static_batching = false;
		GL2 = Wumbo::OpenGL2();
		GL1_USEMATRICES = false;
		if (renderR->getGLVersion() == 2)
			GL2 = true;
		if (GL2)
		{
			matrixUniformID = megaShader->getUniformIDFromName("supermatrix");
			
			shaderAlpha = 1;
			blendcolor[0] = blendcolor[1] = blendcolor[2] = blendcolor[3] = 1.f;
			float verts[] = {
				0, 0, 0,
				1, 0, 0,
				0, 1, 0,
				1, 1, 0
			};
			memcpy(vertices,verts,12*sizeof(float));
			color = new GLfloat[16];
			GLfloat bob[] = {1.f,1.f,1.f,1.f,
							1.f,1.f,1.f,1.f,
							1.f,1.f,1.f,1.f,
							1.f,1.f,1.f,1.f};
			
			memcpy(color,bob,16*sizeof(GLfloat));
		}
		else
		{
			if (GL1_USEMATRICES)
			{
				float verts[] = {
					0, 0, 0,
					1, 0, 0,
					0, 1, 0,
					1, 1, 0
				};
				memcpy(vertices,verts,12*sizeof(float));
			}
			colorGL1 = new GLubyte[16];
			GLubyte bob[] = {255,255,255,255,
							255,255,255,255,
							255,255,255,255,
							255,255,255,255};
			
			memcpy(colorGL1,bob,16*sizeof(GLubyte));
		}
		colorUINT = 0xFFFFFFFF;
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
		colorDirty = false;
		rotation = 0;
		
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
	Sprite::~Sprite()
	{
	}

	void Sprite::setSubimageCount(unsigned int count)
	{
		if (GL2)
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
		} // end GL2
		else
		{
			float *oldTexU = texU;
			float *oldTexV = texV;
			float *oldTexW = texW;
			float *oldTexH = texH;
			bool *oldTextureDirty = textureDirty;
			GLfloat *oldTexcoords = texcoords;

			texU = new float[count];
			texV = new float[count];
			texW = new float[count];
			texH = new float[count];
			textureDirty = new bool[count];
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
				memcpy(texcoords,oldTexcoords,8*sizeof(GLfloat)*num);
			}

			delete[] oldTexU;
			delete[] oldTexV;
			delete[] oldTexW;
			delete[] oldTexH;
			delete[] oldTextureDirty;
			delete[] oldTexcoords;

			subimagecount = count;
		}
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
		colorUINT = (a << 24 | r << 16 | g << 8 | b);
		if (GL2)
		{
			blendcolor[3] = a / 255.f;
			blendcolor[0] = r / 255.f;
			blendcolor[1] = g / 255.f;
			blendcolor[2] = b / 255.f;
		} // end GL2
		else
		{
			blendcolorGL1[3] = a;
			blendcolorGL1[0] = r;
			blendcolorGL1[1] = g;
			blendcolorGL1[2] = b;

			memcpy(&colorGL1[0],&blendcolorGL1[0],4);
			memcpy(&colorGL1[4],&blendcolorGL1[0],4);
			memcpy(&colorGL1[8],&blendcolorGL1[0],4);
			memcpy(&colorGL1[12],&blendcolorGL1[0],4);
		}
	}

	void Sprite::setColor(unsigned int colour)
	{
		colorUINT = colour;
		if (GL2)
		{
			blendcolor[3] = ((colour & 0xFF000000) >> 24) / 255.f;
			blendcolor[0] = ((colour & 0xFF0000) >> 16) / 255.f;
			blendcolor[1] = ((colour & 0xFF00) >> 8) / 255.f;
			blendcolor[2] = ((colour & 0xFF) >> 0) / 255.f;
		} // end GL2
		else
		{
			blendcolorGL1[3] = ((colour & 0xFF000000) >> 24);
			blendcolorGL1[0] = ((colour & 0xFF0000) >> 16);
			blendcolorGL1[1] = ((colour & 0xFF00) >> 8);
			blendcolorGL1[2] = ((colour & 0xFF) >> 0);

			memcpy(&colorGL1[0],&blendcolorGL1[0],4);
			memcpy(&colorGL1[4],&blendcolorGL1[0],4);
			memcpy(&colorGL1[8],&blendcolorGL1[0],4);
			memcpy(&colorGL1[12],&blendcolorGL1[0],4);			
		}
	}

	unsigned int Sprite::getColor()
	{
		return colorUINT;
	}

	void Sprite::setScaleX(float xx)
	{
		xscale = xx;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getScaleX()
	{
		return xscale;
	}
	void Sprite::setScaleY(float yy)
	{
		yscale = yy;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getScaleY()
	{
		return yscale;
	}
	void Sprite::setScale(float xx, float yy)
	{
		xscale = xx;
		yscale = yy;
		if (!GL2)//if GL1
			vertexDirty = true;
	}

	void Sprite::recalculateVertices()
	{
		if (GL2)
		{
		}//end GL2
		else
		{
			if (GL1_USEMATRICES)
				return;
			GLfloat xxx1, xxx2, xxx3, xxx4, yyy1, yyy2, yyy3, yyy4;
			const GLfloat xpx = 2.f/renderer->getVirtualRenderTarget().internalwidth;
			const GLfloat ypx = 2.f/renderer->getVirtualRenderTarget().internalheight;
			
			xxx1 = -1+(STATICXOFFSET+x-xoffset*xscale)*xpx;		// 0,
			yyy1 = 1-(STATICYOFFSET+y-yoffset*yscale)*ypx;		// 0
			
			xxx2 = -1+(STATICXOFFSET+x+(width-xoffset)*xscale)*xpx;	// 1,
			yyy2 = 1-(STATICYOFFSET+y-yoffset*yscale)*ypx;					// 0
			
			xxx3 = -1+(STATICXOFFSET+x-xoffset*xscale)*xpx;		// 0,
			yyy3 = 1-(STATICYOFFSET+y-(yoffset-height)*yscale)*ypx;	// 1

			xxx4 = -1+(STATICXOFFSET+x+(width-xoffset)*xscale)*xpx;	// 1,
			yyy4 = 1-(STATICYOFFSET+y-(yoffset-height)*yscale)*ypx;	// 1

			float verts[] = {
			xxx1, yyy1,0,
			xxx2, yyy2,0,
			xxx3, yyy3,0,
			xxx4, yyy4,0
			};
	    
			memcpy(vertices,verts,12*sizeof(float));
		}
	};

	void Sprite::recalculateTextureCoords()
	{	    
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
	
	void Sprite::recalculateVBO()
	{
		if (GL2)
		{
			int i = -1;
			int j = -1;

			//NSLog(@"POS %f,%f,%f COL %f,%f,%f,%f TEX %f,%f",vertexData[0 + 9*i],vertexData[1 + 9*i],vertexData[2 + 9*i],vertexData[3 + 9*i],vertexData[4 + 9*i],vertexData[5 + 9*i],vertexData[6 + 9*i],vertexData[7 + 9*i],vertexData[8 + 9*i]);
			
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
		} // end GL2
		else
		{
		}
	}

	float Sprite::getAlpha()
	{
		if (GL2)
		{
			return shaderAlpha;
		} // end GL2
		else
		{
			return 0;
		}
	}
	void Sprite::setAlpha(float alpha5)
	{
		if (GL2)
		{
			shaderAlpha = alpha5;
		} // end GL2
		else
		{
		}
	}

	void Sprite::draw()
	{
		if (static_batching)
		{
			static_batch->pushsprite(x - xoffset*xscale, y - yoffset*yscale, x + (width-xoffset)*xscale, y + (height-yoffset)*yscale, colorUINT, shaderAlpha, texU[subimage], texV[subimage], flipX?-texW[subimage]:texW[subimage], flipY?-texH[subimage]:texH[subimage]);//(flipX)?(texU[subimage]+texW[subimage]):texU[subimage], (flipY)?(texV[subimage]+texH[subimage]):texV[subimage], (flipX)?-texW[subimage]:texW[subimage], (flipY)?-texV[subimage]:texV[subimage]);
			return;
		}
		if (GL2)
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
				printf("color data was dirty!\n");
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
			uniformMatrix[0] = STATICXOFFSET+(int)x-SUPER_XOFFSET;//[0][0]
			uniformMatrix[1] = STATICYOFFSET+(int)y-SUPER_YOFFSET;//[0][1]
			uniformMatrix[2] = (int)xoffset;//[0][2]
			uniformMatrix[3] = (int)yoffset;//[0][3]

			uniformMatrix[4] = xscale;//[1][0]
			uniformMatrix[5] = yscale;//[1][1]
			uniformMatrix[6] = width;//[1][2]
			uniformMatrix[7] = height;//[1][3]

			uniformMatrix[8] = renderer->getVirtualRenderTarget().internalwidth;//[2][0]
			uniformMatrix[9] = renderer->getVirtualRenderTarget().internalheight;//[2][1]
			//uniformMatrix[10] = VALUE;
			uniformMatrix[11] = shaderAlpha;

			uniformMatrix[12] = blendcolor[0];
			uniformMatrix[13] = blendcolor[1];
			uniformMatrix[14] = blendcolor[2];
#if PLATFORM_IOS
			uniformMatrix[15] = [[NSDate date] timeIntervalSince1970]-((int)[[NSDate date] timeIntervalSince1970]);
#else
			uniformMatrix[15] = 1;
#endif
			
			glUniformMatrix4fv(matrixUniformID, 1, GL_FALSE, &uniformMatrix[0]);
			GL::BindVertexArray(vertexArray[subimage]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
		} // end GL2
		else
		{
			//printf("OPEN GL 1???\n");
			if (vertexDirty)
				recalculateVertices();
			if (textureDirty[subimage])
				recalculateTextureCoords();
			glVertexPointer(3, GL_FLOAT, 0, vertices);
			glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[8*subimage]);
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorGL1);

#if !PLATFORM_IOS
 			if (GL1_USEMATRICES)
			{
				glm::mat4x4 VIEW = glm::ortho(0.f,320.f,480.f,0.f);// * glm::translate(glm::mat4(1.0f), glm::vec3(-10.f,-10.f,0)) * glm::scale(glm::mat4(1.0f),glm::vec3(2.f/320.f,2.f/480.f,0.f));
				glm::mat4x4 move = glm::translate(glm::mat4(1.0f), glm::vec3(x - xoffset*xscale,y - yoffset*yscale,0));
				glm::mat4x4 scale = glm::scale(glm::mat4(1.0f),glm::vec3(width*xscale,height*yscale,0.f));//
				glm::mat4x4 ultra = VIEW * move * scale;
				glLoadMatrixf(glm::value_ptr(ultra));
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));
			}
			else
#endif
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}

	void Sprite::setXOffset(float xx)
	{
		xoffset = xx;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getXOffset()
	{
		return xoffset;
	}
	void Sprite::setYOffset(float yy)
	{
		yoffset = yy;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getYOffset()
	{
		return yoffset;
	}
	void Sprite::setOffset(float xx, float yy)
	{
		xoffset = xx;
		yoffset = yy;
		if (!GL2)//if GL1
			vertexDirty = true;
	}

	void Sprite::setRotation(float r)
	{
		rotation = r;
		if (!GL2)//if GL1
			vertexDirty = true;
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
		x = xx;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getX()
	{
		return x;
	}
	void Sprite::setY(float yy)
	{
		y = yy;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getY()
	{
		return y;
	}
	void Sprite::setPosition(float xx, float yy)
	{
		x = (int)xx;
		y = (int)yy;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	void Sprite::setSize(float ww, float hh)
	{
		width = ww;
		height = hh;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	void Sprite::setWidth(float ww)
	{
		width = ww;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getWidth()
	{
		return width;
	}

	void Sprite::setHeight(float hh)
	{
		height = hh;
		if (!GL2)//if GL1
			vertexDirty = true;
	}
	float Sprite::getHeight()
	{
		return height;
	}

	void Sprite::setSubrectPixels(int subimg, Rectanglei rect)
	{
		textureDirty[subimg] = true;
		texU[subimg] = rect.x/(float)texture->getWidth();
		texV[subimg] = rect.y/(float)texture->getHeight();
		texW[subimg] = rect.width/(float)texture->getWidth();
		texH[subimg] = rect.height/(float)texture->getHeight();
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
	
	Rectanglef Sprite::getSubrect(int subimg)
	{
		Rectanglef rect;
		rect.x = texU[subimg];
		rect.y = texV[subimg];
		rect.width = texW[subimg];
		rect.height = texH[subimg];
		return rect;
	}

	void Sprite::setSubrectPixels(int subimg, int xx, int yy, int ww, int hh)
	{
		textureDirty[subimg] = true;
		texU[subimg] = xx/(float)texture->getWidth();
		texV[subimg] = yy/(float)texture->getHeight();
		texW[subimg] = ww/(float)texture->getWidth();
		texH[subimg] = hh/(float)texture->getHeight();
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