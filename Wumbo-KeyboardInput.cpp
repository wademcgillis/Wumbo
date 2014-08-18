#include "Wumbo-KeyboardInput.h"

std::string charFromKeyboardKey(unsigned int key)
{
	std::string s = "";
	if (key >= Wumbo::Key::A && key <= Wumbo::Key::Z)
		s.push_back('A'+(key-Wumbo::Key::A));
	else if (key == Wumbo::Key::Space)
		s.push_back(' ');
	else if (key >= Wumbo::Key::Num0 && key <= Wumbo::Key::Num9)
		s.push_back('0'+(key-Wumbo::Key::Num0));
	//printf("Unrecognized character: %c %X\n",key,key);
	return s;
}

namespace Wumbo
{
#if PLATFORM_PC
	unsigned int sfKeyToVirtualKey(sf::Keyboard::Key k)
	{
		switch(k)
		{
#if PLATFORM_WINDOWS
		case sf::Keyboard::Back:
#else
		case sf::Keyboard::BackSpace:
#endif
			return Key::Backspace;
			break;
		case sf::Keyboard::Tab:
			return Key::Tab;
			break;
		case sf::Keyboard::Return:
			return Key::Return;
			break;
		//return Key::Shift; 0x10:
		//return Key::Control; 0x11:
		//return Key::Alt; 0x12:
		case sf::Keyboard::Pause:
			return Key::Pause;
			break;
		//return Key::Capslock; 0x14:
		case sf::Keyboard::Escape:
			return Key::Escape;
			break;
		case sf::Keyboard::Space:
			return Key::Space;
			break;
		case sf::Keyboard::PageDown:
			return Key::PageUp;
			break;
		case sf::Keyboard::PageUp:
			return Key::PageDown;
			break;
		case sf::Keyboard::End:
			return Key::End;
			break;
		case sf::Keyboard::Home:
			return Key::Home;
			break;
		case sf::Keyboard::Left:
			return Key::Left;
			break;
		case sf::Keyboard::Up:
			return Key::Up;
			break;
		case sf::Keyboard::Right:
			return Key::Right;
			break;
		case sf::Keyboard::Down:
			return Key::Down;
			break;
		//return Key::Select; 0x29:
		//return Key::Print; 0x2A:
		//return Key::Execute; 0x2B:
		//return Key::Snapshot; 0x2C:
		case sf::Keyboard::Insert:
			return Key::Insert;
			break;
		case sf::Keyboard::Delete:
			return Key::Delete;
			break;
		//return Key::Help; 0x2F:
		case sf::Keyboard::Num0:
			return Key::Num0;
			break;
		case sf::Keyboard::Num1:
			return Key::Num1;
			break;
		case sf::Keyboard::Num2:
			return Key::Num2;
			break;
		case sf::Keyboard::Num3:
			return Key::Num3;
			break;
		case sf::Keyboard::Num4:
			return Key::Num4;
			break;
		case sf::Keyboard::Num5:
			return Key::Num5;
			break;
		case sf::Keyboard::Num6:
			return Key::Num6;
			break;
		case sf::Keyboard::Num7:
			return Key::Num7;
			break;
		case sf::Keyboard::Num8:
			return Key::Num8;
			break;
		case sf::Keyboard::Num9:
			return Key::Num9;
			break;
		case sf::Keyboard::A:
			return Key::A;
			break;
		case sf::Keyboard::B:
			return Key::B;
			break;
		case sf::Keyboard::C:
			return Key::C;
			break;
		case sf::Keyboard::D:
			return Key::D;
			break;
		case sf::Keyboard::E:
			return Key::E;
			break;
		case sf::Keyboard::F:
			return Key::F;
			break;
		case sf::Keyboard::G:
			return Key::G;
			break;
		case sf::Keyboard::H:
			return Key::H;
			break;
		case sf::Keyboard::I:
			return Key::I;
			break;
		case sf::Keyboard::J:
			return Key::J;
			break;
		case sf::Keyboard::K:
			return Key::K;
			break;
		case sf::Keyboard::L:
			return Key::L;
			break;
		case sf::Keyboard::M:
			return Key::M;
			break;
		case sf::Keyboard::N:
			return Key::N;
			break;
		case sf::Keyboard::O:
			return Key::O;
			break;
		case sf::Keyboard::P:
			return Key::P;
			break;
		case sf::Keyboard::Q:
			return Key::Q;
			break;
		case sf::Keyboard::R:
			return Key::R;
			break;
		case sf::Keyboard::S:
			return Key::S;
			break;
		case sf::Keyboard::T:
			return Key::T;
			break;
		case sf::Keyboard::U:
			return Key::U;
			break;
		case sf::Keyboard::V:
			return Key::V;
			break;
		case sf::Keyboard::W:
			return Key::W;
			break;
		case sf::Keyboard::X:
			return Key::X;
			break;
		case sf::Keyboard::Y:
			return Key::Y;
			break;
		case sf::Keyboard::Z:
			return Key::Z;
			break;
		case sf::Keyboard::LSystem:
			return Key::LWindows;
			break;
		case sf::Keyboard::RSystem:
			return Key::RWindows;
			break;
		//return Key::Applications; 0x5D:
		//return Key::Sleep; 0x5F:
		case sf::Keyboard::Numpad0:
			return Key::Numpad0;
			break;
		case sf::Keyboard::Numpad1:
			return Key::Numpad1;
			break;
		case sf::Keyboard::Numpad2:
			return Key::Numpad2;
			break;
		case sf::Keyboard::Numpad3:
			return Key::Numpad3;
			break;
		case sf::Keyboard::Numpad4:
			return Key::Numpad4;
			break;
		case sf::Keyboard::Numpad5:
			return Key::Numpad5;
			break;
		case sf::Keyboard::Numpad6:
			return Key::Numpad6;
			break;
		case sf::Keyboard::Numpad7:
			return Key::Numpad7;
			break;
		case sf::Keyboard::Numpad8:
			return Key::Numpad8;
			break;
		case sf::Keyboard::Numpad9:
			return Key::Numpad9;
			break;
		case sf::Keyboard::Multiply:
			return Key::Multiply;
			break;
		/*case sf::Keyboard::Add:
			return Key::Add;
			break;
		//return Key::Separator; 0x6C:
		case sf::Keyboard::Subtract:
			return Key::Subtract;
			break;
		//return Key::Decimal; 0x6E:
		//	case sf::Keyboard::Peri:
		//	break;
		case sf::Keyboard::Divide:
			return Key::Divide;
			break;*/
		case sf::Keyboard::F1:
			return Key::F1;	
			break;
		case sf::Keyboard::F2:
			return Key::F2;
			break;
		case sf::Keyboard::F3:
			return Key::F3;
			break;
		case sf::Keyboard::F4:
			return Key::F4;
			break;
		case sf::Keyboard::F5:
			return Key::F5;
			break;
		case sf::Keyboard::F6:
			return Key::F6;	
			break;
		case sf::Keyboard::F7:
			return Key::F7;
			break;
		case sf::Keyboard::F8:
			return Key::F8;
			break;
		case sf::Keyboard::F9:
			return Key::F9;
			break;
		case sf::Keyboard::F10:
			return Key::F10;
			break;
		case sf::Keyboard::F11:
			return Key::F11;
			break;
		case sf::Keyboard::F12:
			return Key::F12;
			break;
		case sf::Keyboard::F13:
			return Key::F13;
			break;
		case sf::Keyboard::F14:
			return Key::F14;
			break;
		case sf::Keyboard::F15:
			return Key::F15;
			break;
		//return Key::F16; 0x7F:
		//return Key::F17; 0x80:
		//return Key::F18; 0x81:
		//return Key::F19; 0x82:
		//return Key::F20; 0x83:
		//return Key::F21; 0x84:
		//return Key::F22; 0x85:
		//return Key::F23; 0x86:
		//return Key::F24; 0x87:
		case sf::Keyboard::LShift:
			return Key::LShift;
			break;
		case sf::Keyboard::RShift:
			return Key::RShift;
			break;
		case sf::Keyboard::LControl:
			return Key::LControl;
			break;
		case sf::Keyboard::RControl:
			return Key::RControl;
			break;
		case sf::Keyboard::SemiColon:
			return Key::Semicolon;	
			break;
		//return Key::Plus;
		case sf::Keyboard::Comma:
			return Key::Comma;
			break;
		//return Key::Minus; 0xBD:
		case sf::Keyboard::Dash:
			return Key::Hyphen;
			break;
		case sf::Keyboard::Equal:
			return Key::Equal;
			break;
		case sf::Keyboard::Period:
			return Key::Period;
			break;
		case sf::Keyboard::Slash:
			return Key::Backslash;
			break;
		case sf::Keyboard::Tilde:
			return Key::Tilde;
			break;
		case sf::Keyboard::LBracket:
			return Key::LBracket;
			break;
		case sf::Keyboard::BackSlash:
			return Key::Slash;
			break;
		case sf::Keyboard::RBracket:
			return Key::RBracket;
			break;
		case sf::Keyboard::Quote:
			return Key::Quote;
			break;
		default:
			return 0;
			break;
		}
	}

	
	sf::Keyboard::Key virtualKeyToSfKey(int vk)
	{
		switch(vk)
		{
		case Key::Backspace:
#if PLATFORM_WINDOWS
			return sf::Keyboard::Back;
#else
			return sf::Keyboard::BackSpace;
#endif
			break;
		case Key::Tab:
			return sf::Keyboard::Tab;
			break;
		case Key::Return:
			return sf::Keyboard::Return;
			break;
		//case Key::Shift: 0x10;
		//case Key::Control: 0x11;
		//case Key::Alt: 0x12;
		case Key::Pause:
			return sf::Keyboard::Pause;
			break;
		//case Key::Capslock: 0x14;
		case Key::Escape:
			return sf::Keyboard::Escape;
			break;
		case Key::Space:
			return sf::Keyboard::Space;
			break;
		case Key::PageUp:
			return sf::Keyboard::PageDown;
			break;
		case Key::PageDown:
			return sf::Keyboard::PageUp;
			break;
		case Key::End:
			return sf::Keyboard::End;
			break;
		case Key::Home:
			return sf::Keyboard::Home;
			break;
		case Key::Left:
			return sf::Keyboard::Left;
			break;
		case Key::Up:
			return sf::Keyboard::Up;
			break;
		case Key::Right:
			return sf::Keyboard::Right;
			break;
		case Key::Down:
			return sf::Keyboard::Down;
			break;
		//case Key::Select: 0x29;
		//case Key::Print: 0x2A;
		//case Key::Execute: 0x2B;
		//case Key::Snapshot: 0x2C;
		case Key::Insert:
			return sf::Keyboard::Insert;
			break;
		case Key::Delete:
			return sf::Keyboard::Delete;
		//case Key::Help: 0x2F;
		case Key::Num0:
			return sf::Keyboard::Num0;
			break;
		case Key::Num1:
			return sf::Keyboard::Num1;
			break;
		case Key::Num2:
			return sf::Keyboard::Num2;
			break;
		case Key::Num3:
			return sf::Keyboard::Num3;
			break;
		case Key::Num4:
			return sf::Keyboard::Num4;
			break;
		case Key::Num5:
			return sf::Keyboard::Num5;
			break;
		case Key::Num6:
			return sf::Keyboard::Num6;
			break;
		case Key::Num7:
			return sf::Keyboard::Num7;
			break;
		case Key::Num8:
			return sf::Keyboard::Num8;
			break;
		case Key::Num9:
			return sf::Keyboard::Num9;
			break;
		case Key::A:
			return sf::Keyboard::A;
			break;
		case Key::B:
			return sf::Keyboard::B;
			break;
		case Key::C:
			return sf::Keyboard::C;
			break;
		case Key::D:
			return sf::Keyboard::D;
			break;
		case Key::E:
			return sf::Keyboard::E;
			break;
		case Key::F:
			return sf::Keyboard::F;
			break;
		case Key::G:
			return sf::Keyboard::G;
			break;
		case Key::H:
			return sf::Keyboard::H;
			break;
		case Key::I:
			return sf::Keyboard::I;
			break;
		case Key::J:
			return sf::Keyboard::J;
			break;
		case Key::K:
			return sf::Keyboard::K;
			break;
		case Key::L:
			return sf::Keyboard::L;
			break;
		case Key::M:
			return sf::Keyboard::M;
			break;
		case Key::N:
			return sf::Keyboard::N;
			break;
		case Key::O:
			return sf::Keyboard::O;
			break;
		case Key::P:
			return sf::Keyboard::P;
			break;
		case Key::Q:
			return sf::Keyboard::Q;
			break;
		case Key::R:
			return sf::Keyboard::R;
			break;
		case Key::S:
			return sf::Keyboard::S;
			break;
		case Key::T:
			return sf::Keyboard::T;
			break;
		case Key::U:
			return sf::Keyboard::U;
			break;
		case Key::V:
			return sf::Keyboard::V;
			break;
		case Key::W:
			return sf::Keyboard::W;
			break;
		case Key::X:
			return sf::Keyboard::X;
			break;
		case Key::Y:
			return sf::Keyboard::Y;
			break;
		case Key::Z:
			return sf::Keyboard::Z;
			break;
		case Key::LWindows:
			return sf::Keyboard::LSystem;
			break;
		case Key::RWindows:
			return sf::Keyboard::RSystem;
			break;
		//case Key::Applications: 0x5D;
		//case Key::Sleep: 0x5F;
		case Key::Numpad0:
			return sf::Keyboard::Numpad0;
			break;
		case Key::Numpad1:
			return sf::Keyboard::Numpad1;
			break;
		case Key::Numpad2:
			return sf::Keyboard::Numpad2;
			break;
		case Key::Numpad3:
			return sf::Keyboard::Numpad3;
			break;
		case Key::Numpad4:
			return sf::Keyboard::Numpad4;
			break;
		case Key::Numpad5:
			return sf::Keyboard::Numpad5;
			break;
		case Key::Numpad6:
			return sf::Keyboard::Numpad6;
			break;
		case Key::Numpad7:
			return sf::Keyboard::Numpad7;
			break;
		case Key::Numpad8:
			return sf::Keyboard::Numpad8;
			break;
		case Key::Numpad9:
			return sf::Keyboard::Numpad9;
			break;
		/*case Key::Multiply:
			return sf::Keyboard::Multiply;
			break;
		case Key::Add:
			return sf::Keyboard::Add;
			break;
		//case Key::Separator: 0x6C;
		case Key::Subtract:
			return sf::Keyboard::Subtract;
			break;*/
		//case Key::Decimal: 0x6E;
		//	return sf::Keyboard::Peri;
		//	break;
		case Key::Divide:
			return sf::Keyboard::Divide;
			break;
		case Key::F1:
			return sf::Keyboard::F1;
			break;
		case Key::F2:
			return sf::Keyboard::F2;
			break;
		case Key::F3:
			return sf::Keyboard::F3;
			break;
		case Key::F4:
			return sf::Keyboard::F4;
			break;
		case Key::F5:
			return sf::Keyboard::F5;
			break;
		case Key::F6:
			return sf::Keyboard::F6;
			break;
		case Key::F7:
			return sf::Keyboard::F7;
			break;
		case Key::F8:
			return sf::Keyboard::F8;
			break;
		case Key::F9:
			return sf::Keyboard::F9;
			break;
		case Key::F10:
			return sf::Keyboard::F10;
			break;
		case Key::F11:
			return sf::Keyboard::F11;
			break;
		case Key::F12:
			return sf::Keyboard::F12;
			break;
		case Key::F13:
			return sf::Keyboard::F13;
			break;
		case Key::F14:
			return sf::Keyboard::F14;
			break;
		case Key::F15:
			return sf::Keyboard::F15;
			break;
		//case Key::F16: 0x7F;
		//case Key::F17: 0x80;
		//case Key::F18: 0x81;
		//case Key::F19: 0x82;
		//case Key::F20: 0x83;
		//case Key::F21: 0x84;
		//case Key::F22: 0x85;
		//case Key::F23: 0x86;
		//case Key::F24: 0x87;
		case Key::LShift:
			return sf::Keyboard::LShift;
			break;
		case Key::RShift:
			return sf::Keyboard::RShift;
			break;
		case Key::LControl:
			return sf::Keyboard::LControl;
			break;
		case Key::RControl:
			return sf::Keyboard::RControl;
			break;
		case Key::Semicolon:
			return sf::Keyboard::SemiColon;
			break;
		//case Key::Plus:
		case Key::Comma:
			return sf::Keyboard::Comma;
			break;
		//case Key::Minus: 0xBD;
		case Key::Hyphen:
			return sf::Keyboard::Dash;
			break;
		case Key::Equal:
			return sf::Keyboard::Equal;
			break;
		case Key::Period:
			return sf::Keyboard::Period;
			break;
		case Key::Backslash:
			return sf::Keyboard::BackSlash;
			break;
		case Key::Tilde:
			return sf::Keyboard::Tilde;
			break;
		case Key::LBracket:
			return sf::Keyboard::LBracket;
			break;
		case Key::Slash:
			return sf::Keyboard::Slash;
			break;
		case Key::RBracket:
			return sf::Keyboard::RBracket;
			break;
		case Key::Quote:
			return sf::Keyboard::Quote;
			break;
		default:
			return sf::Keyboard::KeyCount;
			break;
		}
	}
#endif
	
