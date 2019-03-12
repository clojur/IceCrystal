////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #1051]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_AMBIENTSTACK_H__
#define __GUI_AMBIENTSTACK_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/Ptr.h>
#include <NsCore/Map.h>
#include <NsCore/Stack.h>


namespace Noesis
{

class BaseComponent;
class TypeClass;
class Symbol;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// AmbientStack. Provides a multistack to store values indexed by Class.Property pairs
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API AmbientStack
{
public:
    /// Constructor
    AmbientStack();
    
    // Destructor
    ~AmbientStack();

    /// Adds a value for the typeClass.property passed
    void PushValue(const TypeClass* typeClass, Symbol property, 
        const Ptr<BaseComponent>& value);
    
    /// Pops the latest added value for typeClass.property from the stack
    void PopValue(const TypeClass* typeClass, Symbol property);
    
    /// Get the lastest added value for typeClass.property without removing from the stack
    const Ptr<BaseComponent>& GetValue(const TypeClass* typeClass, 
        Symbol property) const;
    
    /// Checks if there is a value for typeClass.property in the stack
    bool HasValue(const TypeClass* typeClass, Symbol property) const;

    /// Returns the number of ambient properties stored. Only for debug use
    uint32_t GetNumAmbients() const;

private:
    // NOTE: Can't use multimap because the order in which duplicate values are retrieved is not 
    // guaranteed
    typedef eastl::pair<const TypeClass*, Symbol> Key;
    typedef NsStack<Ptr<BaseComponent> > Values;
    typedef NsMap<Key, Values> Ambients;
    Ambients mAmbients;
};

NS_WARNING_POP

}

#endif
