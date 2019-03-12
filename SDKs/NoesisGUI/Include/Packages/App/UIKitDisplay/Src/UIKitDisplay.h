////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_UIKITDISPLAY_H__
#define __APP_UIKITDISPLAY_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/Display.h>

#import <UIKit/UIKit.h>


@class UISubclassedWindow;


namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
class UIKitDisplay final: public Display
{
public:
    UIKitDisplay();
    ~UIKitDisplay();

    /// From Display
    //@{
    void Show() override;
    void EnterMessageLoop() override;
    void* GetNativeHandle() const override;
    uint32_t GetClientWidth() const override;
    uint32_t GetClientHeight() const override;
    //@}

    void OnActivated();
    void OnDeactivated();
    void OnRender();
    void OnSizeChanged(CGFloat width, CGFloat height);
    void OnChar(uint32_t c);
    void OnDeleteBackward();
    void OnTouchDown(CGFloat x, CGFloat y, uint64_t id);
    void OnTouchMove(CGFloat x, CGFloat y, uint64_t id);
    void OnTouchUp(CGFloat x, CGFloat y, uint64_t id);

public:
    UISubclassedWindow* mWindow;
    CADisplayLink* mDisplayLink;

    NS_DECLARE_REFLECTION(UIKitDisplay, Display)
};

}

////////////////////////////////////////////////////////////////////////////////////////////////////
@interface UISubclassedWindow : UIWindow
{
@public
    NoesisApp::UIKitDisplay* display;
}
@end

#endif
