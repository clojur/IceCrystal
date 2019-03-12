////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MATRIXTRANSFORM_H__
#define __GUI_MATRIXTRANSFORM_H__


#include <NsCore/Noesis.h>
#include <NsGui/Transform.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Creates an arbitrary affine matrix transformation that is used to manipulate
/// objects or coordinate systems in a 2-D plane.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.matrixtransform.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API MatrixTransform: public Transform
{
public:
    MatrixTransform();
    MatrixTransform(const Transform2f& matrix);
    ~MatrixTransform();

    /// Parses a string value to create an instance of a MatrixTransform
    static bool TryParse(const char* str, Ptr<MatrixTransform>& result);

    /// Gets or sets matrix
    //@{
    const Transform2f& GetMatrix() const;
    void SetMatrix(const Transform2f& matrix);
    //@}

    /// From Transform
    //@{
    Transform2f GetTransform() const;
    //@}

    /// From Freezable
    //@{
    Ptr<MatrixTransform> Clone() const;
    Ptr<MatrixTransform> CloneCurrentValue() const;
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
    static const DependencyProperty* MatrixProperty;
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
        UpdateFlags_Matrix
    };

    NS_DECLARE_REFLECTION(MatrixTransform, Transform)
};

}

#endif
