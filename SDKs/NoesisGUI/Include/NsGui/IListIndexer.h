////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ILISTINDEXER_H__
#define __GUI_ILISTINDEXER_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

template<class T> class Ptr;
class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents an indexer object that can be accessed with an integer index.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IListIndexer: public Interface
{
    /// Gets or sets the item at the specified index
    /// \return False if index is out of bounds
    //@{
    virtual bool TryGet(uint32_t index, BaseComponent*& item) const = 0;
    virtual bool TrySet(uint32_t index, BaseComponent* item) = 0;
    virtual bool TrySet(uint32_t index, const char* item) = 0;
    //@}

    NS_IMPLEMENT_INLINE_REFLECTION_(IListIndexer, Interface)
};

}

#endif
