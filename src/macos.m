//
//  gamepads
//

#import <Cocoa/Cocoa.h>
#include "application.h"

@interface AppDelegate: NSObject<NSApplicationDelegate>

@end

@implementation AppDelegate

-(void)applicationWillFinishLaunching:(__unused NSNotification*)notification
{
}

-(void)applicationDidFinishLaunching:(__unused NSNotification*)notification
{
}

-(void)applicationWillTerminate:(__unused NSNotification*)notification
{
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
    @autoreleasepool
    {
        NSApplication* application = [NSApplication sharedApplication];
        [application activateIgnoringOtherApps:YES];
        [application setDelegate:[[[AppDelegate alloc] init] autorelease]];
        [application run];
    }

    return 1;
}
