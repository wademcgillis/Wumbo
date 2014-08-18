//
//  Wumbo-AccelerometerInput_iOS.h
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_AccelerometerInput_iOS_h__
#define __Wumbo_AccelerometerInput_iOS_h__
#include "Wumbo-AccelerometerInput.h"
#if PLATFORM_IOS
namespace Wumbo
{
	class AccelerometerInput_iOS : public AccelerometerInput
	{
	private:
		float x, y, z;
	public:
		void initialize();
		void deinitialize();
		void update();
		void onEndOfFrame();
		
		AccelerometerInput_iOS();
		float X();
		float Y();
		float Z();
	};
};
#endif
#endif /* defined(__Wumbo__Wumbo_AccelerometerInput_iOS__) */
