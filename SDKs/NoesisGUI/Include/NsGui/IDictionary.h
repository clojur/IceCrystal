////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IDICTIONARY_H__
#define __GUI_IDICTIONARY_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/IResourceKey.h>


namespace Noesis
{

template<class T> class Delegate;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// IDictionary. Interface to a dictionary container of generic items using a string as key.
/// Note: keys are case-sensitive
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IDictionary: public Interface
{
    /// Looks for an element in the dictionary
    /// \param key Search key
    /// \param resource Variable that will store element if it was found
    /// \return True if element was found, false otherwise
    virtual bool Find(IResourceKey* key, BaseComponent*& resource) const = 0;

    /// Gets or sets the element with the specified key
    //@{
    virtual BaseComponent* Get(IResourceKey* key) const = 0;
    virtual void Set(IResourceKey* key, BaseComponent* value) = 0;
    //@}

    /// Adds an element with the provided key and value to the dictionary
    virtual void Add(IResourceKey* key, BaseComponent* value) = 0;

    /// Removes all elements from the dictionary
    virtual void Clear() = 0;

    /// Determines whether the dictionary contains an element with the specified key
    virtual bool Contains(IResourceKey* key) const = 0;

    /// Removes the element with the specified key from the dictionary
    virtual void Remove(IResourceKey* key) = 0;
    
    /// Gets the number of elements in the dictionary
    virtual uint32_t Count() const = 0;
    
    /// Get an iterator to the elements of the dictionary
    typedef Noesis::Delegate<void (IResourceKey*, BaseComponent*)> EnumCallback;
    virtual void EnumKeyValues(const EnumCallback& callback) const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IDictionary, Interface)
};

}

#endif
