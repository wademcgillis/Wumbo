#define DEPTH_BUFFER 1

#include "Wumbo-Header.h"
#if PLATFORM_IOS
#include "Wumbo-iOSRoot.h"
#include "Wumbo-InputManager.h"
#include "Wumbo-Renderer.h"
#include "Wumbo-Platforms.h"
#include <CoreMotion/CoreMotion.h>
//#include "Finch.h"
#include "Wumbo-Quicky.h"

#if USE_TESTFLIGHT
#import "TestFlight.h"
#endif

std::vector<std::string> messages;

void TFDEBUG(std::string str)
{
	messages.push_back(str);
	while(messages.size() > 10)
		messages.erase(messages.begin());
}

void SignalHandler(int signal)
{
	NSMutableString *fed = [NSMutableString stringWithString:@""];
	[fed appendFormat:@"Oh I crashed. Last %zu messages:\n\n",messages.size()];
	for(int i=0;i<messages.size();i++)
		[fed appendFormat:@"\t%s\n",messages.at(i).c_str()];
	//[TestFlight submitFeedback:fed];
}
@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	//int *a = new int;
	//free(a);
	//free(a);
    // Override point for customization after application launch.
	self.window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
    //self.viewController //[[TitleScreenController alloc] init];//
    self.window.rootViewController = [[ViewController alloc] init];//self.viewController;
	//[self.window addSubview:self.window.rootViewController.view];
    [self.window makeKeyAndVisible];
	signal(SIGABRT, SignalHandler);
#if USE_TESTFLIGHT
	[TestFlight takeOff:@"9f8e6921-4a84-4a59-844a-3b99f1f49199"];
#endif
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	[((ViewController*)self.window.rootViewController) getProgram]->background();
	//SignalHandler(0);
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}



- (CMMotionManager *)motionManager
{
    if (!motionManager) motionManager = [[CMMotionManager alloc] init];
		
		return motionManager;
}

@end

/*
 *
 *
 *
 *
 *
 *
 *
 *
 */


@interface ViewController () {
	GLuint _program;
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

- (BOOL)loadShaders;
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
- (BOOL)linkProgram:(GLuint)prog;
- (BOOL)validateProgram:(GLuint)prog;
@end


UITextView *wumbo_iosroot_textview;

@implementation ViewController

- (Wumbo::Program*) getProgram
{
	return program;
}

- (BOOL)canBecomeFirstResponder {
    return YES;
}

- (void) showKeyboard
{
	[wumbo_iosroot_textview becomeFirstResponder];
	Wumbo::Platform_iOS::__keyboardvisible = true;
}

- (void) hideKeyboard
{
	[wumbo_iosroot_textview resignFirstResponder];
	Wumbo::Platform_iOS::__forcekeyboard = false;
	Wumbo::Platform_iOS::__keyboardvisible = false;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    printf("view did load!\n");
#if 1
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
#else
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
#endif
	
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
	
	
	lastTap1970 = [[NSDate date] timeIntervalSince1970];
    
    GLKView *view = (GLKView *)self.view;
	
	wumbo_iosroot_textview = [[UITextView alloc] initWithFrame:CGRectMake(0,view.bounds.size.height,view.bounds.size.width,32)];//view.bounds.size.height/100)];
	[view addSubview:wumbo_iosroot_textview];
	wumbo_iosroot_textview.autocapitalizationType = UITextAutocapitalizationTypeNone;
	wumbo_iosroot_textview.autocorrectionType = UITextAutocorrectionTypeNo;
	wumbo_iosroot_textview.keyboardAppearance = UIKeyboardAppearanceDark;
	
	if ( self ) {
		NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
		[center addObserver:self selector:@selector(keyboardWasShown) name:UIKeyboardDidShowNotification object:nil];
		[center addObserver:self selector:@selector(keyboardWasHidden) name:UIKeyboardDidHideNotification object:nil];
	}
	
	swipeRecognizerLeft = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
	[swipeRecognizerLeft setDirection:(UISwipeGestureRecognizerDirectionLeft)];
	swipeRecognizerLeft.cancelsTouchesInView = NO;
	[view addGestureRecognizer:swipeRecognizerLeft];

	swipeRecognizerUp = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
	[swipeRecognizerUp setDirection:(UISwipeGestureRecognizerDirectionUp)];
	swipeRecognizerUp.cancelsTouchesInView = NO;
	[view addGestureRecognizer:swipeRecognizerUp];
	
	swipeRecognizerRight = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
	[swipeRecognizerRight setDirection:(UISwipeGestureRecognizerDirectionRight)];
	swipeRecognizerRight.cancelsTouchesInView = NO;
	[view addGestureRecognizer:swipeRecognizerRight];
	
	swipeRecognizerDown = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(handleSwipe:)];
	[swipeRecognizerDown setDirection:(UISwipeGestureRecognizerDirectionDown)];
	swipeRecognizerDown.cancelsTouchesInView = NO;
	[view addGestureRecognizer:swipeRecognizerDown];
	
	tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
	tapRecognizer.cancelsTouchesInView = NO;
	tapRecognizer.numberOfTapsRequired = 1;
	[view addGestureRecognizer:tapRecognizer];
	
	/*doubleTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleDoubleTap:)];
	doubleTapRecognizer.cancelsTouchesInView = NO;
	doubleTapRecognizer.numberOfTapsRequired = 2;
	[view addGestureRecognizer:doubleTapRecognizer];*/
	
	view.backgroundColor = [UIColor colorWithRed:1 green:1 blue:0 alpha:1];
    view.context = self.context;
	view.drawableColorFormat = GLKViewDrawableColorFormatRGB565;
	//FART BAGS
#if 0//DEPTH_BUFFER
	printf("DEPHT BUFFER FOR US!\n");
    view.drawableDepthFormat = GLKViewDrawableDepthFormat16;
#else
	view.drawableDepthFormat = GLKViewDrawableDepthFormatNone;
#endif
    view.multipleTouchEnabled = YES;

	//[view becomeFirstResponder];
    [self setupGL];
}

- (void) keyboardWasHidden
{
	Wumbo::Platform_iOS::__keyboardvisible = false;
}

- (void) keyboardWasShown
{
	Wumbo::Platform_iOS::__keyboardvisible = true;
}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	// ARC WHY YOU DO THIS :C
	//[swipeRecognizerLeft dealloc];
	//[swipeRecognizerUp dealloc];
	//[swipeRecognizerRight dealloc];
	//[swipeRecognizerDown dealloc];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
	
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
	
    // Dispose of any resources that can be recreated.
}





extern Wumbo::Program *WUMBOFRAMEWORK_GETPROGRAM();

void glBindFramebuffer0()
{
	[((GLKView *) ((ViewController*)Wumbo::__quickyProgram->getGLKViewController()).view) bindDrawable];
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
	//finch = new Finch();
	//finch->init();
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//[self
		
	program = WUMBOFRAMEWORK_GETPROGRAM();
	Wumbo::Platform_iOS::INITIALIZE_VALUES(program);
	program->setAppDelegate((AppDelegate*)[[UIApplication sharedApplication] delegate]);
	program->setGLKViewController(self);
	
	
	program->begin();
	toucher = (Wumbo::TouchInput_iOS*) program->getInputManager()->getTouchState();
	
	keyboardStringLength = 0;
	keyboardStringLengthPrevious = 0;
	
	
		

	
	//program->getInputManager()->update();
	
	//program->update();
	//program->getRenderer()->resetVirtualRenderTarget();
	//program->render();
}

- (void)viewWillLayoutSubviews
{

}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
	/*if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }*/
}


