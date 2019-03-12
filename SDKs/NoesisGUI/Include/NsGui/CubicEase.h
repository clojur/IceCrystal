////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CUBICEASE_H__
#define __GUI_CUBICEASE_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/EasingFunctionBase.h>
#include <NsGui/AnimationApi.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an easing function that creates an animation that accelerates and/or 
/// decelerates using the formula f(t) = t3
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.animation.cubicease.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_ANIMATION_API CubicEase: public EasingFunctionBase
{
public:
    CubicEase();
    ~CubicEase();

    /// From Freezable
    //@{
    Ptr<CubicEase> Clone() const;
    Ptr<CubicEase> CloneCurrentValue() const;
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
    
    NS_DECLARE_REFLECTION(CubicEase, EasingFunctionBase)
};

}

#endif
