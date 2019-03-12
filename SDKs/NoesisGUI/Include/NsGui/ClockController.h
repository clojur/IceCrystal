////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CLOCKCONTROLLER_H__
#define __GUI_CLOCKCONTROLLER_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

class Clock;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

class NS_GUI_ANIMATION_API ClockController: public BaseComponent
{
public:
    ClockController(Clock* clock);
    ~ClockController();

private:
    Ptr<Clock> mClock;

    NS_DECLARE_REFLECTION(ClockController, BaseComponent)
};

NS_WARNING_POP

}

#endif
