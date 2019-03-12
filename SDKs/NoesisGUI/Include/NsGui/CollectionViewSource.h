////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLLECTIONVIEWSOURCE_H__
#define __GUI_COLLECTIONVIEWSOURCE_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/DependencyObject.h>
#include <NsGui/IUITreeNode.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

template<class T> class TypedCollection;
class CollectionView;
class ResourceKeyType;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Proxy of CollectionView to be used from XAML files.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.data.collectionviewsource.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API CollectionViewSource: public DependencyObject, public IUITreeNode
{
public:
    CollectionViewSource();
    ~CollectionViewSource();

    /// Gets or sets the desired view type
    //@{
    ResourceKeyType* GetCollectionViewType() const;
    void SetCollectionViewType(ResourceKeyType* type);
    //@}
    
    /// Gets or sets the collection object from which to create this view
    //@{
    BaseComponent* GetSource() const;
    void SetSource(BaseComponent* source);
    //@}
    
    /// Gets the view object that is currently associated with this instance of CollectionViewSource
    //@{
    CollectionView* GetView() const;
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

public:
    static const DependencyProperty* CollectionViewTypeProperty;
    static const DependencyProperty* SourceProperty;
    static const DependencyProperty* ViewProperty;
    
protected:
    /// From DependencyObject
    //@{
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:
    IUITreeNode* mOwner;

    NS_DECLARE_REFLECTION(CollectionViewSource, DependencyObject)
};

NS_WARNING_POP

}

#endif