	unsigned int characterToKey(char c)
	{

		switch(c)
		{
			case ' ':
				return Key::Space;
				break;
			case '0':
				return Key::Num0;
				break;
			case '1':
				return Key::Num1;
				break;
			case '2':
				return Key::Num2;
				break;
			case '3':
				return Key::Num3;
				break;
			case '4':
				return Key::Num4;
				break;
			case '5':
				return Key::Num5;
				break;
			case '6':
				return Key::Num6;
				break;
			case '7':
				return Key::Num7;
				break;
			case '8':
				return Key::Num8;
				break;
			case '9':
				return Key::Num9;
				break;
			case 'A': case 'a':
				return Key::A;
				break;
			case 'B': case 'b':
				return Key::B;
				break;
			case 'C': case 'c':
				return Key::C;
				break;
			case 'D': case 'd':
				return Key::D;
				break;
			case 'E': case 'e':
				return Key::E;
				break;
			case 'F': case 'f':
				return Key::F;
				break;
			case 'G': case 'g':
				return Key::G;
				break;
			case 'H': case 'h':
				return Key::H;
				break;
			case 'I': case 'i':
				return Key::I;
				break;
			case 'J': case 'j':
				return Key::J;
				break;
			case 'K': case 'k':
				return Key::K;
				break;
			case 'L': case 'l':
				return Key::L;
				break;
			case 'M': case 'm':
				return Key::M;
				break;
			case 'N': case 'n':
				return Key::N;
				break;
			case 'O': case 'o':
				return Key::O;
				break;
			case 'P': case 'p':
				return Key::P;
				break;
			case 'Q': case 'q':
				return Key::Q;
				break;
			case 'R': case 'r':
				return Key::R;
				break;
			case 'S': case 's':
				return Key::S;
				break;
			case 'T': case 't':
				return Key::T;
				break;
			case 'U': case 'u':
				return Key::U;
				break;
			case 'V': case 'v':
				return Key::V;
				break;
			case 'W': case 'w':
				return Key::W;
				break;
			case 'X': case 'x':
				return Key::X;
				break;
			case 'Y': case 'y':
				return Key::Y;
				break;
			case 'Z': case 'z':
				return Key::Z;
				break;
			/*case sf::Keyboard::Numpad0:
				return Key::Numpad0;
				break;
			case sf::Keyboard::Numpad1:
				return Key::Numpad1;
				break;
			case sf::Keyboard::Numpad2:
				return Key::Numpad2;
				break;
			case sf::Keyboard::Numpad3:
				return Key::Numpad3;
				break;
			case sf::Keyboard::Numpad4:
				return Key::Numpad4;
				break;
			case sf::Keyboard::Numpad5:
				return Key::Numpad5;
				break;
			case sf::Keyboard::Numpad6:
				return Key::Numpad6;
				break;
			case sf::Keyboard::Numpad7:
				return Key::Numpad7;
				break;
			case sf::Keyboard::Numpad8:
				return Key::Numpad8;
				break;
			case sf::Keyboard::Numpad9:
				return Key::Numpad9;
				break;*/
			/*case '*':
				return Key::Multiply;
				break;
			case '+':
				return Key::Add;
				break;*/
				//return Key::Separator; 0x6C:
			case '=':
				return Key::Equal;
				break;
			case '-':
				return Key::Hyphen;
				break;
				//return Key::Decimal; 0x6E:
				//	case sf::Keyboard::Peri:
				//	break;
			case ';':
				return Key::Semicolon;
				break;
			case ',':
				return Key::Comma;
				break;
			case '.':
				return Key::Period;
				break;
			case '\\':
				return Key::Slash;
				break;
			case '~':
				return Key::Tilde;
				break;
			case '[':
				return Key::LBracket;
				break;
			case '/':
				return Key::Backslash;
				break;
			case ']':
				return Key::RBracket;
				break;
			case '\'':
				return Key::Quote;
				break;
			default:
				return -1;
				break;
		}
	}

