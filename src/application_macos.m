//
//  gamepads
//

#include "application.h"
#include "window_macos.h"

void gpMain(GPApplication* application);

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
