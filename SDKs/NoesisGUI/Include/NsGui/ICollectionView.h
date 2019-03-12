////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ICOLLECTIONVIEW_H__
#define __GUI_ICOLLECTIONVIEW_H__


#include <NsCore/Noesis.h>
#include <NsCore/Interface.h>
#include <NsGui/Events.h>


namespace Noesis
{

class BaseComponent;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ICollectionView: Enables collections to have the functionalities of current record management, 
/// custom sorting, filtering, and grouping. 
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE ICollectionView: public Interface
{
    /// Gets a value that indicates whether this view supports filtering via the Filter property
    virtual bool CanFilter() const = 0;

    /// Gets a value that indicates whether this view supports grouping via GroupDescriptions 
    virtual bool CanGroup() const = 0;

    /// Gets a value that indicates whether this view supports sorting via SortDescriptions 
    virtual bool CanSort() const = 0;

    /// Returns a value that indicates whether a given item belongs to this collection view
    virtual bool Contains(BaseComponent* item) const = 0;

    /// Gets the current item in the view
    virtual BaseComponent* CurrentItem() const = 0;

    /// Gets the ordinal position of the CurrentItem within the view
    virtual int CurrentPosition() const = 0;

    /// Gets a value that indicates whether the CurrentItem of the view is beyond the end of the 
    /// collection
    virtual bool IsCurrentAfterLast() const = 0;

    /// Gets a value that indicates whether the CurrentItem of the view is beyond the beginning of 
    /// the collection
    virtual bool IsCurrentBeforeFirst() const = 0;

    /// Returns a value that indicates whether the resulting view is empty. 
    virtual bool IsEmpty() const = 0;

    /// Move current item methods
    //@{
    virtual bool MoveCurrentTo(BaseComponent* item) = 0;
    virtual bool MoveCurrentToFirst() = 0;
    virtual bool MoveCurrentToLast() = 0;
    virtual bool MoveCurrentToNext() = 0;
    virtual bool MoveCurrentToPosition(int position) = 0;
    virtual bool MoveCurrentToPrevious() = 0;
    //@}

    virtual void Refresh() = 0;

    virtual EventHandler& CurrentChanged() = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(ICollectionView, Interface)
};

}

#endif
