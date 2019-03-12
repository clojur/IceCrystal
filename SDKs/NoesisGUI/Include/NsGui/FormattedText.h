////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FORMATTEDTEXT_H__
#define __GUI_FORMATTEDTEXT_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/Freezable.h>
#include <NsGui/IRenderProxyCreator.h>
#include <NsGui/Enums.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsCore/String.h>
#include <NsCore/Vector.h>
#include <NsCore/Set.h>
#include <NsCore/NSTLPoolAllocator.h>
#include <NsDrawing/Rect.h>


namespace Noesis
{

class Inline;
class Brush;
class FontFamily;
class Font;
struct TextRun;
struct IVGLFontFace;
struct IVGLTextLayout;
template<class T> class TypedCollection;
typedef Noesis::TypedCollection<Noesis::Inline> InlineCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// FormattedText. Draws an ellipse.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FormattedText: public BaseComponent, public IRenderProxyCreator
{
public:
    /// Constructor
    FormattedText();

    /// Destructor
    ~FormattedText();

    void BuildTextRuns(const char* text, InlineCollection* inlines, FontFamily* fontFamily,
        FontWeight fontWeight, FontStretch fontStretch, FontStyle fontStyle, float fontSize,
        float strokeThickness, Brush* background, Brush* foreground, Brush* stroke,
        TextDecorations textDecorations);

    Size Measure(TextAlignment alignment, TextWrapping wrapping, TextTrimming trimming,
        float maxWidth, float maxHeight);

    void Layout(TextAlignment alignment, TextWrapping wrapping, TextTrimming trimming,
        float maxWidth, float maxHeight);
    void ResetLayout();

    Rect GetBounds() const;
    void GetGlyphPosition(uint32_t chIndex, bool afterChar, float& x, float& y) const;
    uint32_t HitTest(float x, float y) const;

    bool IsEmpty() const;

    /// From IRenderProxyCreator
    //@{
    void CreateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UpdateRenderProxy(RenderTreeUpdater& updater, uint32_t proxyIndex) override;
    void UnregisterRenderer(ViewId viewId) override;
    //@}

    NS_IMPLEMENT_INTERFACE_FIXUP

private:
    friend class TextBlock;
    friend class TextSelector;
    friend class TextContainer;

    void AddRuns(InlineCollection* inlines, FontFamily* fontFamily, FontWeight fontWeight,
        FontStretch fontStretch, FontStyle fontStyle, IVGLFontFace* fontFace,
        float fontSize, float strokeThickness, int32_t backgroundIndex,
        int32_t foregroundIndex, int32_t strokeIndex, TextDecorations textDecorations);
    void AddRun(const char* text, IVGLFontFace* fontFace, float fontSize,
        float strokeThickness, int32_t foregroundIndex, int32_t strokeIndex,
        int32_t backgroundIndex, TextDecorations textDecorations);
    IVGLFontFace* AddFontFace(FontFamily* family, FontWeight weight, FontStretch stretch,
        FontStyle style);
    IVGLFontFace* InlineFace(Inline* inl, IVGLFontFace* currentFace,
        FontFamily*& family, FontWeight& weight, FontStretch& stretch, FontStyle& style);
    TextDecorations InlineDecorations(TextDecorations currentDecorations, Inline* inl);
    float InlineSize(float currentSize, Inline* inl, const DependencyProperty* dp);
    int32_t InlineBrushIndex(int32_t currentBrushIndex, Inline* inl, const DependencyProperty* dp);
    int32_t BrushIndex(Brush* brush);

    void UnregisterBrushes();

    void RegisterBrush(Brush* brush);
    void UnregisterBrush(Brush* brush);
    void OnBrushChanged(Freezable* sender, FreezableEventReason reason);

private:
    RenderProxyCreatorFlags mUpdateFlags;

    enum UpdateFlags
    {
        UpdateFlags_Brushes,
        UpdateFlags_TextLayout
    };

    NsVector<TextRun> mTextRuns;
    NsVector<Ptr<IVGLFontFace> > mFontFaces;
    NsVector<Ptr<Brush> > mBrushes;

    typedef NsSet<Brush*, eastl::less<Brush*>, eastl::PoolAllocator> RegisteredBrushes;
    RegisteredBrushes mRegisteredBrushes;

    Ptr<IVGLTextLayout> mTextLayout;

    NS_DECLARE_REFLECTION(FormattedText, BaseComponent)
};

NS_WARNING_POP

}

#endif
