//
//  Wumbo-TouchInput.cpp
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "Wumbo-TouchInput.h"

namespace Wumbo
{
	const int TouchInput::SWIPE_LEFT = 0;
	const int TouchInput::SWIPE_UP = 1;
	const int TouchInput::SWIPE_RIGHT = 2;
	const int TouchInput::SWIPE_DOWN = 3;
	
	void TouchInput::initialize()
	{
		InputMethod::initialize();
	}
	void TouchInput::deinitialize()
	{
		InputMethod::deinitialize();
	}
	void TouchInput::update()
	{
	}
	void TouchInput::onEndOfFrame()
	{
	}
	TouchInput::TouchInput()
	{
		manager = NULL;
	}
	TouchInput::TouchInput(InputManager *man)
	{
		manager = man;
	}
	bool TouchInput::swiped(int direction)
	{
		return false;
	}
	bool TouchInput::doubleTapped()
	{
		return false;
	}
	bool TouchInput::doubleTapped(int x, int y, int x2, int y2)
	{
		return false;
	}
	bool TouchInput::doubleTappedArea(int x, int y, int w, int h)
	{
		return false;
	}
	bool TouchInput::tapped()
	{
		return false;
	}
	bool TouchInput::tapped(int x, int y, int x2, int y2)
	{
		return false;
	}
	bool TouchInput::tappedArea(int x, int y, int w, int h)
	{
		return false;
	}
	void TouchInput::clearTouches()
	{
	}
	bool TouchInput::touched()
	{
		return false;
	}
	bool TouchInput::touched(int x, int y, int x2, int y2)
	{
		return false;
	}
	bool TouchInput::touchedArea(int x, int y, int w, int h)
	{
		return false;
	}
	unsigned int TouchInput::rawtouchCount()
	{
		return 0;
	}
	Wumbo::vec2di TouchInput::rawtouchLocation(unsigned int index)
	{
		return Wumbo::vec2di(0,0);
	}
};