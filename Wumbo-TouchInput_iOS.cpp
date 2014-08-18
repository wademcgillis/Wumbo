//
//  Wumbo-TouchInput_iOS.cpp
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//
#include "Wumbo-Header.h"
#if PLATFORM_IOS
#include "Wumbo-TouchInput_iOS.h"
#include "Wumbo-Program.h"
#include "Wumbo-Renderer.h"
#include "Wumbo-iOSRoot.h"
#include <UIKit/UITouch.h>
namespace Wumbo
{

	void TouchInput_iOS::initialize()
	{
		InputMethod::initialize();
	}
	void TouchInput_iOS::deinitialize()
	{
		InputMethod::deinitialize();
	}
	void TouchInput_iOS::update()
	{
		swipe[SWIPE_LEFT] = newSwipe[SWIPE_LEFT];
		swipe[SWIPE_UP] = newSwipe[SWIPE_UP];
		swipe[SWIPE_RIGHT] = newSwipe[SWIPE_RIGHT];
		swipe[SWIPE_DOWN] = newSwipe[SWIPE_DOWN];
		newSwipe[SWIPE_LEFT] = false;
		newSwipe[SWIPE_UP] = false;
		newSwipe[SWIPE_RIGHT] = false;
		newSwipe[SWIPE_DOWN] = false;
		//NSLog(@"SWIPES:\nL %i\nU %i\nR %i\nD %i",swipe[SWIPE_LEFT],swipe[SWIPE_UP],swipe[SWIPE_RIGHT],swipe[SWIPE_DOWN]);
	}
	void TouchInput_iOS::onEndOfFrame()
	{
		TouchInput::onEndOfFrame();
		hasTap = false;
		hasDoubleTap = false;
		//NSLog(@"SWIPES:\nL %i\nU %i\nR %i\nD %i",swipe[SWIPE_LEFT],swipe[SWIPE_UP],swipe[SWIPE_RIGHT],swipe[SWIPE_DOWN]);
	}
	
