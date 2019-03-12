////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BASETEXTBOX_H__
#define __GUI_BASETEXTBOX_H__


#include <NsCore/Noesis.h>
#include <NsGui/Control.h>
#include <NsGui/Enums.h>


namespace Noesis
{

class Brush;
class ScrollViewer;
struct CanExecuteRoutedEventArgs;
struct ExecutedRoutedEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// An abstract base class that provides functionality for text editing controls, including TextBox.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.primitives.textboxbase.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BaseTextBox: public Control
{
public:
    /// Constructor
    BaseTextBox();
    
    /// Destructor
    virtual ~BaseTextBox() = 0;
    
    /// Gets or sets a value that indicates how the text editing control responds when the user 
    /// presses the ENTER key.
    //@{
    bool GetAcceptsReturn() const;
    void SetAcceptsReturn(bool value);
    //@}
    
    /// Gets or sets a value that indicates how the text editing control responds when the user 
    /// presses the TAB key.
    //@{
    bool GetAcceptsTab() const;
    void SetAcceptsTab(bool value);
    //@}
    
    /// Gets or sets the brush that is used to paint the caret of the text box (WPF 4.0)
    //@{
    Brush* GetCaretBrush() const;
    void SetCaretBrush(Brush* brush);
    //@}
    
    /// Gets or sets a value that indicates whether a horizontal scroll bar is shown.
    //@{
    ScrollBarVisibility GetHorizontalScrollBarVisibility() const;
    void SetHorizontalScrollBarVisibility(ScrollBarVisibility value);
    //@}
    
    /// Gets or sets a value that indicates whether the text editing control is read-only to a user 
    /// interacting with the control.
    //@{
    bool GetIsReadOnly() const;
    void SetIsReadOnly(bool value);
    //@}

    /// Gets a value that indicates whether the text box has focus and selected text
    bool GetIsSelectionActive() const;

    /// Gets or sets the brush that highlights selected text
    //@{
    Brush* GetSelectionBrush() const;
    void SetSelectionBrush(Brush* selectionBrush);
    //@}

    /// Gets or sets the opacity of the SelectionBrush
    /// \remarks If SelectionOpacity is set to 0, the SelectionBrush is transparent and is not
    /// visible. If SelectionOpacity is set to 1.0 or greater, the SelectionBrush is opaque and the
    /// selected text is not visible
    //@{
    float GetSelectionOpacity() const;
    void SetSelectionOpacity(float selectionOpacity);
    //@}

    /// Gets or sets a value that indicates whether a vertical scroll bar is shown
    //@{
    ScrollBarVisibility GetVerticalScrollBarVisibility() const;
    void SetVerticalScrollBarVisibility(ScrollBarVisibility value);
    //@}

    /// Occurs when the text selection has changed
    UIElement::RoutedEvent_<RoutedEventHandler> SelectionChanged();

    /// Occurs when content changes in the text element
    UIElement::RoutedEvent_<RoutedEventHandler> TextChanged();

public:
    /// Dependency Properties
    //@{
    static const DependencyProperty* AcceptsReturnProperty;
    static const DependencyProperty* AcceptsTabProperty;
    static const DependencyProperty* CaretBrushProperty;
    static const DependencyProperty* HorizontalScrollBarVisibilityProperty;
    static const DependencyProperty* IsReadOnlyProperty;
    static const DependencyProperty* IsSelectionActiveProperty;
    static const DependencyProperty* SelectionBrushProperty;
    static const DependencyProperty* SelectionOpacityProperty;
    static const DependencyProperty* VerticalScrollBarVisibilityProperty;
    //@}

    /// Routed Events
    //@{
    static const RoutedEvent* SelectionChangedEvent;
    static const RoutedEvent* TextChangedEvent;
    //@}

protected:
    /// Get starting character position of the next or previous text block. Text blocks are groups
    /// of characters where all of them are alphanum or all are not alphanum. The space characters 
    /// on the right side of a block (alphanum or not) are considered parts of the block.
    /// These functions are useful when double-clicking on a textbox to select a block, or when 
    /// moving cursor between blocks using CTRL+LEFT or CTRL+RIGHT combinations.
    /// \param str String to look for grups in
    /// \param current Current position of cursor
    /// \param stayAtStart If current position is at start of a block, it doesn't move
    //@{
    static uint32_t GetNextBlock(const char* str, uint32_t numChars, uint32_t current, 
        bool stayAtStart);
    static uint32_t GetPrevBlock(const char* str, uint32_t numChars, uint32_t current, 
        bool stayAtStart);
    //@}

    void SetIsSelectionActive(bool isActive);

    void RaiseSelectionChanged();
    virtual void OnSelectionChanged(const RoutedEventArgs& e);

    void RaiseTextChanged();
    virtual void OnTextChanged(const RoutedEventArgs& e);

    /// Notifies inheritors of BaseTextBox property changes
    //@{
    virtual void OnCaretBrushChanged(Brush* oldBrush, Brush* newBrush);
    virtual void OnSelectionBrushChanged(Brush* oldBrush, Brush* newBrush);
    virtual void OnSelectionOpacityChanged(float oldOpacity, float newOpacity);
    //@}

    /// Gets the ScrollViewer that manages text scroll
    virtual ScrollViewer* GetScrollViewer() const;

    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args);
    //@}

    /// From Control
    //@{
    void UpdateVisualStates();
    //@}

    NS_DECLARE_REFLECTION(BaseTextBox, Control)
};

}


#endif
