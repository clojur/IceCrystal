////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FREEZABLECOLLECTION_H__
#define __GUI_FREEZABLECOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Freezable.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/IList.h>
#include <NsGui/IUITreeNode.h>
#include <NsCore/TypeId.h>
#include <NsCore/String.h>
#include <NsCore/Vector.h>
#include <NsCore/DynamicCast.h>
#include <NsCore/Error.h>
#include <NsCore/ReflectionImplement.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// FreezableCollection. Base collection implementation for storing Freezables
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FreezableCollection: public Freezable, public IList,
    public INotifyCollectionChanged, public IUITreeNode
{
public:
    /// Constructor
    FreezableCollection();
    FreezableCollection(IUITreeNode* owner);

    /// Destructor
    virtual ~FreezableCollection();

    /// From Freezable
    //@{
    Ptr<FreezableCollection> Clone() const;
    Ptr<FreezableCollection> CloneCurrentValue() const;
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
    bool Contains(BaseComponent* item) const override;
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

protected:
    // To check an item before adding it to the collection
    virtual bool CheckItem(BaseComponent* item) const;

    /// Allows inheritors to perform specific actions when items are added or removed
    //@{
    virtual void OnItemAdded(BaseComponent* item, uint32_t index);
    virtual void OnItemRemoved(BaseComponent* item, uint32_t index);
    //@}

    // Helper function to get parsing context info
    static NsString GetParsingLineInfo();

    /// From Freezable
    //@{
    bool FreezeCore(bool isChecking) override;
    void CloneCore(const Freezable* source) override;
    void CloneCurrentValueCore(const Freezable* source) override;
    void GetAsFrozenCore(const Freezable* source) override;
    void GetCurrentValueAsFrozenCore(const Freezable* source) override;
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}


    /// From DependencyObject
    //@{
    void OnInit() override;
    //@}

private:
    bool CheckFrozen();

private:
    IUITreeNode* mOwner;

    typedef NsVector<Ptr<BaseComponent> > Items;
    Items mItems;
    
    NotifyCollectionChangedEventHandler mEvent;

    NS_DECLARE_REFLECTION(FreezableCollection, Freezable)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_GUI_CORE_API NsSymbol IdOfTypedCollection(const TypeClass* type);

////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class TypedFreezableCollection: public FreezableCollection
{
public:
    /// From Freezable
    //@{
    Ptr<TypedFreezableCollection<T>> Clone() const
    {
        return NsStaticCast<Ptr<TypedFreezableCollection<T>>>(Freezable::Clone());
    }

    Ptr<TypedFreezableCollection<T>> CloneCurrentValue() const
    {
        return NsStaticCast<Ptr<TypedFreezableCollection<T>>>(Freezable::CloneCurrentValue());
    }
    //@}

protected:
    /// From FreezableCollection
    //@{
    bool CheckItem(BaseComponent* item) const override
    {
        if (!ParentClass::CheckItem(item))
        {
            return false;
        }

        if (NsDynamicCast<T*>(item) == 0)
        {
            NS_ERROR("%sCan't add a '%s' to a '%s'", GetParsingLineInfo().c_str(),
                item->GetClassType()->GetName(), GetClassType()->GetName());
            return false;
        }

        return true;
    }
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override
    {
        return *new TypedFreezableCollection<T>();
    }
    //@}

    NS_IMPLEMENT_INLINE_REFLECTION(TypedFreezableCollection, FreezableCollection)
    {
        NsMeta<TypeId>(IdOfTypedCollection(TypeOf<T>()).GetStr());
    }
};

NS_WARNING_POP

}

#endif
