////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_XAMLREADERPROVIDER_H__
#define __GUI_XAMLREADERPROVIDER_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ValueTargetProvider.h>
#include <NsCore/Symbol.h>


namespace Noesis
{

// Forward declaration
//@{
NS_INTERFACE IXamlContext;
//@}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// XamlReaderProvider
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API XamlReaderProvider: public ValueTargetProvider
{
public:
    XamlReaderProvider(BaseComponent* targetObject, NsSymbol targetProperty, 
        const TypeClass* propertyOwner, const IXamlContext* context);

    const IXamlContext* GetContext() const;
    NsSymbol GetPropertyName() const;
    const TypeClass* GetPropertyOwner() const;

private:
    const IXamlContext* mContext;
    NsSymbol mTargetPropertyName;
    const TypeClass* mPropertyOwner;
};

}

#endif
