//
//  Wumbo-Main-OSX.cpp
//  FjordsOSX
//
//  Created by Wade McGillis on 5/21/14.
//  Copyright (c) 2014 Wade McGillis. All rights reserved.
//

#include "Wumbo-Header.h"
#if PLATFORM_OSX
#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
    return NSApplicationMain(argc, (const char **)argv);
}

#import <Cocoa/Cocoa.h>
#import <SFML/Graphics.hpp>
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
#include "Wumbo-Quicky.h"
extern Wumbo::Program *WUMBOFRAMEWORK_GETPROGRAM();


@interface WumboAppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
{
	Wumbo::Program *program;
	sf::Window *_sfmlwindow;
	NSWindow        *window;
	NSView          *sfmlView;
	NSTimer         *renderTimer;
	BOOL             visible;
	BOOL             initialized;
	NSView *IBsfmlView;
}


@property (retain) IBOutlet NSView *IBsfmlView;
-(void)startFullscreen;
-(void)windowDidEnterFullScreen:(NSNotification*)notification;
-(void)windowWillEnterFullScreen:(NSNotification*)notification;
-(void)windowWillExitFullScreen:(NSNotification*)notification;
-(void)windowDidExitFullScreen:(NSNotification*)notification;
-(void)windowShouldClose:(NSNotification*)notification;
@end
/*
 * This interface is used to prevent the system alert produced when the SFML view
 * has the focus and the user press a key.
 */
@interface SilentWindow : NSWindow
-(void)keyDown:(NSEvent *)theEvent;
@end

// Private stuff
@interface WumboAppDelegate ()

-(void)renderMainWindow:(NSTimer *)aTimer;
@end


WumboAppDelegate *OSX_WUMBO_DELEGATE = nil;

void WUMBO_OSX_FORCE_FULLSCREEN(bool full)
{
	[OSX_WUMBO_DELEGATE startFullscreen];
}


// Finally, the implementation
@implementation WumboAppDelegate
@synthesize IBsfmlView;

- (id)init {
    self = [super init];
    if (self) {
        initialized = NO;
    }
    return self;
}

-(void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    if (!initialized)
    {
		OSX_WUMBO_DELEGATE = self;
		NSLog(@"FACE BART\n");
		_sfmlwindow = new sf::Window(IBsfmlView,sf::ContextSettings(24));
        // Init the SFML render area.
        visible = YES;
        
        // Launch the timer to periodically display our stuff into the Cocoa view.
        renderTimer = [NSTimer timerWithTimeInterval:1.f/30.f
                                                   target:self
                                                 selector:@selector(renderMainWindow:)
                                                 userInfo:nil
                                                  repeats:YES];
        [[NSRunLoop mainRunLoop] addTimer:renderTimer
                                  forMode:NSDefaultRunLoopMode];
        [[NSRunLoop mainRunLoop] addTimer:renderTimer
                                  forMode:NSEventTrackingRunLoopMode];
        /*
         * This is really some ugly code but in order to have the timer fired
         * periodically we need to add it to the two above runloop mode.
         *
         * The default mode allows timer firing while the user doesn't do anything
         * while the second mode allows timer firing while he is using a slider
         * or a menu.
         */
        program = WUMBOFRAMEWORK_GETPROGRAM();
		program->setSFMLWindow(_sfmlwindow);
		glViewport(0,0,program->getSFMLWindow()->getSize().x,program->getSFMLWindow()->getSize().y);
		glEnable(GL_SCISSOR_TEST);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		//glDepthFunc(GL_LEQUAL);
		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ZERO);
		
		program->getSFMLWindow()->setKeyRepeatEnabled(false);
		
		program->begin();
		
		[window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
		[window setDelegate:self];
		
        initialized = YES;
    }
}

