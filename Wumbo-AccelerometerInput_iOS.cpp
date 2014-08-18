//
//  Wumbo-AccelerometerInput_iOS.cpp
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "Wumbo-AccelerometerInput_iOS.h"
#if PLATFORM_IOS
#include "Wumbo-iOSRoot.h"
#import <CoreMotion/CoreMotion.h>
namespace Wumbo
{
	void AccelerometerInput_iOS::initialize()
	{
		Wumbo::InputMethod::initialize();
	}
	void AccelerometerInput_iOS::deinitialize()
	{
		Wumbo::InputMethod::deinitialize();
	}
	void AccelerometerInput_iOS::update()
	{
		if (initialized)
		{
			CMMotionManager *m = [(AppDelegate*)[[UIApplication sharedApplication] delegate] motionManager];
			x = [[m accelerometerData] acceleration].x;
			y = [[m accelerometerData] acceleration].y;
			z = [[m accelerometerData] acceleration].z;
		}
	}
	void AccelerometerInput_iOS::onEndOfFrame()
	{
	}
	
	AccelerometerInput_iOS::AccelerometerInput_iOS()
	{
	}
	float AccelerometerInput_iOS::X()
	{
		return x;
	}
	float AccelerometerInput_iOS::Y()
	{
		return y;
	}
	float AccelerometerInput_iOS::Z()
	{
		return z;
	}
};
#endif