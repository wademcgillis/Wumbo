//
//  Wumbo-iCadeInput.h
//  MORTON
//
//  Created by Wade McGillis on 7/9/14.
//  Copyright (c) 2014 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_iCadeInput_h__
#define __Wumbo_iCadeInput_h__
#include "Wumbo-InputMethod.h"

namespace Wumbo
{
	
	
	const unsigned char ICADE_RIGHT = 0;
	const unsigned char ICADE_UP = 1;
	const unsigned char ICADE_LEFT = 2;
	const unsigned char ICADE_DOWN = 3;
	
	const unsigned char ICADE_B1 = 4;
	const unsigned char ICADE_B2 = 5;
	const unsigned char ICADE_B3 = 6;
	const unsigned char ICADE_B4 = 7;
	
	const unsigned char ICADE_B5 = 8;
	const unsigned char ICADE_B6 = 9;
	const unsigned char ICADE_B7 = 10;
	const unsigned char ICADE_B8 = 11;
	
	const unsigned char ICADE_INPUT_COUNT = 12;
	class iCadeInput : public InputMethod
	{
	private:
		bool down[ICADE_INPUT_COUNT+1];
		bool pressed[ICADE_INPUT_COUNT+1];
		bool released[ICADE_INPUT_COUNT+1];
	public:
		virtual void initialize();
		virtual void deinitialize();
		virtual void update();
		virtual void onEndOfFrame();
		
		iCadeInput();
		
		bool isDown(unsigned char key);
		bool isPressed(unsigned char key);
		bool isReleased(unsigned char key);
		
		void ios_add(const char *addition, unsigned int length);
	};
};

#endif /* defined(__Wumbo_iCadeInput_h__) */
