#ifndef __Wumbo_Sprite_h__
#define __Wumbo_Sprite_h__

#include "Wumbo-Resource.h"
#include "Wumbo-Texture.h"
#include "Wumbo-Renderer.h"
#include "Wumbo-Types.h"
#include "Wumbo-Hitbox.h"

namespace Wumbo
{
	class Sprite1 : public Resource
	{
	private:
		void setVertices(GLfloat xxx1, GLfloat yyy1, GLfloat xxx2, GLfloat yyy2, GLfloat xxx3, GLfloat yyy3, GLfloat xxx4, GLfloat yyy4);
	protected:
		int matrixUniformID;
		

		float shaderAlpha;

		float uniformMatrix[4 * 4];
		Rectanglei boundingBox;
		
		//int subimageCount;
		Renderer *renderer;
		float x, y;
		float xscale, yscale;
		float width, height;

		float xoffset, yoffset;

		float rotation;
		
		unsigned int subimage;
		unsigned int subimagecount;

		Texture *texture;
		float *texU, *texV, *texW, *texH;

		bool flipX, flipY;

		bool vertexDirty;
		bool colorDirty;
		bool *textureDirty;
#if USE_VBOS
		
		
#if TRU_VBO
		GLuint indexBuffer;
		GLubyte indices[4];
		GLfloat vertexData[(3 + 4 + 2) * 4];// (x y z) + (r g b a) + (u v)
#else
		GLfloat vertexData[(3 + 4 + 2) * 6];
		
#endif
		GLuint *vertexArray;
		GLuint *vertexBuffer;
#endif//#else
		GLfloat vertices[12];
		GLfloat *texcoords;//[8 * MAX_SUBIMAGES];
		GLfloat *color; // 16
		GLfloat blendcolor[4];
//#endif
		
	public:
		Sprite(Renderer *renderR);
		~Sprite();

		void draw(int dispOffX = 0, int dispOffY = 0);
		
		void setSubimageCount(unsigned int count);
		unsigned int getSubimageCount();

		void setActiveSubimage(int sub);
		int getActiveSubimage();

		bool dirtyTheVertices();
		bool dirtyTheTextureCoords();

		void recalculateTextureCoords();
		void recalculateVerticesWithRotation();//recalculateVerticesWithRotation
		void recalculateVertices();
		void recalculateVBO();
		
		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
		void setColor(unsigned int colour);
		unsigned int getColor();

		void setScaleX(float xx);
		float getScaleX();
		void setScaleY(float yy);
		float getScaleY();
		void setScale(float xx, float yy);

		void setFlipX(bool xx);
		bool getFlipX();
		void setFlipY(bool yy);
		bool getFlipY();

		void setXOffset(float xx);
		float getXOffset();
		void setYOffset(float yy);
		float getYOffset();
		void setOffset(float xx, float yy);

		void setRotation(float r);
		float getRotation();

		void setX(float xx);
		float getX();
		void setY(float yy);
		float getY();
		void setPosition(float xx, float yy);

		void setSize(float ww, float hh);

		void setWidth(float ww);
		float getWidth();

		void setHeight(float hh);
		float getHeight();

		void setSubrectPixels(int sub, Rectanglei rect);
		Rectanglei getSubrectPixels(int sub);
		void setSubrectPixels(int sub, int xx, int yy, int ww, int hh);
		void setSubrect(int sub, float xx, float yy, float ww, float hh);

		void setTexU(int sub, float uu);
		float getTexU(int sub);
		void setTexV(int sub, float vv);
		float getTexV(int sub);
		void setTexW(int sub, float ww);
		float getTexW(int sub);
		void setTexH(int sub, float hh);
		float getTexH(int sub);

		void setTexture(Texture *tex);
		Texture *getTexture();

		void setBoundingBox(int left, int right, int top, int bottom);
		Hitbox getHitbox();

		float getAlpha();
		void setAlpha(float alpha5);
	};
};

#endif