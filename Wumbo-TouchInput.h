//
//  Wumbo-TouchInput.h
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_TouchInput_h__
#define __Wumbo_TouchInput_h__
#include "Wumbo-InputMethod.h"
#include "Wumbo-InputManager.h"
#include "Wumbo-Header.h"
#include "Wumbo-Types.h"
namespace Wumbo
{
	class TouchInput : public InputMethod
	{
	protected:
		InputManager *manager;
	public:
		static const int SWIPE_LEFT;
		static const int SWIPE_UP;
		static const int SWIPE_RIGHT;
		static const int SWIPE_DOWN;
		virtual void initialize();
		virtual void deinitialize();
		virtual void update();
		virtual void onEndOfFrame();
		
		TouchInput();
		TouchInput(InputManager *man);
		virtual void clearTouches();
		virtual bool swiped(int direction);
		virtual bool doubleTapped();
		virtual bool doubleTapped(int x, int y, int x2, int y2);
		virtual bool doubleTappedArea(int x, int y, int w, int h);
		virtual bool tapped();
		virtual bool tapped(int x, int y, int x2, int y2);
		virtual bool tappedArea(int x, int y, int w, int h);
		virtual bool touched();
		virtual bool touched(int x, int y, int x2, int y2);
		virtual bool touchedArea(int x, int y, int w, int h);
		
		virtual unsigned int rawtouchCount();
		virtual Wumbo::vec2di rawtouchLocation(unsigned int index);
	};
};

#endif /* defined(__Wumbo__Wumbo_TouchInput__) */
