////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BULLETDECORATOR_H__
#define __GUI_BULLETDECORATOR_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Decorator.h>


namespace Noesis
{

class Brush;
class RectangleGeometry;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a layout control that aligns a bullet and another visual object.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.bulletdecorator.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BulletDecorator: public Decorator
{
public:
    /// Constructor
    BulletDecorator();

    /// Destructor
    ~BulletDecorator();

    /// Gets or sets the background color for a BulletDecorator control.
    //@{
    Brush* GetBackground() const;
    void SetBackground(Brush* brush);
    //@}
    
    /// Gets or sets the object to use as the bullet in a BulletDecorator.
    //@{
    UIElement* GetBullet() const;
    void SetBullet(UIElement* bullet);
    //@}

protected:
    /// From DependencyObject
    //@{
    void OnInit();
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const;
    Visual* GetVisualChild(uint32_t index) const;
    //@}

    /// From UIElement
    //@{
    void OnRender(DrawingContext* context);
    //@}

    /// From FrameworkElement
    //@{
    void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const;
    uint32_t GetLogicalChildrenCount() const;
    BaseComponent* GetLogicalChild(uint32_t index) const;
    Size MeasureOverride(const Size& availableSize);
    Size ArrangeOverride(const Size& finalSize);
    //@}

private:
    uint32_t InternalGetNumChildren() const;
    Visual* InternalGetChild(uint32_t index) const;

public:
    static const DependencyProperty* BackgroundProperty;

private:
    Ptr<UIElement> mBullet;
    Ptr<RectangleGeometry> mBackgroundGeometry;

    NS_DECLARE_REFLECTION(BulletDecorator, Decorator)
};

NS_WARNING_POP

}

#endif
