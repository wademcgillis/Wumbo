#include "Wumbo-SpriteBase.h"
namespace Wumbo
{
	SpriteBase::SpriteBase(Renderer *renderR)
	{
	}
	SpriteBase::~SpriteBase()
	{
	}
	void SpriteBase::draw()
	{
	}

	void SpriteBase::setSubimageCount(unsigned int count)
	{
	}
	unsigned int SpriteBase::getSubimageCount()
	{
		return 0;
	}

	void SpriteBase::setActiveSubimage(int sub)
	{
	}
	int SpriteBase::getActiveSubimage()
	{
		return 0;
	}

	bool SpriteBase::dirty(unsigned int kind)
	{
		return false;
	}

	void SpriteBase::recalculate(unsigned int kind)
	{
	}

	void SpriteBase::setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	{
	}
	void SpriteBase::setColor(unsigned int colour)
	{
	}
	unsigned int SpriteBase::getColor()
	{
		return 0;
	}

	void SpriteBase::setScaleX(float xx)
	{
	}
	float SpriteBase::getScaleX()
	{
		return 0.f;
	}
	void SpriteBase::setScaleY(float yy)
	{
	}
	float SpriteBase::getScaleY()
	{
		return 0.f;
	}
	void SpriteBase::setScale(float xx, float yy)
	{
	}

	void SpriteBase::setFlipX(bool xx)
	{
	}
	bool SpriteBase::getFlipX()
	{
		return false;
	}
	void SpriteBase::setFlipY(bool yy)
	{
	}
	bool SpriteBase::getFlipY()
	{
		return false;
	}

	void SpriteBase::setXOffset(float xx)
	{
	}
	float SpriteBase::getXOffset()
	{
		return 0.f;
	}
	void SpriteBase::setYOffset(float yy)
	{
	}
	float SpriteBase::getYOffset()
	{
		return 0.f;
	}
	void SpriteBase::setOffset(float xx, float yy)
	{
	}

	void SpriteBase::setRotation(float r)
	{
	}
	float SpriteBase::getRotation()
	{
		return 0.f;
	}

	void SpriteBase::setX(float xx)
	{
	}
	float SpriteBase::getX()
	{
		return 0.f;
	}
	void SpriteBase::setY(float yy)
	{
	}
	float SpriteBase::getY()
	{
		return 0.f;
	}
	void SpriteBase::setPosition(float xx, float yy)
	{
	}

	void SpriteBase::setSize(float ww, float hh)
	{
	}

	void SpriteBase::setWidth(float ww)
	{
	}
	float SpriteBase::getWidth()
	{
		return 0.f;
	}

	void SpriteBase::setHeight(float hh)
	{
	}
	float SpriteBase::getHeight()
	{
		return 0.f;
	}

	void SpriteBase::setSubrectPixels(int sub, Rectanglei rect)
	{
	}
	Rectanglei SpriteBase::getSubrectPixels(int sub)
	{
		return Rectanglei(0,0,0,0);
	}
	void SpriteBase::setSubrectPixels(int sub, int xx, int yy, int ww, int hh)
	{
	}
	void SpriteBase::setSubrect(int sub, float xx, float yy, float ww, float hh)
	{
	}

	void SpriteBase::setTexU(int sub, float uu)
	{
	}
	float SpriteBase::getTexU(int sub)
	{
		return 0.f;
	}
	void SpriteBase::setTexV(int sub, float vv)
	{
	}
	float SpriteBase::getTexV(int sub)
	{
		return 0.f;
	}
	void SpriteBase::setTexW(int sub, float ww)
	{
	}
	float SpriteBase::getTexW(int sub)
	{
		return 0.f;
	}
	void SpriteBase::setTexH(int sub, float hh)
	{
	}
	float SpriteBase::getTexH(int sub)
	{
		return 0.f;
	}
	void SpriteBase::setTexture(Texture *tex)
	{
	}
	Texture *SpriteBase::getTexture()
	{
		return NULL;
	}

	void SpriteBase::setBoundingBox(int left, int right, int top, int bottom)
	{
	}
	Hitbox SpriteBase::getHitbox()
	{
		return Hitbox(0,0,0,0);
	}

	void SpriteBase::setAlpha(float alpha5)
	{
	}
	float SpriteBase::getAlpha()
	{
		return 0.f;
	}
};