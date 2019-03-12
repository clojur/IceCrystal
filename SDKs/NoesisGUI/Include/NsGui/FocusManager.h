////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FOCUSMANAGER_H__
#define __GUI_FOCUSMANAGER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>


namespace Noesis
{

template<class T> class Ptr;
class DependencyObject;
class DependencyProperty;
class RoutedEvent;
class UIElement;

class NS_GUI_CORE_API FocusManager
{
public:
    /// Gets or sets the element with logical focus within the specified focus scope
    //@{
    static UIElement* GetFocusedElement(DependencyObject* element);
    static void SetFocusedElement(DependencyObject* element, UIElement* value);
    //@}

    /// Determines the closest ancestor of the specified element that has IsFocusScope set to true
    static DependencyObject* GetFocusScope(DependencyObject* element);

    /// Determines whether the specified DependencyObject is a focus scope
    //@{
    static bool GetIsFocusScope(DependencyObject* element);
    static void SetIsFocusScope(DependencyObject* element, bool value);
    //@}

    /// Dependency properties
    //@{
    static const DependencyProperty* FocusedElementProperty;
    static const DependencyProperty* IsFocusScopeProperty;
    //@}

    /// Routed events
    //@{
    static const RoutedEvent* LostFocusEvent;
    static const RoutedEvent* GotFocusEvent;
    //@}

private:
    FocusManager();

    NS_DECLARE_REFLECTION(FocusManager, NoParent)
};

}

#endif
