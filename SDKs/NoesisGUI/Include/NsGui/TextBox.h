////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTBOX_H__
#define __GUI_TEXTBOX_H__


#include <NsCore/Noesis.h>
#include <NsGui/BaseTextBox.h>
#include <NsGui/CompositionUnderline.h>
#include <NsCore/Vector.h>


namespace Noesis
{

NS_INTERFACE IScrollInfo;
class ScrollViewer;
class TextBoxTextContainer;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Represents a control that can be used to display or edit unformatted text.
/// (Mixed WPF definitions of TextBoxBase and TextBox).
///
/// http://msdn.microsoft.com/en-us/library/system.windows.controls.textbox.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TextBox: public BaseTextBox
{
public:
    TextBox();
    ~TextBox();

    /// Gets or sets the insertion position index of the caret
    //@{
    int32_t GetCaretIndex() const;
    void SetCaretIndex(int32_t caretIndex);
    //@}

    /// Gets or sets the maximum number of characters that can be manually entered into the text box
    //@{
    int32_t GetMaxLength() const;
    void SetMaxLength(int32_t length);
    //@}

    /// Gets or sets the maximum number of visible lines
    //@{
    int32_t GetMaxLines() const;
    void SetMaxLines(int32_t lines);
    //@}

    /// Gets or sets the minimum number of visible lines
    //@{
    int32_t GetMinLines() const;
    void SetMinLines(int32_t lines);
    //@}

    /// Gets or sets the content of the current selection in the text box
    /// \remarks Returned string must be copied becuase it will be invalidated next time you
    /// interact with the text box
    /// \prop
    //@{
    const char* GetSelectedText() const;
    void SetSelectedText(const char* text);
    //@}

    /// Gets or sets a value indicating the number of characters in the current selection in the
    /// text box
    /// \prop
    //@{
    int32_t GetSelectionLength() const;
    void SetSelectionLength(int32_t len);
    //@}

    /// Gets or sets a character index for the beginning of the current selection
    /// \prop
    //@{
    int32_t GetSelectionStart() const;
    void SetSelectionStart(int32_t pos);
    //@}

    /// Selects all text
    void SelectAll();

    /// Gets or sets the horizontal alignment of the contents of the text box
    //@{
    TextAlignment GetTextAlignment() const;
    void SetTextAlignment(TextAlignment textAlign);
    //@}

    /// Gets or sets the text contents of the text box
    //@{
    const char* GetText() const;
    void SetText(const char* text);
    //@}

    /// Gets or sets how the text box should wrap text
    //@{
    TextWrapping GetTextWrapping() const;
    void SetTextWrapping(TextWrapping textWrap);
    //@}

    // Gets the container element of the drawn text
    UIElement* GetContentHost() const;

    // Calculates the bounding box of a range of text
    Rect GetRangeBounds(uint32_t start, uint32_t end) const;

    // IME composition underlines management
    //@{
    uint32_t GetNumCompositionUnderlines() const;
    const CompositionUnderline& GetCompositionUnderline(uint32_t index) const;
    void AddCompositionUnderline(const CompositionUnderline& compositionUnderline);
    void ClearCompositionUnderlines();
    //@}

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* MaxLengthProperty;
    static const DependencyProperty* MaxLinesProperty;
    static const DependencyProperty* MinLinesProperty;
    static const DependencyProperty* TextAlignmentProperty;
    static const DependencyProperty* TextProperty;
    static const DependencyProperty* TextWrappingProperty;
    //@}
    
protected:
    /// From DependencyObject
    //@{
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    //@}

    /// From FrameworkElement
    //@{
    uint32_t GetLogicalChildrenCount() const override;
    BaseComponent* GetLogicalChild(uint32_t index) const override;
    Size MeasureOverride(const Size& availableSize) override;
    //@}
    
    /// From Control
    //@{
    void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot) override;
    void OnFontFamilyChanged(FontFamily* fontFamily) override;
    void OnFontSizeChanged(float fontSize) override;
    void OnFontStretchChanged(FontStretch fontStretch) override;
    void OnFontStyleChanged(FontStyle fontStyle) override;
    void OnFontWeightChanged(FontWeight fontWeight) override;
    void OnForegroundChanged(Brush* brush) override;
    void OnMouseDoubleClick(const MouseButtonEventArgs& e) override;
    void OnIsFocusEngagedChanged(bool engaged) override;
    //@}

