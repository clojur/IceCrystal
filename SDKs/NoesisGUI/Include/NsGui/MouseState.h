////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MOUSESTATE_H__
#define __GUI_MOUSESTATE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputEnums.h>
#include <NsDrawing/Point.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API MouseState
{
    Point position;
    MouseButtonState leftButton;
    MouseButtonState rightButton;
    MouseButtonState middleButton;
    MouseButtonState xButton1Button;
    MouseButtonState xButton2Button;

    MouseState();

    MouseState(const Point& position, MouseButtonState leftButton, 
        MouseButtonState rightButton, MouseButtonState middleButton,
        MouseButtonState xButton1Button, MouseButtonState xButton2Button);
};

NS_WARNING_POP

}

#endif
