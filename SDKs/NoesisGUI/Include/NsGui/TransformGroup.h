////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TRANSFORMGROUP_H__
#define __GUI_TRANSFORMGROUP_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform.h>


namespace Noesis
{

struct NotifyCollectionChangedEventArgs;

template <class T> class TypedFreezableCollection;
typedef Noesis::TypedFreezableCollection<Noesis::Transform> TransformCollection;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a composite Transform composed of other Transform objects.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.transformgroup.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TransformGroup: public Transform
{
public:
    TransformGroup();
    ~TransformGroup();

    /// Gets the number of transforms that compose this group
    uint32_t GetNumChildren() const;

    /// Gets the specified transform child
    Transform* GetChild(uint32_t index) const;

    /// Gets or sets the TransformCollection that defines this TransformGroup
    //@{
    TransformCollection* GetChildren() const;
    void SetChildren(TransformCollection* children);
    //@}

    /// From Transform
    //@{
    Transform2f GetTransform() const;
    //@}

    /// From Freezable
    //@{
    Ptr<TransformGroup> Clone() const;
    Ptr<TransformGroup> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UnregisterRenderer(ViewId viewId);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ChildrenProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& e);
    //@}

    /// From Freezable
    //@{
    bool FreezeCore(bool isChecking);
    Ptr<Freezable> CreateInstanceCore() const;
    //@}


private:
    void OnChildrenChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void RegisterChildren(TransformCollection* children);
    void RegisterChild(BaseComponent* child);
    void UnregisterChildren(TransformCollection* children);
    void UnregisterChild(BaseComponent* child);

    void OnChildChanged(Freezable* obj, FreezableEventReason reason);

    void DisconnectChildren();

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Children
    };

    NS_DECLARE_REFLECTION(TransformGroup, Transform)
};

}

#endif