- (void)update
{
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
	//printf("FLATOFY\n");
	static double before = [[NSDate date] timeIntervalSince1970];
	static int ticks = 0;
	double now = [[NSDate date] timeIntervalSince1970];
	if ((int)(now-before) == 1)
	{
		before = now;
		printf("FPS: %i\n",ticks);
		ticks = 0;
	}
	ticks++;
	//printf("pop\n");
	//glScissor(0,0,self.view.bounds.size.width*[UIScreen mainScreen].scale,self.view.bounds.size.height*[UIScreen mainScreen].scale);
	//glClearColor(0.f, 0.f, 0.f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT);
	if (Wumbo::Platform_iOS::__forcekeyboard)
	{
		if (!Wumbo::Platform_iOS::__keyboardvisible)
		{
			NSLog(@"WE DON'T GOT A KEYBOARD");
			[wumbo_iosroot_textview becomeFirstResponder];
		}
	}
	if (Wumbo::Platform_iOS::__keyboardvisible)
	{
		keyboardStringLengthPrevious = keyboardStringLength;
		keyboardStringLength = [[wumbo_iosroot_textview text] length];
		
		if (keyboardStringLength != keyboardStringLengthPrevious)
		{
			if (keyboardStringLength > keyboardStringLengthPrevious)
			{
				printf("iOS KEYBOARD NEW KEY EVENT!\n");
				NSString *newString = [[wumbo_iosroot_textview text] substringFromIndex:keyboardStringLengthPrevious];
				printf("\tkeys added: %s\n",[newString UTF8String]);
				printf("\t\t");
				for(int i=0;i<[newString length];i++)
				{
					printf("%i, ",[newString UTF8String][i]);
				}
				printf("\n");
				if ([newString UTF8String][0] == 13 || [newString UTF8String][0] == 10)
				{
					Wumbo::__quickyKeyboard->iOS_addKeys(0,13);
					keyboardStringLength = 512;
				}
				else
				{
					Wumbo::__quickyKeyboard->iOS_addKeys([newString UTF8String],[newString length]);
				}
				
				if (keyboardStringLength > 511)
				{
					[wumbo_iosroot_textview setText:@""];
					keyboardStringLength = 0;
				}
			}
			else
			{
				Wumbo::__quickyKeyboard->iOS_addKeys(0,9);
			}
		}
	}
	
	
	program->getInputManager()->update();
	program->update();
	
	//printf("BLOOFNOOF\n");
	
	program->getRenderer()->resetVirtualRenderTarget();
	program->render();
	
	toucher->onEndOfFrame();
	Wumbo::__quickyKeyboard->onEndOfFrame();
	
	//printf("ZANGAZEED\n");
	//glClearColor(1.f, 1.f, 0.f, 1.f);
	//glClear(GL_COLOR_BUFFER_BIT);
}

- (BOOL)hasText
{
    return NO;
}

- (void)insertText:(NSString *)str
{
	printf("WE GOT TEXT INSERTED: %s\n",[str UTF8String]);
    Wumbo::__quickyiCade->ios_add([str UTF8String],[str length]);
}

- (void)deleteBackward {}

- (void)handleSwipe:(UIGestureRecognizer *)gestureRecognizer
{
	UISwipeGestureRecognizer *swiper = (UISwipeGestureRecognizer*)gestureRecognizer;
	//NSLog(@"SWIPE! %i",swiper.direction);
	if (swiper.direction == UISwipeGestureRecognizerDirectionLeft)
		toucher->addSwipe(Wumbo::TouchInput::SWIPE_LEFT);
	if (swiper.direction == UISwipeGestureRecognizerDirectionUp)
		toucher->addSwipe(Wumbo::TouchInput::SWIPE_UP);
	if (swiper.direction == UISwipeGestureRecognizerDirectionRight)
		toucher->addSwipe(Wumbo::TouchInput::SWIPE_RIGHT);
	if (swiper.direction == UISwipeGestureRecognizerDirectionDown)
		toucher->addSwipe(Wumbo::TouchInput::SWIPE_DOWN);
}

- (void)handleTap:(UIGestureRecognizer *)gestureRecognizer
{
	UITapGestureRecognizer *tapper = (UITapGestureRecognizer*)gestureRecognizer;
	double currentTap1970 = [[NSDate date] timeIntervalSince1970];
	if ((currentTap1970-lastTap1970) < .27)
	{
		printf("DOUBLE!\n");
		toucher->addDoubleTap([tapper locationInView:nil]);
	}
	lastTap1970 = currentTap1970;
	//NSLog(@"SWIPE! %i",swiper.direction);
	toucher->addTap([tapper locationInView:nil]);
	//NSLog(@"Tap! %f %f",((Wumbo::TouchInput_iOS*)Wumbo::__quickyTouch)->fixTouchForOrientation([tapper locationInView:nil]).x,((Wumbo::TouchInput_iOS*)Wumbo::__quickyTouch)->fixTouchForOrientation([tapper locationInView:nil]).y);
}

