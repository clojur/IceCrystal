////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ILIST_H__
#define __GUI_ILIST_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>


namespace Noesis
{

template<class T> class Ptr;
class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// IList. Represents a generic collection of objects that can be individually accessed by index.
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IList: public Interface
{
    /// Gets the number of items in the collection
    virtual uint32_t Count() const = 0;

    /// Gets or sets the item at the specified index
    //@{
    virtual BaseComponent* Get(uint32_t index) const = 0;
    virtual void Set(uint32_t index, BaseComponent* item) = 0;
    virtual void Set(uint32_t index, const char* item) = 0;
    //@}

    /// Adds an item to the collection
    /// \return It returns the insertion position in the collection
    virtual uint32_t Add(BaseComponent* item) = 0;
    virtual uint32_t Add(const char* item) = 0;

    /// Removes all elements from the collection
    virtual void Clear() = 0;

    /// Determines whether the collection contains a specific item
    virtual bool Contains(BaseComponent* item) const = 0;

    /// Determines the index of a specific item in the collection
    /// \return It returns -1 if the item is not found
    virtual int IndexOf(BaseComponent* item) const = 0;

    /// Inserts an item to the collection at the specified index
    virtual void Insert(uint32_t index, BaseComponent* item) = 0;
    virtual void Insert(uint32_t index, const char* item) = 0;

    /// Removes the first occurrence of a specific item from the collection
    virtual void Remove(BaseComponent* item) = 0;

    /// Removes the collection item at the specified index
    virtual void RemoveAt(uint32_t index) = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IList, Interface)
};

}

#endif
