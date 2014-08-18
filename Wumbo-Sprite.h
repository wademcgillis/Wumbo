#ifndef __Wumbo_Sprite_h__
#define __Wumbo_Sprite_h__
#include "Wumbo-Resource.h"
#include "Wumbo-Texture.h"
#include "Wumbo-Renderer.h"
#include "Wumbo-Types.h"
#include "Wumbo-Hitbox.h"
//#include "Wumbo-SpriteBase.h"
#include "Wumbo-SpriteBatch.h"
#include "Wumbo-Shader.h"
namespace Wumbo
{
	class Sprite// : public SpriteBase
	{
	private:
		static Wumbo::SpriteBatch *static_batch;
		static bool static_batching;
	public:
		static void SetBatcher(Wumbo::SpriteBatch *batcher);
		static Wumbo::SpriteBatch *GetBatcher();
		static void BeginBatch();
		static void EndBatch();
		static void RenderBatch();
	private:
		bool GL2;
		bool GL1_USEMATRICES;
	private:
		// VARS
		GLint matrixUniformID;
		float shaderAlpha;
		float uniformMatrix[4 * 4];
		Rectanglei boundingBox;
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
		GLfloat vertexData[(3 + 4 + 2) * 6];
		GLuint *vertexArray;
		GLuint *vertexBuffer;
		GLfloat vertices[12];
		GLfloat *texcoords;
		GLfloat *color;
		unsigned int colorUINT;
		GLubyte *colorGL1;
		GLfloat blendcolor[4];
		GLubyte blendcolorGL1[4];
		// FUNCS
		void recalculateTextureCoords();
		void recalculateVertices();
		void recalculateVBO();
	public:
		const static unsigned int RECALC_VERTEX;
		const static unsigned int RECALC_TEXTURE;
		const static unsigned int RECALC_VBO;
	public:
		static int STATICXOFFSET;
		static int STATICYOFFSET;
		
		Sprite(Renderer *renderR);
		~Sprite();

		void draw();

		void setSubimageCount(unsigned int count);
		unsigned int getSubimageCount();

		void setActiveSubimage(int sub);
		int getActiveSubimage();

		void recalculate(unsigned int kind);
		
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

		Rectanglei getSubrectPixels(int sub);
		void setSubrectPixels(int sub, Rectanglei rect);
		void setSubrectPixels(int sub, int xx, int yy, int ww, int hh);
		Rectanglef getSubrect(int sub);
		void setSubrect(int sub, Rectanglef rect);
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

		void setAlpha(float alpha5);
		float getAlpha();
	};
};


#endif