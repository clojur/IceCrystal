////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASEVALUECONVERTER_H__
#define __GUI_BASEVALUECONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/CoreApi.h>
#include <NsGui/IValueConverter.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base class for converters
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseValueConverter: public BaseComponent, public IValueConverter
{
public:
    /// From IValueConverter
    //@{
    bool TryConvert(BaseComponent* value, const Type* targetType,
        BaseComponent* parameter, Ptr<BaseComponent>& result) override;
    bool TryConvertBack(BaseComponent* value, const Type* targetType,
        BaseComponent* parameter, Ptr<BaseComponent>& result) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    NS_DECLARE_REFLECTION(BaseValueConverter, BaseComponent)
};

NS_WARNING_POP

}

#endif
