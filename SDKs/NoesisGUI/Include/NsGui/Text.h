
////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXT_H__
#define __GUI_TEXT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

class DependencyProperty;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Adds stroke capabilities to text controls.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API Text
{
    /// Dependency properties
    //@{
    static const DependencyProperty* StrokeProperty;
    static const DependencyProperty* StrokeThicknessProperty;
    //@}

    NS_DECLARE_REFLECTION(Text, NoParent)
};

}

#endif
