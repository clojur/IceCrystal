////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #1020]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_WIN32DISPLAY_H__
#define __APP_WIN32DISPLAY_H__


#include <NsCore/Noesis.h>
#include <NsApp/Display.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/Enums.h>

#include <windows.h>


namespace NoesisApp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Win32Window. Implementation of ISystemWindow for Windows systems using Win32 API.
////////////////////////////////////////////////////////////////////////////////////////////////////
class Win32Display final: public Display
{
public:
    Win32Display();
    ~Win32Display();

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
    void FillKeyTable();
    void LoadCursors();

    void BuildWin32StyleFlags(WindowStyle style, WindowState state, ResizeMode resize,
        bool showInTaskbar, bool topmost, bool allowDragFiles, uint32_t& flags,
        uint32_t& flagsEx) const;
    uint32_t GetWin32StyleFlags() const;
    void SetWin32StyleFlags(uint32_t flags) const;
    uint32_t GetWin32StyleFlagsEx() const;
    void SetWin32StyleFlagsEx(uint32_t flags) const;

    WindowStyle GetStyle(uint32_t flags, uint32_t flagsEx) const;
    WindowState GetState(uint32_t flags) const;
    ResizeMode GetResizeMode(uint32_t flags) const;
    bool GetShowInTaskbar(uint32_t flagsEx) const;
    bool GetTopmost(uint32_t flagsEx) const;
    bool GetAllowFileDrop(uint32_t flagsEx) const;

    void CenterWindow();

    bool DispatchEvent(unsigned int msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);

private:
    HWND mWindowHandle;
    WindowStartupLocation mStartupLocation;
    uint32_t mWindowFlags;

    Noesis::Cursor mCursor;
    HCURSOR mCursors[Noesis::Cursor_Count];
    uint8_t mKeyTable[256];

    NS_DECLARE_REFLECTION(Win32Display, Display)
};

}

#endif
