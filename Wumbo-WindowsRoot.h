#ifndef __Wumbo_WindowsRoot_h__
#define __Wumbo_WindowsRoot_h__

namespace Wumbo
{
	class Program;

	class WindowsRoot
	{
	private:
		Program *program;
	public:
		void go(Program *prog);
	};
};

#endif