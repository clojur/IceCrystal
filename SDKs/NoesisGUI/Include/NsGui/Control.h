////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_CONTROL_H__
#define __GUI_CONTROL_H__


#include <NsCore/Noesis.h>
#include <NsGui/FrameworkElement.h>
#include <NsGui/ControlTemplate.h>
#include <NsGui/Enums.h>


namespace Noesis
{

class Brush;
class FontFamily;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents the base class for all user interactive elements.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.control.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API Control: public FrameworkElement
{
public:
    /// Constructor
    Control();

    /// Destructor
    ~Control();

    /// Gets or sets a brush that describes the background of a control
    //@{
    Brush* GetBackground() const;
    void SetBackground(Brush* brush);
    //@}

    /// Gets or sets a brush that describes the border of a control
    //@{
    Brush* GetBorderBrush() const;
    void SetBorderBrush(Brush* brush);
    //@}

    /// Gets or sets the border thickness of a control
    //@{
    const Thickness& GetBorderThickness() const;
    void SetBorderThickness(const Thickness& thickness);
    //@}

    /// Gets or sets the font family of the control
    //@{
    FontFamily* GetFontFamily() const;
    void SetFontFamily(FontFamily* fontFamily);
    //@}

    /// Gets or sets the font size
    //@{
    float GetFontSize() const;
    void SetFontSize(float size);
    //@}

    /// Gets or sets the font stretch
    //@{
    FontStretch GetFontStretch() const;
    void SetFontStretch(FontStretch stretch);
    //@}

    /// Gets or sets the font style
    //@{
    FontStyle GetFontStyle() const;
    void SetFontStyle(FontStyle style);
    //@}

    /// Gets or sets the weight or thickness of the specified font
    //@{
    FontWeight GetFontWeight() const;
    void SetFontWeight(FontWeight weight);
    //@}

    /// Gets or sets a brush that describes the foreground color
    //@{
    Brush* GetForeground() const;
    void SetForeground(Brush* foreground);
    //@}

    /// Gets or sets the horizontal alignment of a control's content
    //@{
    HorizontalAlignment GetHorizontalContentAlignment() const;
    void SetHorizontalContentAlignment(HorizontalAlignment halign);
    //@}

    /// Gets or sets a value that indicates whether a control is included in tab navigation.
    //@{
    bool GetIsTabStop() const;
    void SetIsTabStop(bool value);
    //@}
    
    /// Gets or sets the padding inside a control
    //@{
    const Thickness& GetPadding() const;
    void SetPadding(const Thickness& padding);
    //@}

    /// Gets or sets a value that determines the order in which elements receive focus when the 
    /// user navigates through controls by using the TAB key.
    //@{
    int32_t GetTabIndex() const;
    void SetTabIndex(int32_t value);
    //@}

    /// Gets or sets control template
    //@{
    ControlTemplate* GetTemplate() const;
    void SetTemplate(ControlTemplate* controlTemplate);
    //@}

    /// Gets or sets the vertical alignment of a control's content
    //@{
    VerticalAlignment GetVerticalContentAlignment() const;
    void SetVerticalContentAlignment(VerticalAlignment valign);
    //@}

    /// Get or sets a value that indicates whether focus is constrained within the control
    /// boundaries (for game pad/remote interaction)
    //@{
    bool GetIsFocusEngaged() const;
    void SetIsFocusEngaged(bool engage);
    //@}

    /// Get or sets a value that indicates whether focus can be constrained within the control
    /// boundaries (for game pad / remote interaction)
    //@{
    bool GetIsFocusEngagementEnabled() const;
    void SetIsFocusEngagementEnabled(bool enable);
    //@}

    /// Occurs when a mouse button is clicked two or more times
    UIElement::RoutedEvent_<MouseButtonEventHandler> MouseDoubleClick();

    /// Occurs when a user clicks the mouse button two or more times
    UIElement::RoutedEvent_<MouseButtonEventHandler> PreviewMouseDoubleClick();

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* BackgroundProperty;
    static const DependencyProperty* BorderBrushProperty;
    static const DependencyProperty* BorderThicknessProperty;
    static const DependencyProperty* FontFamilyProperty;
    static const DependencyProperty* FontSizeProperty;
    static const DependencyProperty* FontStretchProperty;
    static const DependencyProperty* FontStyleProperty;
    static const DependencyProperty* FontWeightProperty;
    static const DependencyProperty* ForegroundProperty;
    static const DependencyProperty* HorizontalContentAlignmentProperty;
    static const DependencyProperty* IsTabStopProperty;
    static const DependencyProperty* PaddingProperty;
    static const DependencyProperty* TabIndexProperty;
    static const DependencyProperty* TemplateProperty;
    static const DependencyProperty* VerticalContentAlignmentProperty;

    static const DependencyProperty* IsFocusEngagedProperty;
    static const DependencyProperty* IsFocusEngagementEnabledProperty;
    //@}

    /// Dependency events
    //@{
    static const RoutedEvent* MouseDoubleClickEvent;
    static const RoutedEvent* PreviewMouseDoubleClickEvent;
    //@}

protected:
    /// Property change notification for inheritors
    //@{
    virtual void OnFontFamilyChanged(FontFamily* fontFamily);
    virtual void OnFontSizeChanged(float fontSize);
    virtual void OnFontStretchChanged(FontStretch fontStretch);
    virtual void OnFontStyleChanged(FontStyle fontStyle);
    virtual void OnFontWeightChanged(FontWeight fontWeight);
    virtual void OnForegroundChanged(Brush* brush);
    virtual void OnIsFocusEngagedChanged(bool engaged);
    //@}

    /// Function to be called when a property involved in the internal VisualStates of the control
    /// has changed
    virtual void UpdateVisualStates();

    /// Calls VisualStateManager GoToState()
    //@{
    void GoToState(NsSymbol stateName);
    void GoToState(NsSymbol stateName, NsSymbol fallbackStateName);
    //@}

    /// Raises the MouseDoubleClick routed event.
    virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
    
    /// Raises the PreviewMouseDoubleClick routed event.
    virtual void OnPreviewMouseDoubleClick(const MouseButtonEventArgs& e);

    /// From UIElement
    //@{
    void OnPreviewKeyDown(const KeyEventArgs& e) override;
    void OnPreviewLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    //@}

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

private:

    /// Class event handlers
    //@{
    static void StaticOnPreviewMouseDoubleClick(BaseComponent* obj, const EventArgs& e);
    static void StaticOnMouseDoubleClick(BaseComponent* obj, const EventArgs& e);
    //@}

    NS_DECLARE_REFLECTION(Control, FrameworkElement)
};

}

#endif