-(void)dealloc
{
	NSLog(@"SHOOG ZOODS\n");
    [renderTimer invalidate];
	renderTimer = nil;
    
    [super dealloc];
}
extern bool TEST_FULLSCREEN;
-(void)windowShouldClose:(NSNotification*)notification
{
	exit(0);
}
-(void)startFullscreen
{
	[window toggleFullScreen:self];
	//[[window contentView] enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
	//[window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
}
-(void)windowDidEnterFullScreen:(NSNotification*)notification
{
	NSLog(@"windowDidEnterFullScreen:");
	Wumbo::Renderer::setFullscreen(true);
	int w = sf::VideoMode::getDesktopMode().width;
	int h = sf::VideoMode::getDesktopMode().height;
	[window setFrame:[[NSScreen mainScreen] frame] display:NO];
	NSRect test = [[NSScreen mainScreen] frame];
	[window.contentView setFrame:test];
	//test.origin.y += test.size.height/2;
	//test.size.height = 512;
	[[[[window contentView] subviews] firstObject] setFrame:test];
	NSLog(@"Set size to %i, %i",w,h);
	[window setFrame:[[NSScreen mainScreen] frame] display:YES];
	program->getRenderer()->setDisplaySize(w,h);
}
-(void)windowWillEnterFullScreen:(NSNotification*)notification
{
	NSLog(@"windowWillEnterFullScreen:");
}
-(void)windowWillExitFullScreen:(NSNotification*)notification
{
	NSLog(@"windowWillExitFullScreen:");
}
-(void)windowDidExitFullScreen:(NSNotification*)notification
{
	NSLog(@"windowDidExitFullScreen:");
	Wumbo::Renderer::setFullscreen(false);
	int Fw = sf::VideoMode::getDesktopMode().width;
	int Fh = sf::VideoMode::getDesktopMode().height;
	int w = 320;
	int h = 480;
	NSLog(@"Set size to %i, %i",w,h);
	NSRect rect = [window frameRectForContentRect:NSMakeRect(0, 0, w, h)];
	rect.origin.x = (Fw-rect.size.width)/2;
	rect.origin.y = (Fh-rect.size.height)/2;
	[window setFrame:rect display:YES];
	[window.contentView setFrame:NSMakeRect(0, 0, w, h)];
	[[[[window contentView] subviews] firstObject] setFrame:NSMakeRect(0, 0, w, h)];
	program->getRenderer()->setDisplaySize(w,h);
}
/*
 -(void)windowDidEnterFullScreen:(NSNotification*)notification
 {
 NSLog(@"windowDidEnterFullScreen:");
 TEST_FULLSCREEN = true;
 NSRect rect = [[NSScreen mainScreen] frame];
 
 [window.contentView setFrame:rect];
 _sfmlwindow->setSize(sf::Vector2<unsigned int>(rect.size.width,rect.size.height));
 [window setFrame:rect display:YES];
 }
 -(void)windowWillEnterFullScreen:(NSNotification*)notification
 {
 NSLog(@"windowWillEnterFullScreen:");
 }
 -(void)windowWillExitFullScreen:(NSNotification*)notification
 {
 NSLog(@"windowWillExitFullScreen:");
 }
 -(void)windowDidExitFullScreen:(NSNotification*)notification
 {
 NSLog(@"windowDidExitFullScreen:");
 TEST_FULLSCREEN = false;
 int Fw = [[NSScreen mainScreen] frame].size.width;
 int Fh = [[NSScreen mainScreen] frame].size.height;
 NSRect rect = [window frameRectForContentRect:NSMakeRect(0, 0, 320, 480)];
 rect.origin.x = (Fw-rect.size.width)/2;
 rect.origin.y = (Fh-rect.size.height)/2;
 
 _sfmlwindow->setSize(sf::Vector2<unsigned int>(rect.size.width,rect.size.height));
 [window.contentView setFrame:rect];
 [window setFrame:rect display:YES];
 }
 */

bool firstRun = true;
bool secondRun = true;
/*#include "Wumbo-Shader.h"
#include "GlobalFunctions.h"
#include "Wumbo-Texture.h"
extern Wumbo::Shader *megaShader;
extern Wumbo::Texture *texOdin;*/
-(void)renderMainWindow:(NSTimer *)aTimer
{
	if (secondRun)
	{
		if (firstRun)
		{
			program->getRenderer()->resetVirtualRenderTarget();
			glClearColor(0.f,0.f,0.f,1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			program->render();
			program->getSFMLWindow()->display();
			firstRun = false;
			return;
		}
	}
    if (program->getRenderer() == NULL)
	{
		FATALMSG("Program has no Renderer! ... Dying.")
		//loop = false;
		//break;
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
		if (event.type == sf::Event::Closed)
			program->getSFMLWindow()->close();
	}
	if (!program->getSFMLWindow()->isOpen())
	{
		if (program->getScene() != NULL)
			program->getScene()->end();
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
@end

@implementation SilentWindow
-(void)keyDown:(NSEvent *)theEvent
{
    // Do nothing except preventing this alert.
}
@end








#endif