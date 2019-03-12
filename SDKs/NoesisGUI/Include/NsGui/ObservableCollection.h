////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_OBSERVABLECOLLECTION_H__
#define __GUI_OBSERVABLECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/Collection.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a dynamic data collection that provides notifications when items get added or
/// removed. It is frequently used in data binding to display a collection of records within an
/// *ItemsControl* such as a *ListBox*, *ListView* or *TreeView*.
///
/// .. code-block:: xml
///
///     <ListBox Width="200"
///         ItemsSource="{Binding Source={StaticResource NameListData}}"
///         ItemTemplate="{StaticResource NameItemTemplate}" />
///
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T> class ObservableCollection: public Collection
{
public:
    inline T* Get(uint32_t index) const;
    inline void Set(uint32_t index, T* item);
    inline uint32_t Add(T* item);
    inline bool Contains(T* item) const;
    inline int IndexOf(T* item) const;
    inline void Insert(uint32_t index, T* item);
    inline void Remove(T* item);

private:
    using Collection::Set;
    using Collection::Add;
    using Collection::Contains;
    using Collection::IndexOf;
    using Collection::Insert;
    using Collection::Remove;

    NS_IMPLEMENT_INLINE_REFLECTION_(ObservableCollection, Collection)
};

}

#include <NsGui/ObservableCollection.inl>

#endif
