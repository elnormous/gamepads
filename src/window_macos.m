//
//  gamepads
//

#import <Cocoa/Cocoa.h>
#include "window.h"
#include "application.h"

typedef struct GPWindowMacOS
{
    NSScreen* screen;
    NSWindow* window;
    NSTextField* textField;
    NSObject<NSWindowDelegate>* windowDelegate;
} GPWindowMacOS;

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

-(void)appendString:(NSString*)string
{
    NSString* value = window->textField.stringValue;
    window->textField.stringValue = [value stringByAppendingString:string];
}

-(void)handleQuit:(__unused id)sender
{
}

@end

int gpWindowInit(GPWindow* window)
{
    GPWindowMacOS* windowMacOS = malloc(sizeof(GPWindowMacOS));
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

    windowMacOS->window.contentView = windowMacOS->textField;
    [windowMacOS->window makeKeyAndOrderFront:nil];

    return 1;
}

int gpWindowDestroy(GPWindow* window)
{
    if (window->opaque)
    {
        GPWindowMacOS* windowMacOS = (GPWindowMacOS*)window->opaque;

        [windowMacOS->textField release];
        windowMacOS->window.delegate = nil;
        [windowMacOS->window release];

        free(windowMacOS);
    }

    return 1;
}

int gpLog(GPApplication* application, const char* string)
{
    GPWindowMacOS* windowMacOS = (GPWindowMacOS*)application->window.opaque;

    NSString* value = [NSString stringWithUTF8String:string];
    [windowMacOS->windowDelegate performSelectorOnMainThread:@selector(appendString:) withObject:value waitUntilDone:NO];

    return 1;
}
