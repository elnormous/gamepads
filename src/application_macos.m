//
//  gamepads
//

#import <Cocoa/Cocoa.h>
#include "application.h"

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
    [sharedApplication run];

    [pool release];

    return 1;
}
