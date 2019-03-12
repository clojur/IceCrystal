////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_MATRIX3DPROJECTION_H__
#define __GUI_MATRIX3DPROJECTION_H__


#include <NsCore/Noesis.h>
#include <NsGui/Projection.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Applies a 3D Matrix to an object.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.media.matrix3dprojection.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Matrix3DProjection: public Projection
{
public:
    Matrix3DProjection();
    Matrix3DProjection(const Matrix4f& matrix);
    ~Matrix3DProjection();

    /// Gets or sets the Matrix3D  that is used for the projection that is applied to the object
    //@{
    const Matrix4f& GetProjectionMatrix() const;
    void SetProjectionMatrix(const Matrix4f& matrix);
    //@}

    /// From Freezable
    //@{
    Ptr<Matrix3DProjection> Clone() const;
    Ptr<Matrix3DProjection> CloneCurrentValue() const;
    //@}

    /// From Projection
    //@{
    bool IsIdentity() const;
    Matrix4f GetProjection(const Size& surface, const Size& size) const;
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
    static const DependencyProperty* ProjectionMatrixProperty;
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
        UpdateFlags_ProjectionMatrix
    };

    NS_DECLARE_REFLECTION(Matrix3DProjection, Projection)
};

}

#endif
