////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "XDisplay.h"

#include <NsCore/TypeId.h>
#include <NsCore/Category.h>
#include <NsCore/ReflectionImplement.h>


using namespace Noesis;
using namespace NoesisApp;


namespace
{

////////////////////////////////////////////////////////////////////////////////////////////////////
MouseButton NoesisButton(int b)
{
    if (b == 1)
    {
        return MouseButton_Left;
    }
    else if (b == 2)
    {
        return MouseButton_Middle;
    }
    else if (b == 3)
    {
        return MouseButton_Right;
    }

    return MouseButton_XButton1;
}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
XDisplay::XDisplay()
{
    // Open display
    _x.display = XOpenDisplay(0);
    if (_x.display == 0)
    {
        NS_FATAL("Failed to open X display");
    }
    
    // Find best XVisualInfo
    int attribs[] =
    { 
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_DEPTH_SIZE, 0,
        GLX_STENCIL_SIZE, 8,
        None 
    };

    _x.visualInfo = glXChooseVisual(_x.display, DefaultScreen(_x.display), attribs);
    if (_x.visualInfo == 0)
    {
        NS_FATAL("Matching XVisualInfo not found");
    }

    // Create a color map
    Window rootWindow = RootWindow(_x.display, _x.visualInfo->screen);
    Colormap colorMap = XCreateColormap(_x.display, rootWindow, _x.visualInfo->visual, AllocNone);

    // Create window
    XSetWindowAttributes swa;
    swa.colormap = colorMap;
    swa.border_pixel = 0;
    swa.event_mask = FocusChangeMask | StructureNotifyMask | SubstructureNotifyMask
        | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

    _x.window = XCreateWindow(_x.display, rootWindow, 0, 0, 1280, 720, 0, _x.visualInfo->depth,
        InputOutput, _x.visualInfo->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
    if (_x.window == 0)
    {
        NS_FATAL("Failed to create window");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
XDisplay::~XDisplay()
{
    XWindowAttributes attrs;
    XGetWindowAttributes(_x.display, _x.window, &attrs);
    
    XFreeColormap(_x.display, attrs.colormap);
    XDestroyWindow(_x.display, _x.window);

    XCloseDisplay(_x.display);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XDisplay::SetTitle(const char* title)
{
    XStoreName(_x.display, _x.window, title);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XDisplay::SetLocation(int x, int y)
{
    XMoveWindow(_x.display, _x.window, x, y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XDisplay::SetSize(uint32_t width, uint32_t height)
{
    XWindowAttributes attrs;
    XGetWindowAttributes(_x.display, _x.window, &attrs);
    XResizeWindow(_x.display, _x.window, width + attrs.border_width, height + attrs.border_width);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XDisplay::Show()
{    
    XMapWindow(_x.display, _x.window);
    XRaiseWindow(_x.display, _x.window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XDisplay::EnterMessageLoop()
{
    for (;;)
    {
        while (XPending(_x.display))
        {
            XEvent event;
            XNextEvent(_x.display, &event);
            
            switch (event.type)
            {
                case ConfigureNotify:
                {
                    XConfigureEvent* e = (XConfigureEvent*)&event;
                    mLocationChanged(this, e->x, e->y);
                    mSizeChanged(this, e->width, e->height);
                    break;
                }
                case FocusIn:
                {
                    mActivated(this);
                    break;
                }
                case FocusOut:
                {
                    mDeactivated(this);
                    break;
                }
                case MotionNotify:
                {
                    XMotionEvent* e = (XMotionEvent*)&event;
                    mMouseMove(this, e->x, e->y);
                    break;
                }
                case ButtonPress:
                {
                    XButtonEvent* e = (XButtonEvent*)&event;
                    mMouseButtonDown(this, e->x, e->y, NoesisButton(e->button));
                    break;
                }
                case ButtonRelease:
                {
                    XButtonEvent* e = (XButtonEvent*)&event;
                    mMouseButtonUp(this, e->x, e->y, NoesisButton(e->button));
                    break;
                }
            }
        }

        mRender(this);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void XDisplay::Close()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void* XDisplay::GetNativeHandle() const
{
    return (void*)&_x;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t XDisplay::GetClientWidth() const
{
    XWindowAttributes attrs;
    XGetWindowAttributes(_x.display, _x.window, &attrs);
    return attrs.width;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t XDisplay::GetClientHeight() const
{
    XWindowAttributes attrs;
    XGetWindowAttributes(_x.display, _x.window, &attrs);
    return attrs.height;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_IMPLEMENT_REFLECTION(XDisplay)
{
    NsMeta<TypeId>("XDisplay");
    NsMeta<Category>("Display");
}