    /// From BaseTextBox
    //@{
    void OnCaretBrushChanged(Brush* oldBrush, Brush* newBrush) override;
    void OnSelectionBrushChanged(Brush* oldBrush, Brush* newBrush) override;
    void OnSelectionOpacityChanged(float oldOpacity, float newOpacity) override;
    ScrollViewer* GetScrollViewer() const override;
    //@}
    
    /// From UIElement
    //@{
    void OnKeyDown(const KeyEventArgs& e) override;
    void OnKeyUp(const KeyEventArgs& e) override;
    void OnTextInput(const TextCompositionEventArgs& e) override;
    void OnGotKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnLostKeyboardFocus(const KeyboardFocusChangedEventArgs& e) override;
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    void OnMouseMove(const MouseEventArgs& e) override;
    //@}

private:
    friend class ComboBox;
    void ArrangeTextContainer(float arrangeWidth);

    void CopyToClipboard(const NsString& text, uint32_t begin, uint32_t end);

    void UpdateContentHostHeight();
    void UpdateContentHostHeight(float padding);

    void OnTextContainerSelectionChanged();

    void EnableFocus();
    void DisableFocus();

    /// Command management
    //@{
    static void StaticOnCanExecuteCopy(BaseComponent* target,
        const CanExecuteRoutedEventArgs& args);
    static void StaticOnExecutedCopy(BaseComponent* target,
        const ExecutedRoutedEventArgs& args);
    static void StaticOnCanExecuteCut(BaseComponent* target,
        const CanExecuteRoutedEventArgs& args);
    static void StaticOnExecutedCut(BaseComponent* target,
        const ExecutedRoutedEventArgs& args);
    static void StaticOnCanExecutePaste(BaseComponent* target,
        const CanExecuteRoutedEventArgs& args);
    static void StaticOnExecutedPaste(BaseComponent* target,
        const ExecutedRoutedEventArgs& args);
    static void StaticOnCanExecuteSelectAll(BaseComponent* target,
        const CanExecuteRoutedEventArgs& args);
    static void StaticOnExecutedSelectAll(BaseComponent* target,
        const ExecutedRoutedEventArgs& args);
    //@}

    void OnCanExecuteCopy(const CanExecuteRoutedEventArgs& args);
    void OnExecuteCopy(const ExecutedRoutedEventArgs& args);
    void OnCanExecuteCut(const CanExecuteRoutedEventArgs& args);
    void OnExecuteCut(const ExecutedRoutedEventArgs& args);
    void OnCanExecutePaste(const CanExecuteRoutedEventArgs& args);
    void OnExecutePaste(const ExecutedRoutedEventArgs& args);
    void OnCanExecuteSelectAll(const CanExecuteRoutedEventArgs& args);
    void OnExecuteSelectAll(const ExecutedRoutedEventArgs& args);

    static bool StaticCoerceHorizontalScrollBarVisibility(const DependencyObject* object,
        const void* value, void* coercedValue);

private:
    Ptr<FrameworkElement> mContentHost;
    Ptr<TextBoxTextContainer> mTextContainer;

    typedef NsVector<CompositionUnderline> CompositionUnderlines;
    CompositionUnderlines mCompositionUnderlines;

    union
    {
        mutable struct
        {
            bool mouseDown:1;
            bool updateContentHostMinMax:1;
        } mFlags;

        // To quickly set all flags to 0
        mutable uint8_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(TextBox, BaseTextBox)
};

NS_WARNING_POP

}

#endif
