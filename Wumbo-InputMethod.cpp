#include "Wumbo-InputMethod.h"
#include <cstdio>

namespace Wumbo
{
	InputMethod::InputMethod()
	{
		initialized = false;
	}
	InputMethod::~InputMethod()
	{
		initialized = false;
	}
	void InputMethod::update()
	{
		printf("InputMethod::update() -- this method must be overwritten in child classes.\n");
	}
	void InputMethod::initialize()
	{
		printf("InputMethod is initialized -- this method can be overwritten in child classes.\n");
		initialized = true;
	}
	void InputMethod::deinitialize()
	{
		printf("InputMethod is NOT initialized -- this method can be overwritten in child classes.\n");
		initialized = false;
	}
	bool InputMethod::isInitialized()
	{
		return initialized;
	}
};