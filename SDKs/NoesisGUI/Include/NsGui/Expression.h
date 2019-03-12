////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_EXPRESSION_H__
#define __GUI_EXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsGui/IExpression.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base implementation for all expressions.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API Expression: public BaseComponent, public IExpression
{
public:
    /// Constructor
    Expression();

    /// Destructor
    virtual ~Expression() = 0;

    /// From IExpression
    //@{
    Ptr<BaseComponent> Evaluate() const override;
    Ptr<IExpression> Reapply(DependencyObject* targetObject,
        const DependencyProperty* targetProperty) const override;
    BeforeSetAction BeforeSet(DependencyObject* obj, const DependencyProperty* prop,
        const void* value, bool valueHasChanged) override;
    void AfterSet(DependencyObject* obj, const DependencyProperty* prop, const void* value,
        bool valueHasChanged) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

    NS_DECLARE_REFLECTION(Expression, BaseComponent)
};

NS_WARNING_POP

}

#endif
