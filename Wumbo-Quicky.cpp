#include "Wumbo-Quicky.h"

namespace Wumbo
{
	Program *__quickyProgram;
	Renderer *__quickyRenderer;
	KeyboardInput *__quickyKeyboard;
	TouchInput *__quickyTouch;
	AccelerometerInput *__quickyAccel;
	MouseInput *__quickyMouse;
	iCadeInput *__quickyiCade;

	void Quicky_SetProgram(Program *program)
	{
		__quickyProgram = program;
		__quickyRenderer = program->getRenderer();
		__quickyAccel = program->getInputManager()->getAccelerometerState();
		__quickyTouch = program->getInputManager()->getTouchState();
		__quickyKeyboard = program->getInputManager()->getKeyboardState();
		__quickyMouse = program->getInputManager()->getMouseState();
		__quickyiCade = program->getInputManager()->getiCadeState();

	}

	Program *Quicky_GetProgram()
	{
		return __quickyProgram;
	}

	Renderer *Quicky_GetRenderer()
	{
		return __quickyRenderer;
	}

	void Quicky_Program_SetScene(Wumbo::Scene *scene);

	bool Quicky_Keyboard_CheckPressed(unsigned int key)
	{
		return __quickyKeyboard->isKeyPressed(key);
	}
	bool Quicky_Keyboard_CheckReleased(unsigned int key)
	{
		return __quickyKeyboard->isKeyReleased(key);
	}
	bool Quicky_Keyboard_Check(unsigned int key)
	{
		return __quickyKeyboard->isKeyDown(key);
	}

	bool Quicky_Touch_Swiped(int direction);
	bool Quicky_Touch_Touch();
	bool Quicky_Touch_Touch(int x, int y, int x2, int y2);

	float Quicky_Accelerometer_X()
	{
		return 0.f;
	}
	float Quicky_Accelerometer_Y()
	{
		return 0.f;
	}
	float Quicky_Accelerometer_Z()
	{
		return 0.f;
	}

	void Quicky_Sprite_Draw(Sprite *sprite, float x, float y, float xscale, float yscale);
};