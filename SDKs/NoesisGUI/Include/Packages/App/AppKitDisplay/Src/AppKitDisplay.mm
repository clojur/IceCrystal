////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "AppKitDisplay.h"
#include "UISubclassedWindow.h"
#include "AppDelegate.h"
#include "KeyCodes.h"

#include <NsCore/ReflectionImplement.h>
#include <NsCore/Category.h>
#include <AppKit/NSColor.h>


using namespace Noesis;
using namespace NoesisApp;


////////////////////////////////////////////////////////////////////////////////////////////////////
AppKitDisplay::AppKitDisplay(): mClosed(false)
{
    @autoreleasepool
    {
        FillKeyTable();

        NSRect frame = NSMakeRect(0, 0, 1280, 720);
        NSUInteger styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
            NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

        mWindow = [[UISubclassedWindow alloc] initWithContentRect:frame styleMask:styleMask
            backing: NSBackingStoreBuffered defer:FALSE];

        mWindow->display = this;
        [mWindow setReleasedWhenClosed:FALSE];
        [mWindow setBackgroundColor:[NSColor blackColor]];
        [mWindow setOpaque:YES];
        [mWindow setDelegate:mWindow];
        [mWindow setAcceptsMouseMovedEvents:TRUE];
        [mWindow registerForDraggedTypes:[NSArray arrayWithObjects: NSFilenamesPboardType, nil]];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
AppKitDisplay::~AppKitDisplay()
{
    [mWindow release];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::SetTitle(const char* title)
{
    [mWindow setTitle:[NSString stringWithUTF8String:title]];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::SetLocation(int x, int y)
{
    [mWindow setFrameOrigin:NSMakePoint(x, y)];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::SetSize(uint32_t width, uint32_t height)
{
    NSRect frame = mWindow.frame;
    [mWindow setFrame:NSMakeRect(frame.origin.x, frame.origin.y, width, height) display:NO];
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::SetAllowFileDrop(bool allowFileDrop)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::Show()
{
    @autoreleasepool
    {
        NSRect frame = mWindow.frame;
        mSizeChanged(this, (uint32_t)frame.size.width, (uint32_t)frame.size.height);
        mLocationChanged(this, (int)frame.origin.x, (int)frame.origin.y);

        [mWindow center];
        [mWindow makeKeyAndOrderFront:mWindow];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::EnterMessageLoop()
{
    @autoreleasepool
    {
        AppDelegate* appDelegate = [[AppDelegate alloc] initWithDisplay:this];
        [[NSApplication sharedApplication] setDelegate:appDelegate];
        [NSApp run];
        [appDelegate release];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::Close()
{
    [mWindow close];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* AppKitDisplay::GetNativeHandle() const
{
    return mWindow.contentView;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t AppKitDisplay::GetClientWidth() const
{
    return (uint32_t)mWindow.contentView.bounds.size.width;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t AppKitDisplay::GetClientHeight() const
{
    return (uint32_t)mWindow.contentView.bounds.size.height;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::FillKeyTable()
{
    memset(mKeyTable, 0, sizeof(mKeyTable));

    mKeyTable[VK_ANSI_Q] = Key_Q;
    mKeyTable[VK_ANSI_W] = Key_W;
    mKeyTable[VK_ANSI_E] = Key_E;
    mKeyTable[VK_ANSI_R] = Key_R;
    mKeyTable[VK_ANSI_T] = Key_T;
    mKeyTable[VK_ANSI_Y] = Key_Y;
    mKeyTable[VK_ANSI_U] = Key_U;
    mKeyTable[VK_ANSI_I] = Key_I;
    mKeyTable[VK_ANSI_O] = Key_O;
    mKeyTable[VK_ANSI_P] = Key_P;
    mKeyTable[VK_ANSI_A] = Key_A;
    mKeyTable[VK_ANSI_S] = Key_S;
    mKeyTable[VK_ANSI_D] = Key_D;
    mKeyTable[VK_ANSI_F] = Key_F;
    mKeyTable[VK_ANSI_G] = Key_G;
    mKeyTable[VK_ANSI_H] = Key_H;
    mKeyTable[VK_ANSI_J] = Key_J;
    mKeyTable[VK_ANSI_K] = Key_K;
    mKeyTable[VK_ANSI_L] = Key_L;
    mKeyTable[VK_ANSI_Z] = Key_Z;
    mKeyTable[VK_ANSI_X] = Key_X;
    mKeyTable[VK_ANSI_C] = Key_C;
    mKeyTable[VK_ANSI_V] = Key_V;
    mKeyTable[VK_ANSI_B] = Key_B;
    mKeyTable[VK_ANSI_N] = Key_N;
    mKeyTable[VK_ANSI_M] = Key_M;
    mKeyTable[VK_ANSI_1] = Key_D1;
    mKeyTable[VK_ANSI_2] = Key_D2;
    mKeyTable[VK_ANSI_3] = Key_D3;
    mKeyTable[VK_ANSI_4] = Key_D4;
    mKeyTable[VK_ANSI_5] = Key_D5;
    mKeyTable[VK_ANSI_6] = Key_D6;
    mKeyTable[VK_ANSI_7] = Key_D7;
    mKeyTable[VK_ANSI_8] = Key_D8;
    mKeyTable[VK_ANSI_9] = Key_D9;
    mKeyTable[VK_ANSI_0] = Key_D0;

    mKeyTable[VK_ANSI_Keypad0] = Key_NumPad0;
    mKeyTable[VK_ANSI_Keypad1] = Key_NumPad1;
    mKeyTable[VK_ANSI_Keypad2] = Key_NumPad2;
    mKeyTable[VK_ANSI_Keypad3] = Key_NumPad3;
    mKeyTable[VK_ANSI_Keypad4] = Key_NumPad4;
    mKeyTable[VK_ANSI_Keypad5] = Key_NumPad5;
    mKeyTable[VK_ANSI_Keypad6] = Key_NumPad6;
    mKeyTable[VK_ANSI_Keypad7] = Key_NumPad7;
    mKeyTable[VK_ANSI_Keypad8] = Key_NumPad8;
    mKeyTable[VK_ANSI_Keypad9] = Key_NumPad9;

    mKeyTable[VK_Return] = Key_Return;
    mKeyTable[VK_Tab] = Key_Tab;
    mKeyTable[VK_Space] = Key_Space;
    mKeyTable[VK_Delete] = Key_Back;
    mKeyTable[VK_ForwardDelete] = Key_Delete;
    mKeyTable[VK_Escape] = Key_Escape;
    mKeyTable[VK_Shift] = Key_LeftShift;
    mKeyTable[VK_Control] = Key_LeftCtrl;
    mKeyTable[VK_Option] = Key_LeftAlt;
    mKeyTable[VK_Home] = Key_Home;
    mKeyTable[VK_Help] = Key_Help;
    mKeyTable[VK_End] = Key_End;
    mKeyTable[VK_LeftArrow] = Key_Left;
    mKeyTable[VK_RightArrow] = Key_Right;
    mKeyTable[VK_DownArrow] = Key_Down;
    mKeyTable[VK_UpArrow] = Key_Up;

    mKeyTable[VK_F1] = Key_F1;
    mKeyTable[VK_F2] = Key_F2;
    mKeyTable[VK_F3] = Key_F3;
    mKeyTable[VK_F4] = Key_F4;
    mKeyTable[VK_F5] = Key_F5;
    mKeyTable[VK_F6] = Key_F6;
    mKeyTable[VK_F7] = Key_F7;
    mKeyTable[VK_F8] = Key_F8;
    mKeyTable[VK_F9] = Key_F9;
    mKeyTable[VK_F10] = Key_F10;
    mKeyTable[VK_F11] = Key_F11;
    mKeyTable[VK_F12] = Key_F12;
    mKeyTable[VK_F13] = Key_F13;
    mKeyTable[VK_F14] = Key_F14;
    mKeyTable[VK_F15] = Key_F15;
    mKeyTable[VK_F16] = Key_F16;
    mKeyTable[VK_F17] = Key_F17;
    mKeyTable[VK_F18] = Key_F18;
    mKeyTable[VK_F19] = Key_F19;
    mKeyTable[VK_F20] = Key_F20;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnActivated()
{
    mActivated(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnDeactivated()
{
    mDeactivated(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnRender()
{
    mRender(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnSizeChanged(CGFloat width, CGFloat height)
{
    mSizeChanged(this, (uint32_t)width, (uint32_t)height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnLocationChanged(CGFloat x, CGFloat y)
{
    mLocationChanged(this, (int)x, (int)y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnKeyDown(unsigned short key)
{
    mKeyDown(this, (Noesis::Key)mKeyTable[key]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnKeyUp(unsigned short key)
{
    mKeyUp(this, (Noesis::Key)mKeyTable[key]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnChar(unichar ch)
{
    mChar(this, ch);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnMouseDoubleClick(CGFloat x, CGFloat y, Noesis::MouseButton button)
{
    mMouseDoubleClick(this, (int)x, (int)y, button);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnMouseButtonDown(CGFloat x, CGFloat y, Noesis::MouseButton button)
{
    mMouseButtonDown(this, (int)x, (int)y, button);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnMouseButtonUp(CGFloat x, CGFloat y, Noesis::MouseButton button)
{
    mMouseButtonUp(this, (int)x, (int)y, button);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnMouseMove(CGFloat x, CGFloat y)
{
    mMouseMove(this, (int)x, (int)y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnMouseWheel(CGFloat x, CGFloat y, CGFloat delta)
{
    mMouseWheel(this, (int)x, (int)y, (int)(delta * 1000.0f));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void AppKitDisplay::OnFileDropped(const char* filename)
{
    mFileDropped(this, filename);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(AppKitDisplay)
{
    NsMeta<TypeId>("AppKitDisplay");
    NsMeta<Category>("Display");
}
