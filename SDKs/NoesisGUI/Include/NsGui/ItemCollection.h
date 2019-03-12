////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_ITEMCOLLECTION_H__
#define __GUI_ITEMCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>
#include <NsGui/CoreApi.h>
#include <NsGui/ICollectionView.h>
#include <NsGui/IList.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IUITreeNode.h>


namespace Noesis
{

class ItemsControl;
class Collection;
class CollectionView;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// ItemCollection
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ItemCollection: public BaseComponent, public ICollectionView, 
    public IList, public INotifyCollectionChanged, public IComponentInitializer, 
    public IUITreeNode
{
public:
    ItemCollection();
    ItemCollection(ItemsControl* itemsControl);
    ~ItemCollection();

    /// Sets ItemCollection owner, it would be an ItemsControl object
    void SetItemsControlOwner(ItemsControl* itemsControl);
    
    /// To be called from ItemsControl when the ItemsSource is assigned with a CollectionView
    void SetView(CollectionView* view);
    
    /// Check if the itemcollection is related to an external collection, and so it is readonly
    bool IsReadOnly() const;
    
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

    /// From IList
    //@{
    uint32_t Count() const override;
    BaseComponent* Get(uint32_t index) const override;
    void Set(uint32_t index, BaseComponent* item) override;
    void Set(uint32_t index, const char* item) override;
    uint32_t Add(BaseComponent* item) override;
    uint32_t Add(const char* item) override;
    void Clear() override;
    int IndexOf(BaseComponent* item) const override;
    void Insert(uint32_t index, BaseComponent* item) override;
    void Insert(uint32_t index, const char* item) override;
    void Remove(BaseComponent* item) override;
    void RemoveAt(uint32_t index) override;
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
    bool CheckReadOnly() const;

    void ConnectChildren() const;
    void DisconnectChildren() const;

    void CreateCollections();

    void OnCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnCurrentChanged(BaseComponent* sender, const EventArgs& args);

    void ConnectCollection();
    void DisconnectCollection();

    void RegisterDelegates();
    void UnregisterDelegates();
    
private:
    bool mIsInitialized;
    IUITreeNode* mOwner;

    ItemsControl* mItemsControl;
    
    Ptr<Collection> mLocalCollection;
    Ptr<CollectionView> mEffectiveCollectionView;
    
    EventHandler mCurrentChanged;
    NotifyCollectionChangedEventHandler mCollectionChanged;

    NS_DECLARE_REFLECTION(ItemCollection, BaseComponent)
};

NS_WARNING_POP

}

#endif
