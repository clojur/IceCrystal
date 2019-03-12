////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_VISUALBRUSH_H__
#define __GUI_VISUALBRUSH_H__


#include <NsCore/Noesis.h>
#include <NsGui/TileBrush.h>
#include <NsGui/IRenderProxyCreator.h>


namespace Noesis
{

class Visual;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Paints an area with a Visual.
///
/// Current VisualBrush implementation only works when the referred Visual is contained within
/// the logical tree.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.visualbrush.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API VisualBrush: public TileBrush
{
public:
    /// Default constructor
    VisualBrush();
    
    /// Destructor
    ~VisualBrush();

    /// Gets or sets the brush's content
    //@{
    Visual* GetVisual() const;
    void SetVisual(Visual* visual);
    //@}

    /// From Freezable
    //@{
    Ptr<VisualBrush> Clone() const;
    Ptr<VisualBrush> CloneCurrentValue() const;
    //@}

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex);
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* VisualProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

private:
    void OnVisualChanged();
    uint32_t UpdateVisual(RenderTreeUpdater& updater);

private:
    enum UpdateFlags
    {
        UpdateFlags_Visual = TileBrush::UpdateFlags_Sentinel
    };

    bool mUpdatingVisual;

    NS_DECLARE_REFLECTION(VisualBrush, TileBrush)
};

}

#endif
