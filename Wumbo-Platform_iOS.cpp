//
//  Platform_iOS.cpp
//  Wumbo
//
//  Created by Wade McGillis on 11/25/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "Wumbo-Platform_iOS.h"
#include "Wumbo-Header.h"
#include "Wumbo-Program.h"
namespace Wumbo
{
	namespace Platform_iOS
	{
		bool __ipad;
		bool __fourinch;
		Program *__wumboprogram;
		bool __forcekeyboard;
		bool __keyboardvisible;
		
		void INITIALIZE_VALUES(Program *program)
		{
#if PLATFORM_IOS
			__forcekeyboard = false;
			__keyboardvisible = false;
			__ipad = ([[UIDevice currentDevice].model rangeOfString:@"iPad"].location != NSNotFound);
			__fourinch = ([UIScreen mainScreen].bounds.size.height==568);
			__wumboprogram = program;
			[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
#endif
		}
		
		bool iPad()
		{
			return __ipad;
		}
		
		bool FourInch()
		{
			return __fourinch;
		}
		
		UIDeviceOrientation getDeviceOrientation()
		{
#if PLATFORM_IOS
			return (UIDeviceOrientation)[__wumboprogram->getGLKViewController() interfaceOrientation];
#else
			return UIDeviceOrientationUnknown;
#endif
		}
		
		void showKeyboard(bool force)
		{
#if PLATFORM_IOS
			__forcekeyboard = force;
			[((ViewController*)((AppDelegate*)__wumboprogram->getAppDelegate()).window.rootViewController) showKeyboard];
#endif
		}
		void hideKeyboard()
		{
#if PLATFORM_IOS
			__forcekeyboard = false;
			[((ViewController*)((AppDelegate*)__wumboprogram->getAppDelegate()).window.rootViewController) hideKeyboard];
#endif
		}
		
		bool isKeyboardVisible()
		{
			return __keyboardvisible;
		}
	};
};