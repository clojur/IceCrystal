////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_IVIEW_H__
#define __GUI_IVIEW_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/InputEnums.h>
#include <NsCore/Interface.h>
#include <NsCore/ReflectionDeclareEnum.h>


namespace Noesis
{

class FrameworkElement;
class ResourceDictionary;
template<class T> class Delegate;
NS_INTERFACE IRenderer;
NS_INTERFACE IView;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum TessellationQuality
{
    /// The lowest tessellation quality for curves
    TessellationQuality_Low,

    /// Medium tessellation quality for curves
    TessellationQuality_Medium,

    /// The highest tessellation quality for curves
    TessellationQuality_High
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum RenderFlags
{
    /// Toggles wireframe mode when rendering triangles
    RenderFlags_Wireframe = 1,
    
    /// Each batch submitted to the GPU is given a unique solid color
    RenderFlags_ColorBatches = 2,

    /// Display pixel overdraw using blending layers. Different colors are used for each type
    /// of triangle. Green for normal, Red for opacities and Blue for clipping masks
    RenderFlags_Overdraw = 4,

    /// Inverts the render vertically
    RenderFlags_FlipY = 8,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct ViewStats
{
    float frameTime;
    float updateTime;
    float renderTime;

    uint32_t batches;

    uint32_t paths;
    uint32_t images;
    uint32_t texts;
    uint32_t glyphs;
    uint32_t masks;

    uint32_t triangles;
    uint32_t maskTriangles;
    uint32_t solidTriangles;
    uint32_t linearTriangles;
    uint32_t radialTriangles;
    uint32_t patternTriangles;

    uint32_t fills;
    uint32_t strokes;

    uint32_t rasterizedGlyphs;
    uint32_t tessellatedGlyphs;
    uint32_t discardedGlyphTiles;

    uint32_t uploadedRamps;

    uint32_t renderTargetSwitches;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Views manage UI trees
////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INTERFACE IView: public Interface
{
    /// Returns the renderer, to be used in the render thread
    virtual IRenderer* GetRenderer() = 0;

    /// Returns the root element
    virtual FrameworkElement* GetContent() const = 0;

    /// Sets the size of the surface where UI elements will layout and render
    virtual void SetSize(uint32_t width, uint32_t height) = 0;

    /// Per-Primitive Antialiasing extrudes the contours of the geometry and smooths them.
    /// Useful when GPU MSAA is not enabled
    virtual void SetIsPPAAEnabled(bool enabled) = 0;

    /// Sets tessellation quality. Medium is the default
    virtual void SetTessellationQuality(TessellationQuality quality) = 0;

    /// Enables debugging flags. No debug flags are active by default
    virtual void SetFlags(uint32_t flags) = 0;

    /// Activates the view and recovers keyboard focus
    virtual void Activate() = 0;

    /// Deactivates the view and removes keyboard focus
    virtual void Deactivate() = 0;

    /// Notifies that a mouse button was pressed. Origin is in the upper-left corner
    virtual void MouseButtonDown(int x, int y, MouseButton button) = 0;

    /// Notifies that a mouse button was released. Origin is in the upper-left corner
    virtual void MouseButtonUp(int x, int y, MouseButton button) = 0;

    /// Notifies that a mouse button was double clicked. Origin is in the upper-left corner
    virtual void MouseDoubleClick(int x, int y, MouseButton button) = 0;

    /// Notifies that mouse was moved. Origin is in the upper-left corner
    virtual void MouseMove(int x, int y) = 0;

    /// Notifies that mouse vertical wheel was rotated. Origin is in the upper-left corner
    /// The parameter wheelRotation indicates the distance the wheel is rotated, expressed in
    /// multiples or divisions of 120 (is the default value for 3 lines of scrolling). A positive
    /// value indicates that the wheel was rotated forward, away from the user; a negative value
    /// indicates that the wheel was rotated backward, toward the user
    virtual void MouseWheel(int x, int y, int wheelRotation) = 0;

    /// Notifies that mouse horizontal wheel was rotated. Origin is in the upper-left corner
    /// The parameter wheelRotation indicates the distance the wheel is rotated, expressed in
    /// multiples or divisions of 120 (is the default value for 3 lines of scrolling). A positive
    /// value indicates that the wheel was rotated to the right; a negative value indicates that
    /// the wheel was rotated to the left
    virtual void MouseHWheel(int x, int y, int wheelRotation) = 0;

    /// Notifies that a vertical scroll is being actioned. This is normally mapped to a gamepad axis.
    /// Value range from -1 (fully pressed down) to +1 (fully pressed up)
    virtual void Scroll(float value) = 0;

    /// Notifies that a horizontal scroll is being actioned. This is normally mapped to a gamepad axis.
    /// Value range from -1 (fully pressed left) to +1 (fully pressed right)
    virtual void HScroll(float value) = 0;

    /// Notifies that a finger touched the screen. Origin is in the upper-left corner
    virtual void TouchDown(int x, int y, uint64_t id) = 0;

    /// Notifies that a finger moved on the screen. Origin is in the upper-left corner
    virtual void TouchMove(int x, int y, uint64_t id) = 0;

    /// Notifies that a finger raised off of the screen. Origin is in the upper-left corner
    virtual void TouchUp(int x, int y, uint64_t id) = 0;

    /// Notifies that a key was pressed
    virtual void KeyDown(Key key) = 0;

    /// Notifies that a key was released
    virtual void KeyUp(Key key) = 0;

    /// Notifies that a key was translated to the corresponding character
    virtual void Char(uint32_t ch) = 0;

    /// Performs a layout pass and sends updates to the render tree
    virtual void Update(double timeInSeconds) = 0;

    /// Rendering event occurs after animation and layout have been applied to the composition
    /// tree, just before objects in the composition tree are rendered
    typedef Delegate<void(IView* view)> RenderingEventHandler;
    virtual RenderingEventHandler& Rendering() = 0;

    /// Gets stats counters
    virtual ViewStats GetStats() const = 0;

    NS_IMPLEMENT_INLINE_REFLECTION_(IView, Interface)
};

}

#endif
