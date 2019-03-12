////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __APP_TSF_H__
#define __APP_TSF_H__


#include <NsCore/Noesis.h>


namespace NoesisApp
{
namespace TSF
{

/// Initializes Text Services Framework (TSF) API, for IME editing
void Init();

/// Shutdowns TSF
void Shutdown();

/// Invoke this function each time the window is activated (WM_ACTIVATE)
void ActivateWindow(void* hWnd);

/// Invoke this function each time the window is deactivated (WA_INACTIVE)
void DeactivateWindow(void* hWnd);

/// Invoke this function each time the window moves (WM_MOVE) or a resize finishes (WM_EXITSIZEMOVE)
void MoveWindow(void* hWnd);

}
}

#endif
