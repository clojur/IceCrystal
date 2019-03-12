////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SCALETRANSFORM_H__
#define __GUI_SCALETRANSFORM_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Scales an object in the 2D coordinate system.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.scaletransform.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API ScaleTransform: public Transform
{
public:
    ScaleTransform();
    ScaleTransform(float scaleX, float scaleY);
    ~ScaleTransform();

    /// Gets or sets the x-axis scale factor
    //@{
    float GetScaleX() const;
    void SetScaleX(float scaleX);
    //@}
    
    /// Gets or sets the y-axis scale factor
    //@{
    float GetScaleY() const;
    void SetScaleY(float scaleY);
    //@}
    
    /// Gets or sets the x-coordinate of the scale center point
    //@{
    float GetCenterX() const;
    void SetCenterX(float centerX);
    //@}
    
    /// Gets or sets the y-coordinate of the scale center point
    //@{
    float GetCenterY() const;
    void SetCenterY(float centerY);
    //@}

    /// From Transform
    //@{
    Transform2f GetTransform() const;
    //@}

    /// From Freezable
    //@{
    Ptr<ScaleTransform> Clone() const;
    Ptr<ScaleTransform> CloneCurrentValue() const;
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
    static const DependencyProperty* CenterXProperty;
    static const DependencyProperty* CenterYProperty;
    static const DependencyProperty* ScaleXProperty;
    static const DependencyProperty* ScaleYProperty;
    //@}

protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& e);
    //@}

    /// From Freezable
    //@{
    Ptr<Freezable> CreateInstanceCore() const;
    //@}

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_CenterX,
        UpdateFlags_CenterY,
        UpdateFlags_ScaleX,
        UpdateFlags_ScaleY
    };

    NS_DECLARE_REFLECTION(ScaleTransform, Transform)
};

}

#endif