	TouchInput_iOS::TouchInput_iOS(InputManager *man)
	{
		manager = man;
		/*if (ipad)
			maxcount = 10;
		 else
			maxcount = 5;
		 */
		maxCount = 10;
		clearTouches();
		controller = ((AppDelegate*)[[UIApplication sharedApplication] delegate]).window.rootViewController;
		screenSize = [UIScreen mainScreen].bounds.size;
	}
	void TouchInput_iOS::clearTouches()
	{
		touchCount = 0;
		touches.clear();
	}
	bool TouchInput_iOS::swiped(int direction)
	{
		if (direction >= 0 && direction <= 3)
			return swipe[direction];
		return false;
	}
	void TouchInput_iOS::addDoubleTap(CGPoint location)
	{
		hasDoubleTap = true;
		doubleTapLocation = location;
	}
	bool TouchInput_iOS::doubleTapped()
	{
		return hasDoubleTap;
	}
	bool TouchInput_iOS::doubleTapped(int x, int y, int x2, int y2)
	{
		if (hasDoubleTap)
		{
			int xx = fixTouchForOrientation(doubleTapLocation).x;
			int yy = fixTouchForOrientation(doubleTapLocation).y;
			if (xx >= x)
				if (xx < x2)
					if (yy >= y)
						if (yy < y2)
							return true;
		}
		return false;
	}
	bool TouchInput_iOS::doubleTappedArea(int x, int y, int w, int h)
	{
		if (hasDoubleTap)
		{
			//printf("Is there a tap in (%i,%i) wiwth size %i %i?\n",x,y,w,h);
			int xx = fixTouchForOrientation(doubleTapLocation).x;
			int yy = fixTouchForOrientation(doubleTapLocation).y;
			if (xx >= x)
				if (xx < x+w)
					if (yy >= y)
						if (yy < y+h)
						{
							//printf("\tYES!\n");
							return true;
						}
		}
		return false;
	}
	void TouchInput_iOS::addTap(CGPoint location)
	{
		hasTap = true;
		tapLocation = location;
	}
	bool TouchInput_iOS::tapped()
	{
		if (hasTap)
			return true;
		return false;
	}
	bool TouchInput_iOS::tapped(int x, int y, int x2, int y2)
	{
		if (hasTap)
		{
			int xx = fixTouchForOrientation(tapLocation).x;
			int yy = fixTouchForOrientation(tapLocation).y;
			if (xx >= x)
				if (xx < x2)
					if (yy >= y)
						if (yy < y2)
							return true;
		}
		return false;
	}
	bool TouchInput_iOS::tappedArea(int x, int y, int w, int h)
	{
		if (hasTap)
		{
			//printf("Is there a tap in (%i,%i) wiwth size %i %i?\n",x,y,w,h);
			int xx = fixTouchForOrientation(tapLocation).x;
			int yy = fixTouchForOrientation(tapLocation).y;
			if (xx >= x)
				if (xx < x+w)
					if (yy >= y)
						if (yy < y+h)
						{
							//printf("\tYES!\n");
							return true;
						}
		}
		return false;
	}
	bool TouchInput_iOS::touched()
	{
		return (touches.size() > 0);
	}
	bool TouchInput_iOS::touched(int x, int y, int x2, int y2)
	{
		for(int i=0;i<touches.size();i++)
			if (touches.at(i) != NULL)
			{
				CGPoint p = fixTouchForOrientation([touches.at(i) locationInView:nil]);
				if (p.x >= x)
					if (p.x < x2)
						if (p.y >= y)
							if (p.y < y2)
								return true;
			}
		return false;
	}
	bool TouchInput_iOS::touchedArea(int x, int y, int w, int h)
	{
		for(int i=0;i<touches.size();i++)
			if (touches.at(i) != NULL)
			{
				CGPoint p = fixTouchForOrientation([touches.at(i) locationInView:nil]);
				if (p.x >= x)
					if (p.x < x+w)
						if (p.y >= y)
							if (p.y < y+h)
								return true;
			}
		return false;
	}
	CGPoint TouchInput_iOS::fixTouchForOrientation(CGPoint p)
	{
		CGPoint retval = p;
		switch(controller.interfaceOrientation)
		{
			case UIInterfaceOrientationPortrait:
				//NSLog(@"%f %f",retval.x,retval.y);
				return retval;
				break;
			case UIInterfaceOrientationPortraitUpsideDown:
				retval.x = screenSize.height-p.x;
				retval.y = screenSize.width-p.y;
				//NSLog(@"%f %f",retval.x,retval.y);
				return retval;
				break;
			case UIInterfaceOrientationLandscapeLeft:
				retval.x = screenSize.height-p.y;
				retval.y = p.x;
				//NSLog(@"%f %f",retval.x,retval.y);
				return retval;
				break;
			case UIInterfaceOrientationLandscapeRight:
				retval.x = p.y;
				retval.y = screenSize.width-p.x;
				//NSLog(@"%f %f",retval.x,retval.y);
				return retval;
				break;
			default:
				return retval;
				break;
		}
		return p;
	}

	void TouchInput_iOS::addTouch(UITouch *touch)
	{
		touches.push_back(touch);
		//printf("touchCount: %i\n",(int)touches.size());
	}
	
	void TouchInput_iOS::addSwipe(int direction)
	{
		printf("direction = %i\n",direction);
		if (direction >= 0 && direction <= 3)
			newSwipe[direction] = true;
	}
	
	void TouchInput_iOS::removeTouch(UITouch *touch)
	{
		for(int i=0;i<touches.size();i++)
		{
			if (touches.at(i) == touch)
			{
				touches.erase(touches.begin()+i);
				i--;
			}
		}
		//printf("touchCount: %i\n",(int)touches.size());
	}
	
	unsigned int TouchInput_iOS::rawtouchCount()
	{
		return touches.size();
	}
	Wumbo::vec2di TouchInput_iOS::rawtouchLocation(unsigned int index)
	{
		if (index < touches.size())
			return vec2di(fixTouchForOrientation([touches.at(index) locationInView:nil]).x,fixTouchForOrientation([touches.at(index) locationInView:nil]).y);
		else
			return vec2di(-1000,-1000);
	}
};
#endif