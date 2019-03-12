////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_ANDROIDDISPLAY_H__
#define __APP_ANDROIDDISPLAY_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsApp/Display.h>


struct android_app;
struct AInputEvent;


namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implementation of Display using Android NDK.
////////////////////////////////////////////////////////////////////////////////////////////////////
class AndroidDisplay: public Display
{
public:
    AndroidDisplay();
    ~AndroidDisplay();

    /// From Display
    //@{
    void SetTitle(const char* title) override;
    void SetLocation(int x, int y) override;
    void SetSize(uint32_t width, uint32_t height) override;
    void SetWindowStyle(WindowStyle windowStyle) override;
    void SetWindowState(WindowState windowState) override;
    void SetResizeMode(ResizeMode resizeMode) override;
    void SetShowInTaskbar(bool showInTaskbar) override;
    void SetTopmost(bool topmost) override;
    void SetAllowFileDrop(bool allowFileDrop) override;
    void SetWindowStartupLocation(WindowStartupLocation location) override;
    void Show() override;
    void EnterMessageLoop() override;
    void Close() override;
    void* GetNativeHandle() const override;
    uint32_t GetClientWidth() const override;
    uint32_t GetClientHeight() const override;
    //@}

private:
    bool ProcessMessages();

    static void OnAppCmd(android_app* app, int cmd);
    void DispatchAppEvent(int eventId);

    static int OnInputEvent(android_app* app, AInputEvent* event);
    int DispatchInputEvent(AInputEvent* event);

    void UpdateSize();

    void FillKeyTable();

private:
    uint8_t mKeyTable[256];
    uint32_t mWidth;
    uint32_t mHeight;

    NS_DECLARE_REFLECTION(AndroidDisplay, Display)
};

}


#endif
