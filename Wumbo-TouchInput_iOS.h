//
//  Wumbo-TouchInput_iOS.h
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_TouchInput_iOS_h__
#define __Wumbo_TouchInput_iOS_h__
#include "Wumbo-TouchInput.h"
#include "Wumbo-Types.h"
#if PLATFORM_IOS
#include <vector>
namespace Wumbo
{
	class TouchInput_iOS : public TouchInput
	{
	private:
		int maxCount;
		int touchCount;
		std::vector<UITouch *> touches;
		bool swipe[4];
		bool newSwipe[4];
		UIViewController *controller;
		CGSize screenSize;
		
		bool hasDoubleTap;
		CGPoint doubleTapLocation;
		
		bool hasTap;
		CGPoint tapLocation;
	public:
		void initialize();
		void deinitialize();
		void update();
		void onEndOfFrame();
		
		TouchInput_iOS(InputManager *man);
		void clearTouches();
		bool swiped(int direction);
		bool touched();
		bool touched(int x, int y, int x2, int y2);
		bool touchedArea(int x, int y, int w, int h);
		
		bool tapped();
		bool tapped(int x, int y, int x2, int y2);
		bool tappedArea(int x, int y, int w, int h);
		
		bool doubleTapped();
		bool doubleTapped(int x, int y, int x2, int y2);
		bool doubleTappedArea(int x, int y, int w, int h);
		
		CGPoint fixTouchForOrientation(CGPoint p);
		void addDoubleTap(CGPoint location);
		void addTap(CGPoint location);
		void addTouch(UITouch *touch);
		void removeTouch(UITouch *touch);
		void addSwipe(int direction);
		
		unsigned int rawtouchCount();
		Wumbo::vec2di rawtouchLocation(unsigned int index);
	};
};
#endif

#endif /* defined(__Wumbo__Wumbo_TouchInput_iOS__) */
