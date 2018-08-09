//
//  gamepads
//

#import <Cocoa/Cocoa.h>
#include "application.h"

void gpMain(GPApplication* application);

typedef struct GPWindowMacOS
{
    NSScreen* screen;
    NSWindow* window;
    NSTextField* textField;
    NSObject<NSWindowDelegate>* windowDelegate;
    NSMutableString* text;
} GPWindowMacOS;

@interface AppDelegate: NSObject<NSApplicationDelegate>
{
    GPApplication* application;
}
@end

@implementation AppDelegate

-(id)initWithApplication:(GPApplication*)initApplication
{
    if (self = [super init])
    {
        application = initApplication;
    }

    return self;
}

-(void)applicationWillFinishLaunching:(__unused NSNotification*)notification
{
    gpWindowInit(&application->window);
    gpInputInit(&application->input);
    gpMain(application);
}

-(void)applicationDidFinishLaunching:(__unused NSNotification*)notification
{
}

-(void)applicationWillTerminate:(__unused NSNotification*)notification
{
    gpWindowDestroy(&application->window);
}

-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(__unused NSApplication*)sender
{
    return YES;
}

-(void)applicationDidBecomeActive:(__unused NSNotification*)notification
{
}

-(void)applicationDidResignActive:(__unused NSNotification*)notification
{
}

-(void)handleQuit:(__unused id)sender
{
    [[NSApplication sharedApplication] terminate:nil];
}

@end

int gpApplicationInit(GPApplication* application)
{
    return 1;
}

int gpApplicationDestroy(GPApplication* application)
{
    return 1;
}

int gpApplicationRun(GPApplication* application)
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    NSApplication* sharedApplication = [NSApplication sharedApplication];
    [sharedApplication activateIgnoringOtherApps:YES];
    [sharedApplication setDelegate:[[[AppDelegate alloc] initWithApplication:application] autorelease]];

    NSMenu* mainMenu = [[[NSMenu alloc] initWithTitle:@"Main Menu"] autorelease];

    NSMenuItem* mainMenuItem = [[[NSMenuItem alloc] init] autorelease];
    [mainMenu addItem:mainMenuItem];

    NSMenu* subMenu = [[[NSMenu alloc] init] autorelease];
    [mainMenuItem setSubmenu:subMenu];

    NSMenuItem* quitItem = [[[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(handleQuit:) keyEquivalent:@"q"] autorelease];
    [quitItem setTarget: [sharedApplication delegate]];
    [subMenu addItem:quitItem];

    sharedApplication.mainMenu = mainMenu;

    [sharedApplication run];

    [pool release];

    return 1;
}

int gpLog(GPApplication* application, const char* string)
{
    GPWindowMacOS* windowMacOS = (GPWindowMacOS*)application->window.opaque;

    [windowMacOS->text appendString:[NSString stringWithUTF8String:string]];
    [windowMacOS->text appendString:@"\n"];

    [windowMacOS->textField performSelectorOnMainThread:@selector(setStringValue:) withObject:[NSString stringWithString:windowMacOS->text] waitUntilDone:NO];

    return 1;
}

@interface WindowDelegate: NSObject<NSWindowDelegate>
{
    GPWindowMacOS* window;
}

@end

@implementation WindowDelegate

-(id)initWithWindow:(GPWindowMacOS*)initWindow
{
    if (self = [super init])
    {
        window = initWindow;
    }

    return self;
}

@end

int gpWindowInit(GPWindow* window)
{
    GPWindowMacOS* windowMacOS = malloc(sizeof(GPWindowMacOS));
    memset(windowMacOS, 0, sizeof(GPWindowMacOS));
    window->opaque = windowMacOS;

    windowMacOS->screen = [NSScreen mainScreen];

    CGSize windowSize;
    windowSize.width = round(windowMacOS->screen.frame.size.width * 0.6);
    windowSize.height = round(windowMacOS->screen.frame.size.height * 0.6);

    NSRect frame = NSMakeRect(round(windowMacOS->screen.frame.size.width / 2.0f - windowSize.width / 2.0f),
                              round(windowMacOS->screen.frame.size.height / 2.0f - windowSize.height / 2.0f),
                              windowSize.width, windowSize.height);

    NSWindowStyleMask windowStyleMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;

    windowMacOS->window  = [[NSWindow alloc] initWithContentRect:frame
                                                       styleMask:windowStyleMask
                                                         backing:NSBackingStoreBuffered
                                                           defer:NO
                                                          screen:windowMacOS->screen];
    [windowMacOS->window setReleasedWhenClosed:NO];

    windowMacOS->window.acceptsMouseMovedEvents = YES;
    windowMacOS->windowDelegate = [[WindowDelegate alloc] initWithWindow:windowMacOS];
    windowMacOS->window.delegate = windowMacOS->windowDelegate;

    [windowMacOS->window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
    [windowMacOS->window setTitle:@"Gamepads"];

    NSRect windowFrame = [NSWindow contentRectForFrameRect:[windowMacOS->window frame]
                                                 styleMask:[windowMacOS->window styleMask]];


    windowMacOS->textField = [[NSTextField alloc] initWithFrame:windowFrame];
    windowMacOS->textField.editable = NO;
    windowMacOS->textField.usesSingleLineMode = NO;
    windowMacOS->textField.selectable = YES;

    windowMacOS->text = [[NSMutableString alloc] init];

    windowMacOS->window.contentView = windowMacOS->textField;
    [windowMacOS->window makeKeyAndOrderFront:nil];

    return 1;
}

int gpWindowDestroy(GPWindow* window)
{
    if (window->opaque)
    {
        GPWindowMacOS* windowMacOS = (GPWindowMacOS*)window->opaque;

        [windowMacOS->text release];
        [windowMacOS->textField release];
        windowMacOS->window.delegate = nil;
        [windowMacOS->window release];

        free(windowMacOS);
    }

    return 1;
}
