////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_GRADIENTSTOPCOLLECTION_H__
#define __GUI_GRADIENTSTOPCOLLECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/FreezableCollection.h>
#include <NsGui/GradientStop.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a collection of GradientStop objects that can be individually accessed by index.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API GradientStopCollection: public FreezableCollection, public IRenderProxyCreator
{
public:
    /// Constructor
    GradientStopCollection();

    /// Destructor
    ~GradientStopCollection();

    /// From Freezable
    //@{
    Ptr<GradientStopCollection> Clone() const;
    Ptr<GradientStopCollection> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

protected:
    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const override;
    //@}

    /// From FreezableCollection
    //@{
    bool CheckItem(BaseComponent* item) const override;
    void OnItemAdded(BaseComponent* item, uint32_t index) override;
    void OnItemRemoved(BaseComponent* item, uint32_t index) override;
    //@}

private:
    void RegisterChild(BaseComponent* item);
    void UnregisterChild(BaseComponent* item);
    void OnChildChanged(Freezable* obj, FreezableEventReason reason);

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Stops
    };

    NS_DECLARE_REFLECTION(GradientStopCollection, FreezableCollection)
};

NS_WARNING_POP

}

#endif
