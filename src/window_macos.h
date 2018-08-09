//
//  gamepads
//

#ifndef WINDOW_MACOS_H
#define WINDOW_MACOS_H

#import <Cocoa/Cocoa.h>
#include "window.h"

typedef struct GPWindowMacOS
{
    NSScreen* screen;
    NSWindow* window;
    NSTextField* textField;
    NSObject<NSWindowDelegate>* windowDelegate;
    NSMutableString* text;
} GPWindowMacOS;

#endif
