////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BINDINGOPERATIONS_H__
#define __GUI_BINDINGOPERATIONS_H__


#include <NsCore/Noesis.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class DependencyObject;
class DependencyProperty;
class BaseBinding;
class Binding;
class BaseBindingExpression;
class BindingExpression;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// BindingOperations: Provides static methods to manipulate bindings, including Binding, 
/// MultiBinding, and PriorityBinding objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API BindingOperations
{
    /// Removes all bindings, including bindings of type Binding, MultiBinding, and PriorityBinding,
    /// from the specified DependencyObject
    static void ClearAllBindings(DependencyObject* target);

    /// Removes the binding from a property if there is one
    static void ClearBinding(DependencyObject* target, const DependencyProperty* dp);

    /// Retrieves the BindingBase object that is set on the specified property
    static BaseBinding* GetBindingBase(DependencyObject* target, const DependencyProperty* dp);

    /// Retrieves the Binding object that is set on the specified property
    static Binding* GetBinding(DependencyObject* target, const DependencyProperty* dp);

    /// Returns the BindingExpressionBase object associated with the specified binding target
    /// property on the specified object
    static BaseBindingExpression* GetBindingExpressionBase(DependencyObject* target,
        const DependencyProperty* dp);

    /// Returns the BindingExpression object associated with the specified binding target property
    /// on the specified object
    static BindingExpression* GetBindingExpression(DependencyObject* target, 
        const DependencyProperty* dp);

    /// Returns a value that indicates whether the specified property is currently data-bound. 
    static bool IsDataBound(DependencyObject* target, const DependencyProperty* dp);

    /// Creates and associates a new instance of BindingExpressionBase with the specified binding 
    /// target property
    static BaseBindingExpression* SetBinding(DependencyObject* target, const DependencyProperty* dp,
        BaseBinding* binding);
};

}

#endif