- (void)handleDoubleTap:(UIGestureRecognizer *)gestureRecognizer
{
	//printf("DOUBLE TAP!\n");
}

// Handles the start of a touch
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSSet *allTouches = [event allTouches];
    for(int i=0;i<[allTouches count];i++)
	{
		UITouch *touch = [[allTouches allObjects] objectAtIndex:i];
		if ([touch phase] == UITouchPhaseBegan)
			toucher->addTouch(touch);
	}
}

// Handles the continuation of a touch.
-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	/*NSSet *allTouches = [event allTouches];
	if ([allTouches count] > 0)
	//UITouch *herman = ;
	NSLog(@"Position: %f %f",[[[allTouches allObjects] objectAtIndex:0] locationInView:nil].x,[[[allTouches allObjects] objectAtIndex:0] locationInView:nil].y);*/
}

// Handles the end of a touch event.
-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSSet *allTouches = [event allTouches];
    for(int i=0;i<[allTouches count];i++)
	{
		UITouch *touch = [[allTouches allObjects] objectAtIndex:i];
        if ([touch phase] == UITouchPhaseEnded)
            toucher->removeTouch(touch);
	}
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	NSSet *allTouches = [event allTouches];
    for(int i=0;i<[allTouches count];i++)
	{
		UITouch *touch = [[allTouches allObjects] objectAtIndex:i];
        if ([touch phase] == UITouchPhaseCancelled)
            toucher->removeTouch(touch);
	}
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
	bool ret = false;
	ret |= (supportedOrientations[UIDeviceOrientationLandscapeLeft] && interfaceOrientation == UIInterfaceOrientationLandscapeLeft);
	ret |= (supportedOrientations[UIDeviceOrientationPortrait] && interfaceOrientation == UIInterfaceOrientationPortrait);
	ret |= (supportedOrientations[UIDeviceOrientationLandscapeRight] && interfaceOrientation == UIInterfaceOrientationLandscapeRight);
	ret |= (supportedOrientations[UIDeviceOrientationPortraitUpsideDown] && interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
	return ret;
    //return (interfaceOrientation == UIInterfaceOrientationLandscapeUp || interfaceOrientation == UIInterfaceOrientationLandscapeUp || interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}

- (void)addSupportedOrientation:(UIInterfaceOrientation) orient
{
	if (orient > 0 && orient <= 4)
		supportedOrientations[orient] = YES;
}
- (void)removeSupportedOrientation:(UIInterfaceOrientation) orient
{
	if (orient > 0 && orient <= 4)
		supportedOrientations[orient] = NO;
}

#pragma mark -  OpenGL ES 2 shader compilation

- (BOOL)loadShaders
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    // Create shader program.
    _program = glCreateProgram();
    
    // Create and compile vertex shader.
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
	//NSLog(@"%s\n",[fragShaderPathname UTF8String]);
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }
    
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
    glBindAttribLocation(_program, GLKVertexAttribPosition, "position");
	glBindAttribLocation(_program, GLKVertexAttribColor, "color");
	glBindAttribLocation(_program, GLKVertexAttribTexCoord0, "texcoord0");
	
	//glGetUniformLocation(_program, "lolo");
    //glBindAttribLocation(_program, GLKVertexAttribNormal, "normal");
    
    // Link program.
    if (![self linkProgram:_program]) {
        NSLog(@"Failed to link program: %d", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
		}
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return NO;
    }
    
    // Get uniform locations.
    //uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
    //uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
		glDeleteShader(vertShader);
	}
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    NSLog(@"Shader success");
    return YES;
}

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
        return NO;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
	
    if (status == 0) {
        glDeleteShader(*shader);
        return NO;
    }
    
    return YES;
}

- (BOOL)linkProgram:(GLuint)prog
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

- (BOOL)validateProgram:(GLuint)prog
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return NO;
    }
	return YES;
}
@end
#endif