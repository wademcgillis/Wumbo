#ifndef __Wumbo_iOSRoot_h__
#define __Wumbo_iOSRoot_h__
#include "Wumbo-Header.h"
#if PLATFORM_IOS
//#include <iostream>
#import <UIKit/UIKit.h>
#import <CoreMotion/CoreMotion.h>
#include "GM.h"
#include "Finch/Finch.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
	CMMotionManager *motionManager;
}

@property (readonly) CMMotionManager *motionManager;
@property (strong, nonatomic) UIWindow *window;

@end

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "Wumbo-TouchInput_iOS.h"
@interface ViewController : GLKViewController <UIGestureRecognizerDelegate>//, UIKeyInput>
{
	Wumbo::Program *program;
	bool supportedOrientations[5];
	Wumbo::TouchInput_iOS *toucher;
	UISwipeGestureRecognizer *swipeRecognizerLeft;
	UISwipeGestureRecognizer *swipeRecognizerUp;
	UISwipeGestureRecognizer *swipeRecognizerRight;
	UISwipeGestureRecognizer *swipeRecognizerDown;
	UITapGestureRecognizer *tapRecognizer;
	UITapGestureRecognizer *doubleTapRecognizer;
	Finch *finch;
	unsigned long keyboardStringLength;
	unsigned long keyboardStringLengthPrevious;
	double lastTap1970;
}
- (Wumbo::Program*) getProgram;
- (void)handleSwipe:(UIGestureRecognizer *)gestureRecognizer;
- (void)handleTap:(UIGestureRecognizer *)gestureRecognizer;
- (void)handleDoubleTap:(UIGestureRecognizer *)gestureRecognizer;
- (void)addSupportedOrientation:(UIInterfaceOrientation) orient;
- (void)removeSupportedOrientation:(UIInterfaceOrientation) orient;

- (void) keyboardWasShown;
- (void) keyboardWasHidden;

- (void) showKeyboard;
- (void) hideKeyboard;
@end
#endif
#endif