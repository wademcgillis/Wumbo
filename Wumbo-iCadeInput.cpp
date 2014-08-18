//
//  Wumbo-iCadeInput.cpp
//  MORTON
//
//  Created by Wade McGillis on 7/9/14.
//  Copyright (c) 2014 Wade McGillis. All rights reserved.
//

#include "Wumbo-iCadeInput.h"

namespace Wumbo
{
	
	const unsigned int ICUP = 0x80; // I SEE YOU PEE! HAHAHA
	const unsigned int ICDOWN = 0x40;
	inline unsigned char char2icade(char c)
	{
		if (c >= 'A' && c <= 'Z')
			c += 'a'-'A';
		if (c < 'a' || c > 'z')
			return ICADE_INPUT_COUNT;
		printf("we got a key! %i\n",c);
		switch (c)
		{
			case 'w':
				return ICDOWN | ICADE_UP;
				break;
			case 'e':
				return ICUP | ICADE_UP;
				break;
			case 'a':
				return ICDOWN | ICADE_LEFT;
				break;
			case 'q':
				return ICUP | ICADE_LEFT;
				break;
			case 'x':
				return ICDOWN | ICADE_DOWN;
				break;
			case 'z':
				return ICUP | ICADE_DOWN;
				break;
			case 'd':
				return ICDOWN | ICADE_RIGHT;
				break;
			case 'c':
				return ICUP | ICADE_RIGHT;
				break;
			case 'h':
				return ICDOWN | ICADE_B1;
				break;
			case 'r':
				return ICUP | ICADE_B1;
				break;
			case 'j':
				return ICDOWN | ICADE_B2;
				break;
			case 'n':
				return ICUP | ICADE_B2;
				break;
			case 'k':
				return ICDOWN | ICADE_B3;
				break;
			case 'p':
				return ICUP | ICADE_B3;
				break;
			case 'l':
				return ICDOWN | ICADE_B4;
				break;
			case 'v':
				return ICUP | ICADE_B4;
				break;
			case 'y':
				return ICDOWN | ICADE_B5;
				break;
			case 't':
				return ICUP | ICADE_B5;
				break;
			case 'u':
				return ICDOWN | ICADE_B6;
				break;
			case 'f':
				return ICUP | ICADE_B6;
				break;
			case 'i':
				return ICDOWN | ICADE_B7;
				break;
			case 'm':
				return ICUP | ICADE_B7;
				break;
			case 'o':
				return ICDOWN | ICADE_B8;
				break;
			case 'g':
				return ICUP | ICADE_B8;
				break;
			default:
				return ICADE_INPUT_COUNT;
		}
	}
	void iCadeInput::initialize()
	{
		for(unsigned int i=0;i<ICADE_INPUT_COUNT;i++)
			down[i] = released[i] = pressed[i] = false;
	}
	void iCadeInput::deinitialize()
	{
		for(unsigned int i=0;i<ICADE_INPUT_COUNT;i++)
			down[i] = released[i] = pressed[i] = false;
	}
	void iCadeInput::update()
	{
	}
	void iCadeInput::onEndOfFrame()
	{
		for(unsigned int i=0;i<ICADE_INPUT_COUNT;i++)
			released[i] = pressed[i] = false;
	}
	
	iCadeInput::iCadeInput()
	{
		for(unsigned int i=0;i<ICADE_INPUT_COUNT;i++)
			down[i] = released[i] = pressed[i] = false;
	}
	
	bool iCadeInput::isDown(unsigned char key)
	{
		if (key < ICADE_INPUT_COUNT)
			return down[key];
		return false;
	}
	bool iCadeInput::isPressed(unsigned char key)
	{
		if (key < ICADE_INPUT_COUNT)
			return pressed[key];
		return false;
	}
	bool iCadeInput::isReleased(unsigned char key)
	{
		if (key < ICADE_INPUT_COUNT)
			return released[key];
		return false;
	}
	
	void iCadeInput::ios_add(const char *addition, unsigned int length)
	{
#if !PLATFORM_IOS
		return;
#else
		printf("we got %u keys\n",length);
		for(unsigned int i=0;i<length;i++)
		{
			unsigned char val = char2icade(addition[i]);
			if (val & 0x80) // released // 0b10000000
			{
				pressed[val & 0x3F] = false;
				released[val & 0x3F] = true;
				down[val & 0x3F] = false;
			}
			if (val & 0x40) // pressed // 0b10000000
			{
				pressed[val & 0x3F] = true;
				released[val & 0x3F] = false;
				down[val & 0x3F] = true;
			}
		}
#endif
	}
};