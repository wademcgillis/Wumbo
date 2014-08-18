//
//  Platform_iOS.h
//  Wumbo
//
//  Created by Wade McGillis on 11/25/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_Platform_iOS_h__
#define __Wumbo_Platform_iOS_h__
#include "Wumbo-Header.h"
#if PLATFORM_PC
typedef enum
{
UIDeviceOrientationUnknown,
UIDeviceOrientationPortrait,            // Device oriented vertically, home button on the bottom
UIDeviceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
UIDeviceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
UIDeviceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
UIDeviceOrientationFaceUp,              // Device oriented flat, face up
UIDeviceOrientationFaceDown             // Device oriented flat, face down
} UIDeviceOrientation;

typedef enum
{
	UIInterfaceOrientationUnknown,
	UIInterfaceOrientationPortrait,            // Device oriented vertically, home button on the bottom
	UIInterfaceOrientationPortraitUpsideDown,  // Device oriented vertically, home button on the top
	UIInterfaceOrientationLandscapeLeft,       // Device oriented horizontally, home button on the right
	UIInterfaceOrientationLandscapeRight,      // Device oriented horizontally, home button on the left
	UIInterfaceOrientationFaceUp,              // Device oriented flat, face up
	UIInterfaceOrientationFaceDown             // Device oriented flat, face down
} UIInterfaceOrientation;
#endif

namespace Wumbo
{
	class Program;
	
	namespace Platform_iOS
	{
		extern bool __ipad;
		extern bool __fourinch;
		extern bool __forcekeyboard;
		extern bool __keyboardvisible;
		
		extern void INITIALIZE_VALUES(Program *program);
		extern bool iPad();
		extern bool FourInch();
		
		extern void update();
		
		extern UIDeviceOrientation getDeviceOrientation();
		
		extern void showKeyboard(bool force = false);
		extern void hideKeyboard();
		
		extern bool isKeyboardVisible();
	};
};

#endif /* defined(__Wumbo__Platform_iOS__) */
