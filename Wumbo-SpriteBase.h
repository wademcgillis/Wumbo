#ifndef __Wumbo_SpriteBase_h__
#define __Wumbo_SpriteBase_h__
#include "Wumbo-Renderer.h"
#include "Wumbo-Texture.h"
#include "Wumbo-Hitbox.h"

namespace Wumbo
{
	class SpriteBase
	{
	public:
		SpriteBase(Renderer *renderR);
		~SpriteBase();

		virtual void draw();

		virtual void setSubimageCount(unsigned int count);
		virtual unsigned int getSubimageCount();

		virtual void setActiveSubimage(int sub);
		virtual int getActiveSubimage();

		virtual bool dirty(unsigned int kind);

		virtual void recalculate(unsigned int kind);
		
		virtual void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
		virtual void setColor(unsigned int colour);
		virtual unsigned int getColor();

		virtual void setScaleX(float xx);
		virtual float getScaleX();
		virtual void setScaleY(float yy);
		virtual float getScaleY();
		virtual void setScale(float xx, float yy);

		virtual void setFlipX(bool xx);
		virtual bool getFlipX();
		virtual void setFlipY(bool yy);
		virtual bool getFlipY();

		virtual void setXOffset(float xx);
		virtual float getXOffset();
		virtual void setYOffset(float yy);
		virtual float getYOffset();
		virtual void setOffset(float xx, float yy);

		virtual void setRotation(float r);
		virtual float getRotation();

		virtual void setX(float xx);
		virtual float getX();
		virtual void setY(float yy);
		virtual float getY();
		virtual void setPosition(float xx, float yy);

		virtual void setSize(float ww, float hh);

		virtual void setWidth(float ww);
		virtual float getWidth();

		virtual void setHeight(float hh);
		virtual float getHeight();

		virtual void setSubrectPixels(int sub, Rectanglei rect);
		virtual Rectanglei getSubrectPixels(int sub);
		virtual void setSubrectPixels(int sub, int xx, int yy, int ww, int hh);
		virtual void setSubrect(int sub, float xx, float yy, float ww, float hh);

		virtual void setTexU(int sub, float uu);
		virtual float getTexU(int sub);
		virtual void setTexV(int sub, float vv);
		virtual float getTexV(int sub);
		virtual void setTexW(int sub, float ww);
		virtual float getTexW(int sub);
		virtual void setTexH(int sub, float hh);
		virtual float getTexH(int sub);

		virtual void setTexture(Texture *tex);
		virtual Texture *getTexture();

		virtual void setBoundingBox(int left, int right, int top, int bottom);
		virtual Hitbox getHitbox();

		virtual void setAlpha(float alpha5);
		virtual float getAlpha();
	};

}
#endif