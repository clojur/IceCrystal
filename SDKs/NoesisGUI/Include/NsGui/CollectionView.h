////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLLECTIONVIEW_H__
#define __GUI_COLLECTIONVIEW_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/Events.h>
#include <NsGui/ICollectionView.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/Vector.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

NS_INTERFACE IList;
class CollectionViewTest;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// CollectionView
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CollectionView: public BaseComponent, public ICollectionView,
    public INotifyCollectionChanged, public IComponentInitializer, public IUITreeNode
{
public:
    /// Serialization constructor
    CollectionView();

    /// Constructor
    CollectionView(IList* collection);

    /// Destructor
    ~CollectionView();

    /// Returns the underlying unfiltered collection
    IList* GetSourceCollection() const;

    /// Gets the number of records in the view
    uint32_t Count() const;

    /// Retrieves the item at the specified zero-based index in the view
    BaseComponent* GetItemAt(uint32_t index) const;

    /// Returns the index at which the specified item is located; -1 if the item is unknown
    int IndexOf(BaseComponent* item) const;

    /// From ICollectionView
    //@{
    bool CanFilter() const override;
    bool CanGroup() const override;
    bool CanSort() const override;
    bool Contains(BaseComponent* item) const override;
    BaseComponent* CurrentItem() const override;
    int CurrentPosition() const override;
    bool IsCurrentAfterLast() const override;
    bool IsCurrentBeforeFirst() const override;
    bool IsEmpty() const override;
    bool MoveCurrentTo(BaseComponent* item) override;
    bool MoveCurrentToFirst() override;
    bool MoveCurrentToLast() override;
    bool MoveCurrentToNext() override;
    bool MoveCurrentToPosition(int position) override;
    bool MoveCurrentToPrevious() override;
    void Refresh() override;
    EventHandler& CurrentChanged() override;
    //@}

    /// From INotifyCollectionChanged
    //@{
    NotifyCollectionChangedEventHandler& CollectionChanged() override;
    //@}

    /// From IComponentInitializer
    //@{
    bool IsInitialized() const override;
    void Init() override;
    //@}

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(IResourceKey* key,
        bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    void InternalCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);

    void ConnectCollection();
    
    void SetCurrentPosition(int pos);

private:
    friend class CollectionViewTest;

    bool mIsInitialized;
    IUITreeNode* mOwner;

    bool mIsDynamic;
    int mCurrentPosition;

    Ptr<IList> mCollection;

    EventHandler mCurrentChanged;
    NotifyCollectionChangedEventHandler mCollectionChanged;

    NS_DECLARE_REFLECTION(CollectionView, BaseComponent)
};

NS_WARNING_POP

}

#endif
