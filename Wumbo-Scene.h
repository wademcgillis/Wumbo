#ifndef __Wumbo_Scene_h__
#define __Wumbo_Scene_h__
#include "Wumbo-Program.h"

namespace Wumbo
{
	class Scene
	{
	protected:
		Program *program;
	public:
		Scene();
		Scene(Program *prog);
		Program *getProgram();
		//
		virtual void begin();
		virtual void update();
		virtual void render();
		virtual void end();
		virtual void onClose();
	};
};
#endif