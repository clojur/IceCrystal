////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DICTIONARYKEYPROPERTYMETADATA_H__
#define __GUI_DICTIONARYKEYPROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/TypeMetaData.h>
#include <NsCore/Symbol.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// DictionaryKeyPropertyMetaData
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API DictionaryKeyPropertyMetaData: public TypeMetaData
{
public:
    /// Constructor
    DictionaryKeyPropertyMetaData(NsSymbol property);

    /// Gets content property
    inline NsSymbol GetProperty() const;

private:
    NsSymbol mProperty;
    
private:
    NS_DECLARE_REFLECTION(DictionaryKeyPropertyMetaData, TypeMetaData)
};

}

#include <NsGui/DictionaryKeyPropertyMetaData.inl>

#endif
