////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_RESOURCEDICTIONARY_H__
#define __GUI_RESOURCEDICTIONARY_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/DictionaryMap.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/BaseDictionary.h>
#include <NsGui/INameScope.h>
#include <NsGui/IComponentInitializer.h>
#include <NsGui/IUITreeNode.h>
#include <NsCore/HashMap.h>


namespace Noesis
{

class ResourceDictionary;
class ResourceDictionaryTest;
struct NotifyCollectionChangedEventArgs;

template <class T> class TypedCollection;
typedef Noesis::TypedCollection<Noesis::ResourceDictionary> ResourceDictionaryCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides a hash table that contains resources used by the UI.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.resourcedictionary.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ResourceDictionary: public BaseDictionary, public INameScope,
    public IUITreeNode
{
public:
    /// Constructor
    ResourceDictionary();

    /// Destructor
    ~ResourceDictionary();

    /// Indicates if this dictionary is read-only
    bool IsReadOnly() const;

    /// Indicates if this dictionary or any of the merged dictionaries contains a resource
    bool HasResourcesDefined() const;

    /// Gets a collection of the ResourceDictionary dictionaries that constitute the various 
    /// resource dictionaries in the merged dictionaries.
    ResourceDictionaryCollection* GetMergedDictionaries() const;

    /// Get or set source file for dictionary
    //@{
    const char* GetSource() const;
    void SetSource(const char* source);
    //@}

    /// From IDictionary
    //@{
    bool Find(IResourceKey* key, BaseComponent*& resource) const override;
    BaseComponent* Get(IResourceKey* key) const override;
    void Set(IResourceKey* key, BaseComponent* value) override;
    void Add(IResourceKey* key, BaseComponent* value) override;
    void Clear() override;
    bool Contains(IResourceKey* key) const override;
    void Remove(IResourceKey* key) override;
    /// The number of entries of the base dictionary (excluding merged dictionaries)
    uint32_t Count() const override;
    /// Enumerates the entries of the base dictionary (excluding merged dictionaries)
    void EnumKeyValues(const EnumCallback& callback) const override;
    //@}

    /// From INameScope
    //@{
    BaseComponent* FindName(const char* name) const override;
    void RegisterName(const char* name, BaseComponent* object) override;
    void UnregisterName(const char* name) override;
    void UpdateName(const char* name, BaseComponent* object) override;
    INameScope::ChangedDelegate& NameScopeChanged() override;
    //@}

    // Templated version of FindName
    // \remarks Asserts that returned object implements specified type
    template<class T>
    T* FindName(const char* name) const
    {
        return NsStaticCast<T*>(FindName(name));
    }

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
    friend class UISystem;
    friend class Style;
    friend class FrameworkTemplate;
    friend class ResourceDictionaryTest;

    /// Makes this dictionary (and merged dictionaries) read-only
    void Seal();

    /// Item management
    //@{
    void OnItemAdded(BaseComponent* item);
    void OnItemRemoved(BaseComponent* item);
    void OnAllItemsRemoved();
    //@}

    /// Tries to get an object from the map
    BaseComponent* TryGet(IResourceKey* key) const;

    /// Clears all dictionary elements, including merged dictionaries
    void Reset();

    /// Load source dictionary resource
    void LoadSource();

    bool CheckReadOnly() const;
    bool CheckItemKey(IResourceKey* key, BaseComponent* item) const;

    /// Called when a dictionary is added to be merged
    void OnMergedDictionariesChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnAddMergedDictionary(ResourceDictionary* dict);
    void OnRemoveMergedDictionary(ResourceDictionary* dict);
    void OnRemoveMergedDictionaryForMerged(ResourceDictionary* dict);

    void EnsureMergedDictionaries() const;

private:
    IUITreeNode* mOwner;

    NsString mSource;

    struct Data;
    Ptr<Data> mData;

    bool mIsReadOnly;

    NS_DECLARE_REFLECTION(ResourceDictionary, BaseDictionary)
};

NS_WARNING_POP

}

#endif
