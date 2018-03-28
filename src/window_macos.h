//
//  gamepads
//

#pragma once

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
