#include "Wumbo-Header.h"
#if PLATFORM_WINDOWS
#define USECONSOLE

#include "Wumbo-WindowsRoot.h"
#include "Wumbo-Program.h"
#include "Wumbo-Types.h"
#include "Wumbo-Functions.h"
extern Wumbo::Program *WUMBOFRAMEWORK_GETPROGRAM();
#if PLATFORM_WINDOWS
#include <winsock.h>
#include <windows.h>
#include <mmsystem.h>
#include <SFML/OpenGL.hpp>

#pragma comment(lib,"sfml-system-s.lib")
#pragma comment(lib,"sfml-window-s.lib")
//#pragma comment(lib,"sfml-graphics-s.lib")
#pragma comment(lib,"sfml-audio-s.lib")
//#pragma comment(lib,"sfml-network-s.lib")
#ifndef DISABLE_XINPUT
#pragma comment(lib,"xinput.lib")
#endif
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"glew.lib")
#pragma comment(lib, "User32.lib")
#endif


#ifndef USECONSOLE
	#ifdef _WIN32
		int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow)
	#else
		int main()
	#endif
#else
	int main(int argc, char *argv[])
#endif
{







	/*WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	hFind = FindFirstFile("C:/FJORDS/FjordsEditor/*.h", &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return 0;
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			printf("%s\n", ffd.cFileName);
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);




	system("pause");*/
	//return 0;
#if PLATFORM_WINDOWS
	timeBeginPeriod(1);
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
#endif
	Wumbo::WindowsRoot root;
	Wumbo::Program *prog = WUMBOFRAMEWORK_GETPROGRAM();
	root.go(prog);
#if PLATFORM_WINDOWS
	timeEndPeriod(1);
	WSACleanup();
#endif
	return 0;
}
#endif