	void KeyboardInput::iOS_addKeys(const char *keys, unsigned long length)
	{
		if (keys == 0)
		{
			printf("\t\t\t\t\t---------ENTER WOO---------\n");
			if (length == 13)
			{
				keystatePressed[Key::Return] = keystateDown[Key::Return] = true;
				gm_lastchar = Key::Return;
				gm_lastkey = Key::Return;
			}
			if (length == 9)
			{
				keystatePressed[Key::Backspace] = keystateDown[Key::Backspace] = true;
				gm_lastchar = Key::Backspace;
				gm_lastkey = Key::Backspace;
				
				if (gm_string.length() > 0)
					gm_string.erase(gm_string.begin()+gm_string.size()-1);
			}
			return;
		}
		for(unsigned long i = 0; i < length; i++)
		{
			printf("\t\tcharacter %c pressed.\n",keys[i]);
			unsigned int charkey = characterToKey(keys[i]);
			//keystateDown[charkey] = true;
			keystatePressed[charkey] = true;
			if (i == length-1)
			{
				printf("\t\tlast chararacter is %c\n",keys[i]);
				
				lastCharPressed = keys[i];
				gm_lastchar = keys[i];
				
				lastKeyPressed = charkey;
				gm_lastkey = charkey;
			}
		}
		gm_string += keys;
	}
	
