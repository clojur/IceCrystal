////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TRANSLATETRANSFORM_H__
#define __GUI_TRANSLATETRANSFORM_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Translates (moves) an object in the 2D coordinate system.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.translatetransform.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TranslateTransform: public Transform
{
public:
    TranslateTransform();
    TranslateTransform(float x, float y);
    ~TranslateTransform();

    /// Gets or sets the x-axis displacement
    //@{
    float GetX() const;
    void SetX(float x);
    //@}
    
    /// Gets or sets the y-axis displacement
    //@{
    float GetY() const;
    void SetY(float y);
    //@}

    /// From Freezable
    //@{
    Ptr<TranslateTransform> Clone() const;
    Ptr<TranslateTransform> CloneCurrentValue() const;
    //@}

    /// From Transform
    //@{
    Transform2f GetTransform() const;
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
    static const DependencyProperty* XProperty;
    static const DependencyProperty* YProperty;
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
        UpdateFlags_X,
        UpdateFlags_Y
    };

    NS_DECLARE_REFLECTION(TranslateTransform, Transform)
};

}

#endif
