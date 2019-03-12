////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CIRCLEEASE_H__
#define __GUI_CIRCLEEASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that creates an animation that accelerates and/or
/// decelerates using a circular function.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.circleease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API CircleEase: public EasingFunctionBase
{
public:
    CircleEase();
    ~CircleEase();

    /// From Freezable
    //@{
    Ptr<CircleEase> Clone() const;
    Ptr<CircleEase> CloneCurrentValue() const;
    //@}

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

    /// From EasingFunctionBase
    //@{
    float EaseInCore(float normalizedTime);
    //@}
    
    NS_DECLARE_REFLECTION(CircleEase, EasingFunctionBase)
};

}

#endif
