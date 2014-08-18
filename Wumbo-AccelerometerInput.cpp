//
//  Wumbo-AccelerometerInput.cpp
//  Wumbo
//
//  Created by Wade McGillis on 10/28/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "Wumbo-AccelerometerInput.h"

namespace Wumbo
{
	void AccelerometerInput::initialize()
	{
		Wumbo::InputMethod::initialize();
	}
	void AccelerometerInput::deinitialize()
	{
		Wumbo::InputMethod::deinitialize();
	}
	void AccelerometerInput::update()
	{
	}
	void AccelerometerInput::onEndOfFrame()
	{
	}
	
	AccelerometerInput::AccelerometerInput()
	{
	}
	float AccelerometerInput::X()
	{
		return 0.f;
	}
	float AccelerometerInput::Y()
	{
		return 0.f;
	}
	float AccelerometerInput::Z()
	{
		return 0.f;
	}
};