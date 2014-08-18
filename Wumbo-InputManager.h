#ifndef __Wumbo_InputManager_h__
#define __Wumbo_InputManager_h__
#include "Wumbo-Header.h"

namespace Wumbo
{
	class KeyboardInput;
	class TouchInput;
	class AccelerometerInput;
	class MouseInput;
	class Xbox360Input;
	class iCadeInput;
	class Program;

	class InputManager
	{
	private:
		Program *program;
		//
		KeyboardInput *keyboard;
		Xbox360Input *xbox360;
		TouchInput *touch;
		AccelerometerInput *accelerometer;
		MouseInput *mouse;
		iCadeInput *icade;
	public:
		InputManager(Program *prog);
		void initialize();
		void deinitialize();
		void update();
		
		Program *getProgram();

		KeyboardInput *getKeyboardState();
		void setReceiveKeyboardUpdates(bool set);
		bool getReceiveKeyboardUpdates();

		Xbox360Input *getXbox360State();
		void setReceiveXbox360Updates(bool set);
		bool getReceiveXbox360Updates();

		TouchInput *getTouchState();
		void setReceiveTouchUpdates(bool set);
		bool getReceiveTouchUpdates();

		AccelerometerInput *getAccelerometerState();
		void setReceiveAccelerometerUpdates(bool set);
		bool getReceiveAccelerometerUpdates();

		MouseInput *getMouseState();
		void setReceiveMouseUpdates(bool set);
		bool getReceiveMouseUpdates();
		
		iCadeInput *getiCadeState();
		void setReceiveiCadeUpdates(bool set);
		bool getReceiveiCadeUpdates();
	};
};
#endif