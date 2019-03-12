////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IEXPRESSION_H__
#define __GUI_IEXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/IExpressionTypes.h>


namespace Noesis
{

template<class T> class Ptr;
class BaseComponent;
class DependencyObject;
class DependencyProperty;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// IExpression
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IExpression: public Interface
{
    /// Evaluates expression when applied to the specified target object
    virtual Ptr<BaseComponent> Evaluate() const = 0;
    
    /// Applies expression to a new target, cloning the expression itself if necessary
    virtual Ptr<IExpression> Reapply(DependencyObject* targetObject,
        const DependencyProperty* targetProperty) const = 0;

    /// Executes the set
    virtual BeforeSetAction BeforeSet(DependencyObject* obj, const DependencyProperty* dp, 
        const void* value, bool valueHasChanged) = 0;
        
    /// Executed after set is completed and the property changed has been notified
    virtual void AfterSet(DependencyObject* obj, const DependencyProperty* dp, 
        const void* value, bool valueHasChanged) = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IExpression, Interface)
};

}

#endif
