#ifndef __Wumbo_Quicky_h__
#define __Wumbo_Quicky_h__
#include "Wumbo-Program.h"
#include "Wumbo-InputManager.h"
#include "Wumbo-KeyboardInput.h"
#include "Wumbo-iCadeInput.h"
#include "Wumbo-TouchInput.h"
#include "Wumbo-MouseInput.h"
#include "Wumbo-AccelerometerInput.h"
#include "Wumbo-Scene.h"
#include "Wumbo-Sprite.h"


namespace Wumbo
{
	extern Program *__quickyProgram;
	extern Renderer *__quickyRenderer;
	extern KeyboardInput *__quickyKeyboard;
	extern TouchInput *__quickyTouch;
	extern AccelerometerInput *__quickyAccel;
	extern MouseInput *__quickyMouse;
	extern iCadeInput *__quickyiCade;

	void Quicky_SetProgram(Program *program);
	Program *Quicky_GetProgram();

	Renderer *Quicky_GetRenderer();

	void Quicky_Program_SetScene(Wumbo::Scene *scene);

	bool Quicky_Keyboard_CheckPressed(unsigned int key);
	bool Quicky_Keyboard_CheckReleased(unsigned int key);
	bool Quicky_Keyboard_Check(unsigned int key);

	bool Quicky_Touch_Swiped(int direction);
	bool Quicky_Touch_Touched();
	bool Quicky_Touch_Touched(int x, int y, int x2, int y2);

	float Quicky_Accelerometer_X();
	float Quicky_Accelerometer_Y();
	float Quicky_Accelerometer_Z();

	void Quicky_Sprite_Draw(Sprite *sprite, float x, float y, float xscale, float yscale);
};

#endif