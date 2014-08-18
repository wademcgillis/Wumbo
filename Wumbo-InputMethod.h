#ifndef __Wumbo_InputMethod_h__
#define __Wumbo_InputMethod_h__
#include "Wumbo-Header.h"

namespace Wumbo
{
	class InputMethod
	{
	protected:
		bool initialized;
	public:
		InputMethod();
		~InputMethod();
		virtual void update();
		virtual void initialize();
		virtual void deinitialize();
		virtual bool isInitialized();
	};
};
#endif