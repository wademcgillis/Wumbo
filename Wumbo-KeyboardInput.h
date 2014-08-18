#ifndef __Wumbo_KeyboardInput_h__
#define __Wumbo_KeyboardInput_h__
#include "Wumbo-InputMethod.h"
#if PLATFORM_PC
#include <SFML/Window.hpp>
#else
namespace sf
{
	typedef unsigned int Event;
	namespace Keyboard
	{
		typedef unsigned char Key;
	};
};
#endif

#include <string>

namespace Wumbo
{
	namespace Key
	{
		// all values stolen from http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
		const unsigned int Backspace = 0x08;
		const unsigned int Tab = 0x09;
		const unsigned int Return = 0X0D;
		const unsigned int Shift = 0x10;
		const unsigned int Control = 0x11;
		const unsigned int Alt = 0x12;
		const unsigned int Pause = 0x13;
		const unsigned int Capslock = 0x14;
		const unsigned int Escape = 0x1B;
		const unsigned int Space = 0x20;
		const unsigned int PageUp = 0x21;
		const unsigned int PageDown = 0x22;
		const unsigned int End = 0x23;
		const unsigned int Home = 0x24;
		const unsigned int Left = 0x25;
		const unsigned int Up = 0x26;
		const unsigned int Right = 0x27;
		const unsigned int Down = 0x28;
		//const unsigned int Select = 0x29;
		//const unsigned int Print = 0x2A;
		//const unsigned int Execute = 0x2B;
		//const unsigned int Snapshot = 0x2C;
		const unsigned int Insert = 0x2D;
		const unsigned int Delete = 0x2E;
		//const unsigned int Help = 0x2F;
		const unsigned int Num0 = 0x30;
		const unsigned int Num1 = 0x31;
		const unsigned int Num2 = 0x32;
		const unsigned int Num3 = 0x33;
		const unsigned int Num4 = 0x34;
		const unsigned int Num5 = 0x35;
		const unsigned int Num6 = 0x36;
		const unsigned int Num7 = 0x37;
		const unsigned int Num8 = 0x38;
		const unsigned int Num9 = 0x39;
		const unsigned int A = 0x41;
		const unsigned int B = 0x42;
		const unsigned int C = 0x43;
		const unsigned int D = 0x44;
		const unsigned int E = 0x45;
		const unsigned int F = 0x46;
		const unsigned int G = 0x47;
		const unsigned int H = 0x48;
		const unsigned int I = 0x49;
		const unsigned int J = 0x4A;
		const unsigned int K = 0x4B;
		const unsigned int L = 0x4C;
		const unsigned int M = 0x4D;
		const unsigned int N = 0x4E;
		const unsigned int O = 0x4F;
		const unsigned int P = 0x50;
		const unsigned int Q = 0x51;
		const unsigned int R = 0x52;
		const unsigned int S = 0x53;
		const unsigned int T = 0x54;
		const unsigned int U = 0x55;
		const unsigned int V = 0x56;
		const unsigned int W = 0x57;
		const unsigned int X = 0x58;
		const unsigned int Y = 0x59;
		const unsigned int Z = 0x5A;
		const unsigned int LWindows = 0x5B;
		const unsigned int RWindows = 0x5C;
		const unsigned int Applications = 0x5D;
		const unsigned int Sleep = 0x5F;
		const unsigned int Numpad0 = 0x60;
		const unsigned int Numpad1 = 0x61;
		const unsigned int Numpad2 = 0x62;
		const unsigned int Numpad3 = 0x63;
		const unsigned int Numpad4 = 0x64;
		const unsigned int Numpad5 = 0x65;
		const unsigned int Numpad6 = 0x66;
		const unsigned int Numpad7 = 0x67;
		const unsigned int Numpad8 = 0x68;
		const unsigned int Numpad9 = 0x69;
		const unsigned int Multiply = 0x6A;
		const unsigned int Add = 0x6B;
		const unsigned int Separator = 0x6C;
		const unsigned int Subtract = 0x6D;
		const unsigned int Decimal = 0x6E;
		const unsigned int Divide = 0x6F;
		const unsigned int F1 = 0x70;
		const unsigned int F2 = 0x71;
		const unsigned int F3 = 0x72;
		const unsigned int F4 = 0x73;
		const unsigned int F5 = 0x74;
		const unsigned int F6 = 0x75;
		const unsigned int F7 = 0x76;
		const unsigned int F8 = 0x77;
		const unsigned int F9 = 0x78;
		const unsigned int F10 = 0x79;
		const unsigned int F11 = 0x7A;
		const unsigned int F12 = 0x7B;
		const unsigned int F13 = 0x7C;
		const unsigned int F14 = 0x7D;
		const unsigned int F15 = 0x7E;
		const unsigned int F16 = 0x7F;
		const unsigned int F17 = 0x80;
		const unsigned int F18 = 0x81;
		const unsigned int F19 = 0x82;
		const unsigned int F20 = 0x83;
		const unsigned int F21 = 0x84;
		const unsigned int F22 = 0x85;
		const unsigned int F23 = 0x86;
		const unsigned int F24 = 0x87;
		const unsigned int LShift = 0xA0;
		const unsigned int RShift = 0xA1;
		const unsigned int LControl = 0xA2;
		const unsigned int RControl = 0xA3;
		const unsigned int Semicolon = 0xBA;
		const unsigned int Plus = 0xBB;
		const unsigned int Comma = 0xBC;
		const unsigned int Minus = 0xBD;
		const unsigned int Period = 0xBE;
		const unsigned int Backslash = 0xBF;
		const unsigned int Tilde = 0xC0;
		const unsigned int LBracket = 0xDB;
		const unsigned int Slash = 0xDC;
		const unsigned int RBracket = 0xDD;
		const unsigned int Quote = 0xDE;


		const unsigned int Hyphen = 0x200;
		const unsigned int Equal = 0x100;
	}

	class KeyboardInput : public InputMethod
	{
	private:
		bool keystateDown[256];
		bool keystatePressed[256];
		bool keystateReleased[256];
		int lastKeyPressed;
		int lastCharPressed;

		int gm_lastkey;
		std::string gm_lastchar;
		std::string gm_string;
	public:
		/*Keyboard(InputManager *man);*/
		void initialize();
		void deinitialize();
		void update();
		void onEndOfFrame();
		//
		void clear();
		void handleEvent(sf::Event event);
		bool isKeyPressed(unsigned int key);
		bool isKeyReleased(unsigned int key);
		bool isKeyDown(unsigned int key);

		int getLastKeyPressed();
		int getLastCharPressed();

		std::string gmkeyboard_lastchar();
		int gmkeyboard_lastkey();
		std::string gmkeyboard_string();
		void set_gmkeyboard_string(std::string str);
		
		void iOS_addKeys(const char *keys, unsigned long length);
	};
};
#endif