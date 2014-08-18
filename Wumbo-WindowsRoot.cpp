#include "Wumbo-Header.h"
#if PLATFORM_WINDOWS || PLATFORM_OSX

#if PLATFORM_WINDOWS
#include <GL/glew.h>
#include <windows.h>
#include <gl/gl.h>
#endif
#if PLATFORM_OSX
#include <OpenGL/gl.h>
#endif

#include "Wumbo-WindowsRoot.h"
#include "Wumbo-Program.h"
#include "Wumbo-InputManager.h"
#include "Wumbo-Scene.h"
#include "Wumbo-KeyboardInput.h"
#include "Wumbo-MouseInput.h"
#include "Wumbo-Renderer.h"
#include "Wumbo-VirtualRenderTarget.h"
#include "Wumbo-Functions.h"

void TFDEBUG(std::string str)
{
}

extern std::string MULDERRESOURCE(std::string str);

/*HWND my_window;
LRESULT CALLBACK WndProc(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam)
{
    switch (Message)
    {
        // Quit when we close the main window
	case WM_CREATE:

		break;
    case WM_CLOSE:
		//CloseWindow(my_window);
		//ShowWindow(my_window,SW_HIDE);
        //PostQuitMessage(0);
        //return 0;
		break;
    }
    
    return DefWindowProc(Handle, Message, WParam, LParam);
}*/

void Wumbo::WindowsRoot::go(Wumbo::Program *prog)
{
	program = prog;
	program->setSFMLWindow(new sf::Window(sf::VideoMode(32,32,32),"Lemons",sf::Style::Close,sf::ContextSettings(0,0,0,1,1)));
/*
	HWND window32 = program->getSFMLWindow()->getSystemHandle();
	//program->getSFMLWindow()->setV
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(window32, GWL_HINSTANCE);

WNDCLASSEX wc;
 wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "Home";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);


	my_window = CreateWindowEx(WS_EX_CLIENTEDGE,
        "Home",
        "The Home page",
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 400,
        NULL, NULL, hInstance, NULL);

ShowWindow(my_window,SW_NORMAL);
UpdateWindow(my_window);
	HWND btn = CreateWindowW(L"BUTTON",L"OK",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,10,10,100,100,my_window,NULL,(HINSTANCE)GetWindowLong(my_window, GWL_HINSTANCE),NULL);
	HWND text*/
	
	//Sleep(1000);
	//return;
	//ShowWindow(btn,SW_SHOW);
	//glClearColor(0.f,1.f,0.f,1.f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glFlush();
	/*sf::Texture tex;
	tex.loadFromFile(MULDERRESOURCE("load.png"));
	sf::Sprite spr(tex);
	spr.setScale(1,1);
	program->getSFMLWindow()->draw(spr);
	program->getSFMLWindow()->display();*/
	//program->getSFMLWindow()->ce
#if PLATFORM_WINDOWS
	char *vers = (char*)glGetString(GL_VERSION);
	printf("%s\n",vers);
	if (atof(vers) >= 2.0f)
		glewInit();
#endif
	 
	glViewport(0,0,program->getSFMLWindow()->getSize().x,program->getSFMLWindow()->getSize().y);
	glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ZERO);

	

	program->begin();

	program->getInputManager()->update();
	program->update();
	program->getRenderer()->resetVirtualRenderTarget();
	program->render();
	program->getSFMLWindow()->display();
	program->getSFMLWindow()->setKeyRepeatEnabled(false);

	clock_t timeSinceLastFrame = clock();

	bool loop = true;
	//program->getSFMLWindow()->clear();
	//MSG Message;
	//Message.message = ~WM_QUIT;
	while(loop)
	{
		/*if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			// If a message was waiting in the message queue, process it
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else*/
		{
			if (program->getRenderer() == NULL)
			{
				FATALMSG("Program has no Renderer! ... Dying.")
				loop = false;
				break;
			}

			sf::Event event;
			while(program->getSFMLWindow()->pollEvent(event))
			{
				if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
				{
					if (program->getInputManager() != NULL)
						if (program->getInputManager()->getReceiveKeyboardUpdates())
							program->getInputManager()->getKeyboardState()->handleEvent(event);
				}
				if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved)
				{
					//printf("EVENT IS A MOUSE EVENT\n");
					if (program->getInputManager()->getReceiveMouseUpdates())
					{
						//printf("WE HAVE MOUSE HANDLER\n");
						program->getInputManager()->getMouseState()->handleEvent(event);
					}
				}
				/*if (event.type == sf::Event::Resized)
				{
					program->getRenderer()->forceDisplaySize(event.size.width,event.size.height);
				}*/
				if (event.type == sf::Event::Closed)
					program->getSFMLWindow()->close();
			}
			if (!program->getSFMLWindow()->isOpen())
			{
				if (program->getScene() != NULL)
					program->getScene()->end();
				loop = false;
				break;
			}
			program->getInputManager()->update();
			program->update();

			program->getRenderer()->resetVirtualRenderTarget();
			program->render();
			program->getSFMLWindow()->display();
			if (program->getInputManager() != NULL)
			{
				if (program->getInputManager()->getReceiveKeyboardUpdates())
					program->getInputManager()->getKeyboardState()->onEndOfFrame();
				if (program->getInputManager()->getReceiveMouseUpdates())
					program->getInputManager()->getMouseState()->onEndOfFrame();
			}
		}
	}
}
#endif