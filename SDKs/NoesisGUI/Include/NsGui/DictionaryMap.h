////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #1215]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_DICTIONARYMAP_H__
#define __GUI_DICTIONARYMAP_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Ptr.h>
#include <NsCore/Map.h>
#include <NsGui/IResourceKey.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
// Specialized map for IResourceKey keys
////////////////////////////////////////////////////////////////////////////////////////////////////
struct DictionaryMapLess 
{
    bool operator()(const Ptr<IResourceKey>& a, const Ptr<IResourceKey>& b) const 
    {
        NS_ASSERT(a);
        NS_ASSERT(b);

        return a->IsLessThan(b.GetPtr());
    }
};

typedef NsMap<Ptr<IResourceKey>, Ptr<BaseComponent>, DictionaryMapLess> DictionaryMap;

}

#endif
