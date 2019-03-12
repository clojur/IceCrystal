////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "UIKitDisplay.h"
#include "ViewController.h"
#include "View.h"

#include <NsCore/ReflectionImplement.h>
#include <NsCore/Category.h>
#include <NsGui/IntegrationAPI.h>
#include <NsApp/DisplayLauncher.h>
#include <NsApp/CommandLine.h>
#include <UIKit/UIKit.h>


using namespace Noesis;
using namespace NoesisApp;


@implementation UISubclassedWindow

-(void)activate:(NSNotification*)note
{
    display->OnActivated();
}

-(void)deactivate:(NSNotification*)note
{
    display->OnDeactivated();
}

-(void)render
{
    display->OnRender();
}

@end

namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////
bool ShowSoftwareKeyboard(void* user, UIElement* focusedElement)
{
    UIKitDisplay* display = (UIKitDisplay*)user;
    UIView* view = display->mWindow.rootViewController.view;
    [view becomeFirstResponder];
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void HideSoftwareKeyboard(void* user)
{
    UIKitDisplay* display = (UIKitDisplay*)user;
    UIView* view = display->mWindow.rootViewController.view;
    [view resignFirstResponder];
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
UIKitDisplay::UIKitDisplay()
{
    @autoreleasepool
    {
        mWindow = [[UISubclassedWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        mWindow->display = this;
        mWindow.autoresizesSubviews = YES;
	    mWindow.rootViewController = [[[ViewController alloc] initWithDisplay:this] autorelease];
	    [mWindow setBackgroundColor:[UIColor blackColor]];

        [[NSNotificationCenter defaultCenter] addObserver:mWindow selector:@selector(activate:)
            name:UIApplicationDidBecomeActiveNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:mWindow selector:@selector(deactivate:)
            name:UIApplicationWillResignActiveNotification object:nil];

        GUI::SetSoftwareKeyboardCallbacks(this, ShowSoftwareKeyboard, HideSoftwareKeyboard);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
UIKitDisplay::~UIKitDisplay()
{
    [mDisplayLink invalidate];
    [mDisplayLink release];

    GUI::SetSoftwareKeyboardCallbacks(nullptr, nullptr, nullptr);

    [[NSNotificationCenter defaultCenter] removeObserver:mWindow
        name:UIApplicationDidBecomeActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:mWindow
        name:UIApplicationWillResignActiveNotification object:nil];

    [mWindow release];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::Show()
{
    mSizeChanged(this, GetClientWidth(), GetClientHeight());
    mLocationChanged(this, 0, 0);
    [mWindow makeKeyAndVisible];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::EnterMessageLoop()
{
    mDisplayLink = [CADisplayLink displayLinkWithTarget:mWindow selector:@selector(render)];
    [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] run];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* UIKitDisplay::GetNativeHandle() const
{
    return mWindow.rootViewController.view;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t UIKitDisplay::GetClientWidth() const
{
    CGFloat scale = UIScreen.mainScreen.scale;
    return  (uint32_t)(mWindow.bounds.size.width * scale);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t UIKitDisplay::GetClientHeight() const
{
    CGFloat scale = UIScreen.mainScreen.scale;
    return  (uint32_t)(mWindow.bounds.size.height * scale);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnActivated()
{
    mActivated(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnDeactivated()
{
    mDeactivated(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnRender()
{
    mRender(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnSizeChanged(CGFloat width, CGFloat height)
{
    mSizeChanged(this, (uint32_t)width, (uint32_t)height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnChar(uint32_t c)
{
    mChar(this, c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnDeleteBackward()
{
    mKeyDown(this, Key::Key_Back);
    mKeyUp(this, Key::Key_Back);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnTouchDown(CGFloat x, CGFloat y, uint64_t id)
{
    mTouchDown(this, (int)x, (int)y, id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnTouchMove(CGFloat x, CGFloat y, uint64_t id)
{
    mTouchMove(this, (int)x, (int)y, id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UIKitDisplay::OnTouchUp(CGFloat x, CGFloat y, uint64_t id)
{
    mTouchUp(this, (int)x, (int)y, id);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(UIKitDisplay)
{
    NsMeta<TypeId>("UIKitDisplay");
    NsMeta<Category>("Display");
}

