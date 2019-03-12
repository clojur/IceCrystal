////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IDICTIONARYINDEXER_H__
#define __GUI_IDICTIONARYINDEXER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

template<class T> class Ptr;
class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an indexer object that can be accessed with a string key.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IDictionaryIndexer: public Interface
{
    /// Gets or sets the item associated with the specified key
    /// \return False if index is out of bounds
    //@{
    virtual bool TryGet(const char* key, BaseComponent*& item) const = 0;
    virtual bool TrySet(const char* key, BaseComponent* item) = 0;
    virtual bool TrySet(const char* key, const char* item) = 0;
    //@}

    NS_IMPLEMENT_INLINE_REFLECTION_(IDictionaryIndexer, Interface)
};

}

#endif
