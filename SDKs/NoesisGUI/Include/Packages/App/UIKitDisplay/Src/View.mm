////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "View.h"
#include "UIKitDisplay.h"


using namespace NoesisApp;


@implementation View

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGFloat s = [UIScreen mainScreen].scale;
    for (UITouch* touch in touches)
    {
        CGPoint p = [touch locationInView:nil];
        display->OnTouchDown(p.x * s, p.y * s, (uintptr_t)touch);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGFloat s = [UIScreen mainScreen].scale;
    for (UITouch* touch in touches)
    {
        CGPoint p = [touch locationInView:nil];
        display->OnTouchMove(p.x * s, p.y * s, (uintptr_t)touch);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGFloat s = [UIScreen mainScreen].scale;
    for (UITouch* touch in touches)
    {
        CGPoint p = [touch locationInView:nil];
        display->OnTouchUp(p.x * s, p.y * s, (uintptr_t)touch);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}

- (void)insertText:(NSString *)text
{
    for (size_t i = 0; i < text.length; i++)
    {
        display->OnChar([text characterAtIndex:i]);
    }
}

- (void)deleteBackward
{
    display->OnDeleteBackward();
}

- (BOOL)hasText
{
    return YES;
}

- (BOOL)canBecomeFirstResponder
{
    return YES;
}

@end
