////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VALUETARGETPROVIDER_H__
#define __GUI_VALUETARGETPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>


namespace Noesis
{

class DependencyProperty;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ValueTargetProvider
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ValueTargetProvider
{
public:
    ValueTargetProvider(BaseComponent* targetObject,
        const DependencyProperty* targetProperty);

    BaseComponent* GetTargetObject() const;
    const DependencyProperty* GetTargetProperty() const;

private:
    Ptr<BaseComponent> mTargetObject;
    const DependencyProperty* mTargetProperty;
};

NS_WARNING_POP

}

#endif
