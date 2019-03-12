////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IPROPERTYVALUECONVERTER_H__
#define __GUI_IPROPERTYVALUECONVERTER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

class DependencyProperty;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// This type of converters use a DependencyProperty to specify the target type
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IPropertyValueConverter: public Interface
{
    virtual const DependencyProperty* GetProperty() const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IPropertyValueConverter, Interface)
};

}

#endif