	void KeyboardInput::clear()
	{
		for(int i=0;i<256;i++)
			keystateDown[i] = keystatePressed[i] = keystateReleased[i] = false;
	}
	void KeyboardInput::initialize()
	{
		//sfKeyToVirtualKey(sf::Keyboard::KeyCount);
		Wumbo::InputMethod::initialize();
		for(int i=0;i<256;i++)
			keystateDown[i] = keystatePressed[i] = keystateReleased[i] = false;
	}
	void KeyboardInput::deinitialize()
	{
		Wumbo::InputMethod::deinitialize();
		for(int i=0;i<256;i++)
			keystateDown[i] = keystatePressed[i] = keystateReleased[i] = false;
	}
	void KeyboardInput::update()
	{
		// lol. this method does nothing.
	}
	//
	void KeyboardInput::onEndOfFrame()
	{
		if (!initialized)
			return;
		for(int i=0;i<256;i++)
			keystateReleased[i] = keystatePressed[i] = false;
	}
	int KeyboardInput::getLastKeyPressed()
	{
		if (!initialized)
			return -1;
		return lastKeyPressed;
	}
	int KeyboardInput::getLastCharPressed()
	{
		if (!initialized)
			return -1;
		return lastCharPressed;
	}

#if PLATFORM_IOS
	void KeyboardInput::handleEvent(sf::Event event)
	{
		return;
	}
#else
	void KeyboardInput::handleEvent(sf::Event event)
	{
		if (!initialized)
			return;
		if (event.type == sf::Event::KeyPressed)
		{
			unsigned int VK = sfKeyToVirtualKey(event.key.code);
			keystateDown[VK] = true;
			keystatePressed[VK] = true;

			lastKeyPressed = VK;

			
			bool nope = false;
			bool S = (keystateDown[Key::LShift] || keystateDown[Key::RShift]);
			if (VK >= Key::A && VK <= Key::Z)
				lastCharPressed = S?('A'+(VK-Key::A)):('a'+(VK-Key::A));
			else if (VK >= Key::Num0 && VK <= Key::Num9)
			{
				if (S)
				{
					switch(VK)
					{
					case Key::Num1:
						lastCharPressed = '!';
						break;
					case Key::Num2:
						lastCharPressed = '@';
						break;
					case Key::Num3:
						lastCharPressed = '#';
						break;
					case Key::Num4:
						lastCharPressed = '$';
						break;
					case Key::Num5:
						lastCharPressed = '%';
						break;
					case Key::Num6:
						lastCharPressed = '^';
						break;
					case Key::Num7:
						lastCharPressed = '&';
						break;
					case Key::Num8:
						lastCharPressed = '*';
						break;
					case Key::Num9:
						lastCharPressed = '(';
						break;
					case Key::Num0:
						lastCharPressed = ')';
						break;
					}
				}
				else
					lastCharPressed = '0'+(VK-Key::Num0);
			}
			else if (VK == Key::Space)
				lastCharPressed = ' ';
			else if (VK == Key::Comma)
				lastCharPressed = S?'<':',';
			else if (VK == Key::Period)
				lastCharPressed = S?'>':'.';
			else if (VK == Key::Backslash)
				lastCharPressed = S?'?':'/';
			else if (VK == Key::Hyphen)
				lastCharPressed = S?'_':'-';
			else if (VK == Key::Equal)
				lastCharPressed = S?'+':'=';
			else if (VK == Key::Semicolon)
				lastCharPressed = S?':':';';
			else if (VK == Key::Quote)
				lastCharPressed = S?'\"':'\'';
			else if (VK == Key::LBracket)
				lastCharPressed = S?'{':'[';
			else if (VK == Key::RBracket)
				lastCharPressed = S?'}':']';
			else if (VK == Key::Tilde)
				lastCharPressed = S?'~':'`';
			else 
				nope = true;
			printf("KEY EVENT: 0x%X %i -> 0x%X %i %c\n",event.key.code,event.key.code,VK,VK,lastCharPressed);
			gm_lastkey = lastKeyPressed;
			if (charFromKeyboardKey(lastKeyPressed) != "")
				gm_lastchar = charFromKeyboardKey(lastKeyPressed);
			if (gm_lastkey == Key::Backspace)
			{
				if (gm_string.length() > 0)
					gm_string.erase(gm_string.begin()+gm_string.size()-1);
			}
			else if (!nope)// if (charFromKeyboardKey(gm_lastkey) != "")
			{
				gm_string += lastCharPressed;//gm_lastchar;
			}
			while(gm_string.size() > 1023)
				gm_string.erase(gm_string.begin());
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			unsigned int VK = sfKeyToVirtualKey(event.key.code);
			keystateDown[VK] = false;
			keystateReleased[VK] = true;
		}
	}
#endif
	
	bool KeyboardInput::isKeyPressed(unsigned int key)
	{
		if (!initialized)
			return false;
		if (key > 256)
			return false;
		return keystatePressed[key];
	}
	bool KeyboardInput::isKeyReleased(unsigned int key)
	{
		if (!initialized)
			return false;
		if (key > 256)
			return false;
		return keystateReleased[key];
	}
	bool KeyboardInput::isKeyDown(unsigned int key)
	{
		if (!initialized)
			return false;
		if (key > 256)
			return false;
		return keystateDown[key];
	}

	std::string KeyboardInput::gmkeyboard_lastchar()
	{
		return gm_lastchar;
	}
	int KeyboardInput::gmkeyboard_lastkey()
	{
		return gm_lastkey;
	}
	void KeyboardInput::set_gmkeyboard_string(std::string str)
	{
		gm_string = str;
	}
	std::string KeyboardInput::gmkeyboard_string()
	{
		return gm_string;
	}
};