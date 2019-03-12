////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GEOMETRYGROUP_H__
#define __GUI_GEOMETRYGROUP_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Geometry.h>
#include <NsGui/Enums.h>


namespace Noesis
{

struct NotifyCollectionChangedEventArgs;

template<class T> class TypedFreezableCollection;
typedef Noesis::TypedFreezableCollection<Noesis::Geometry> GeometryCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a composite geometry, composed of other Geometry objects. 
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.geometrygroup.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API GeometryGroup: public Geometry
{
public:
    GeometryGroup();
    ~GeometryGroup();

    /// Gets or sets the GeometryCollection that contains the objects that define this GeometryGroup
    // TODO [srodriguez] Create a GeometryCollection
    //@{
    GeometryCollection* GetChildren() const;
    void SetChildren(GeometryCollection* collection);
    //@}

    /// Gets or sets how the intersecting areas of the objects contained in this GeometryGroup are 
    /// combined.
    //@{
    FillRule GetFillRule() const;
    void SetFillRule(FillRule rule);
    //@}

    /// From Geometry
    //@{
    bool IsEmpty() const;
    //@}

    /// From Freezable
    //@{
    Ptr<GeometryGroup> Clone() const;
    Ptr<GeometryGroup> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ChildrenProperty;
    static const DependencyProperty* FillRuleProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    /// From Freezable
    //@{
    bool FreezeCore(bool isChecking);
    Ptr<Freezable> CreateInstanceCore() const;
    //@}


    /// From Geometry
    //@{
    Rect GetRenderBoundsOverride(Pen* pen) const;
    bool FillContainsOverride(const Point& point) const;
    bool StrokeContainsOverride(Pen* pen, const Point& point) const;
    //@}

private:
    void OnChildrenChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void RegisterChildren(GeometryCollection* children);
    void RegisterChild(BaseComponent* child);
    void UnregisterChildren(GeometryCollection* children);
    void UnregisterChild(BaseComponent* child);

    void OnChildChanged(Freezable* obj, FreezableEventReason reason);

    void DisconnectChildren();

private:
    enum UpdateFlags
    {
        UpdateFlags_Children = Geometry::UpdateFlags_Sentinel,
        UpdateFlags_FillRule
    };

    NS_DECLARE_REFLECTION(GeometryGroup, Geometry)
};

NS_WARNING_POP

}

#endif
