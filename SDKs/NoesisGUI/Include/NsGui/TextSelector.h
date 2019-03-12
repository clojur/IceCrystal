////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_TEXTSELECTOR_H__
#define __GUI_TEXTSELECTOR_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsGui/ContentControl.h>
#include <NsGui/CoreApi.h>
#include <NsDrawing/Point.h>


namespace Noesis
{

struct CanExecuteRoutedEventArgs;
struct ExecutedRoutedEventArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides copy and paste functionality for all the text elements contained in its visual tree
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API TextSelector: public ContentControl
{
public:
    TextSelector();
    ~TextSelector();

    Brush* GetSelectionBrush() const;
    void SetSelectionBrush(Brush* brush);

public:
    static const DependencyProperty* SelectionBrushProperty;

protected:
    // From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    //@}

    // From UIElement
    //@{
    void OnMouseLeftButtonDown(const MouseButtonEventArgs& e) override;
    void OnMouseMove(const MouseEventArgs& e) override;
    void OnMouseLeftButtonUp(const MouseButtonEventArgs& e) override;
    Size MeasureOverride(const Size& availableSize) override;
    Size ArrangeOverride(const Size& finalSize) override;
    //@}

private:
    static void StaticOnSelectionBrushChanged(DependencyObject* d,
        const DependencyPropertyChangedEventArgs& e);
    static void StaticOnCanExecuteCopy(BaseComponent* target,
        const CanExecuteRoutedEventArgs& args);
    static void StaticOnExecutedCopy(BaseComponent* target,
        const ExecutedRoutedEventArgs& args);

private:
    Point mStartPoint;
    Point mEndPoint;
    bool mSelectionStarted;

    NsString mSelectedText;

    class TextSelection;
    Ptr<TextSelection> mTextSelection;

    NS_DECLARE_REFLECTION(TextSelector, ContentControl)
};

NS_WARNING_POP

}


#endif
