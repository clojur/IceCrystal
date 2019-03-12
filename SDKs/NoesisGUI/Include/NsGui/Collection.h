////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLLECTION_H__
#define __GUI_COLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/INotifyCollectionChanged.h>
#include <NsGui/INotifyPropertyChanged.h>
#include <NsGui/IList.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IUITreeNode.h>
#include <NsCore/ReflectionImplement.h>
#include <NsCore/TypeId.h>
#include <NsCore/NSTLForwards.h>
#include <NsCore/Vector.h>
#include <NsCore/String.h>
#include <NsCore/DynamicCast.h>
#include <NsCore/Error.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides the base class for a generic collection.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Collection: public BaseComponent, public IList,
    public INotifyCollectionChanged, public INotifyPropertyChanged, public IComponentInitializer,
    public IUITreeNode
{
public:
    /// Constructor
    Collection();
    Collection(IUITreeNode* owner);

    /// Destructor
    virtual ~Collection();

    // From IList
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

    // From INotifyCollectionChanged
    //@{
    NotifyCollectionChangedEventHandler& CollectionChanged() override;
    //@}

    // From INotifyPropertyChanged
    //@{
    PropertyChangedEventHandler& PropertyChanged() override;
    //@}

    // From IComponentInitializer
    //@{
    bool IsInitialized() const override;
    void Init() override;
    //@}

    // From IUITreeNode
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
    /// Allows inheritors to perform specific actions when items are added or removed
    //@{
    virtual void OnItemAdded(BaseComponent* item, uint32_t index);
    virtual void OnItemRemoved(BaseComponent* item, uint32_t index);
    //@}

    // To check before adding an element
    virtual bool CheckItem(BaseComponent* item) const;

    // Helper function to get item's type name taking into account boxed values
    static const char* GetItemType(BaseComponent* item);

    // Helper function to get parsing context info
    static NsString GetParsingLineInfo();

private:
    int GetCount() const; // to expose Count as int property like WPF

private:
    bool mIsInitialized;
    IUITreeNode* mOwner;

    typedef NsVector<Ptr<BaseComponent> > Items;
    Items mItems;

    NotifyCollectionChangedEventHandler mCollectionChanged;
    PropertyChangedEventHandler mPropertyChanged;

    NS_DECLARE_REFLECTION(Collection, BaseComponent)
};

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_GUI_CORE_API NsSymbol IdOfTypedCollection(const TypeClass* type);

////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class TypedCollection: public Collection
{
public:
    TypedCollection() { }
    TypedCollection(IUITreeNode* owner): Collection(owner) { }

protected:
    /// From Collection
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
                GetItemType(item), GetClassType()->GetName());
            return false;
        }

        return true;
    }
    //@}

    NS_IMPLEMENT_INLINE_REFLECTION(TypedCollection, Collection)
    {
        NsMeta<TypeId>(IdOfTypedCollection(TypeOf<T>()).GetStr());
    }
};

NS_WARNING_POP

}

#endif
