#include "Wumbo-InputManager.h"
#include "Wumbo-KeyboardInput.h"
#include "Wumbo-Xbox360Input.h"
#include "Wumbo-AccelerometerInput.h"
#include "Wumbo-AccelerometerInput_iOS.h"
#include "Wumbo-TouchInput.h"
#include "Wumbo-TouchInput_iOS.h"
#include "Wumbo-MouseInput.h"
#include "Wumbo-iCadeInput.h"

namespace Wumbo 
{
	InputManager::InputManager(Program *prog)
	{
		program = prog;
		keyboard = NULL;
		xbox360 = NULL;
		mouse = NULL;
		touch = NULL;
		accelerometer = NULL;
		icade = NULL;
	}
	
	Program *InputManager::getProgram()
	{
		return program;
	}

	void InputManager::initialize()
	{
		keyboard = new KeyboardInput();
		setReceiveKeyboardUpdates(false);

#ifndef DISABLE_XINPUT
#if PLATFORM_WINDOWS
		xbox360 = new Xbox360Input_Win32();
#else
		xbox360 = new Xbox360Input();
#endif
#else
		xbox360 = new Xbox360Input();
#endif
		
		setReceiveXbox360Updates(false);
#if PLATFORM_PC
		mouse = new MouseInput();
		setReceiveMouseUpdates(true);
#endif

#if PLATFORM_IOS
		touch = new TouchInput_iOS(this);
#else
		touch = new TouchInput(this);
#endif
		setReceiveTouchUpdates(false);
#if PLATFORM_IOS
		accelerometer = new AccelerometerInput_iOS();
#else
		accelerometer = new AccelerometerInput();
#endif
		setReceiveAccelerometerUpdates(false);
		
		icade = new iCadeInput();
		setReceiveiCadeUpdates(false);
	}

	void InputManager::deinitialize()
	{
		setReceiveKeyboardUpdates(false);
		delete keyboard;
		keyboard = NULL;

		setReceiveXbox360Updates(false);
		delete xbox360;
		xbox360 = NULL;
		
		setReceiveMouseUpdates(false);
		delete mouse;
		mouse = NULL;
		
		setReceiveTouchUpdates(false);
		delete touch;
		touch = NULL;
		
		setReceiveAccelerometerUpdates(false);
		delete accelerometer;
		accelerometer = NULL;
		
		setReceiveiCadeUpdates(false);
		delete icade;
		icade = NULL;
	}

	KeyboardInput *InputManager::getKeyboardState()
	{
		return keyboard;
	}

	Xbox360Input *InputManager::getXbox360State()
	{
		return xbox360;
	}

	TouchInput *InputManager::getTouchState()
	{
		return touch;
	}

	AccelerometerInput *InputManager::getAccelerometerState()
	{
		return accelerometer;
	}

	MouseInput *InputManager::getMouseState()
	{
		return mouse;
	}
	
	iCadeInput *InputManager::getiCadeState()
	{
		return icade;
	}
	
	void InputManager::update()
	{
		if (keyboard->isInitialized())
			keyboard->update();
		if (xbox360->isInitialized())
			xbox360->update();
#if PLATFORM_PC
		if (mouse->isInitialized())
			mouse->update();
#endif
		if (touch->isInitialized())
			touch->update();
		if (accelerometer->isInitialized())
			accelerometer->update();
	}

	void InputManager::setReceiveKeyboardUpdates(bool set)
	{
		printf("BINGO! %i\n",set);
		if (set)
			keyboard->initialize();
		else
			keyboard->deinitialize();
	}

	bool InputManager::getReceiveKeyboardUpdates()
	{
		return keyboard->isInitialized();
	}

	void InputManager::setReceiveXbox360Updates(bool set)
	{
		printf("BONGO! %i\n",set);
		if (set)
			xbox360->initialize();
		else
			xbox360->deinitialize();
	}

	bool InputManager::getReceiveXbox360Updates()
	{
		return xbox360->isInitialized();
	}

	void InputManager::setReceiveMouseUpdates(bool set)
	{
		if (mouse == NULL)
			return;
		if (set)
			mouse->initialize();
		else
			mouse->deinitialize();
	}

	bool InputManager::getReceiveMouseUpdates()
	{
		return mouse->isInitialized();
	}

	void InputManager::setReceiveTouchUpdates(bool set)
	{
		if (set)
			touch->initialize();
		else
			touch->deinitialize();
	}

	bool InputManager::getReceiveTouchUpdates()
	{
		return touch->isInitialized();
	}

	void InputManager::setReceiveAccelerometerUpdates(bool set)
	{
		if (set)
			accelerometer->initialize();
		else
			accelerometer->deinitialize();
	}

	bool InputManager::getReceiveAccelerometerUpdates()
	{
		return accelerometer->isInitialized();
	}
	
	void InputManager::setReceiveiCadeUpdates(bool set)
	{
		if (set)
			icade->initialize();
		else
			icade->deinitialize();
	}
	
	bool InputManager::getReceiveiCadeUpdates()
	{
		return icade->isInitialized();
	}


};