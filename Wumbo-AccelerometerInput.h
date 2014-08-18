//
//  Wumbo-AccelerometerInput.h
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __Wumbo_AccelerometerInput_h__
#define __Wumbo_AccelerometerInput_h__
#include "Wumbo-InputMethod.h"

namespace Wumbo
{
	class AccelerometerInput : public InputMethod
	{
	public:
		virtual void initialize();
		virtual void deinitialize();
		virtual void update();
		virtual void onEndOfFrame();
		
		AccelerometerInput();
		virtual float X();
		virtual float Y();
		virtual float Z();
	};
};

#endif /* defined(__Wumbo__Wumbo_AccelerometerInput__) */
