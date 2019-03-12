////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_FRAMEWORKELEMENT_H__
#define __GUI_FRAMEWORKELEMENT_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclareEnum.h>
#include <NsGui/UIElement.h>
#include <NsGui/IUITreeNode.h>
#include <NsGui/INameScope.h>
#include <NsGui/Style.h>
#include <NsGui/Enums.h>


namespace Noesis
{

class ContextMenu;
class NameScope;
class FocusVisualLayer;
class ResourceDictionary;
class BaseBinding;
class BaseBindingExpression;
class BindingExpression;
class ResourceKeyType;
class FrameworkTemplate;
class DataTemplate;
class TemplateLocalValueProvider;
class TransformGroup;
class TranslateTransform;
class MatrixTransform;
class CombinedGeometry;
class RectangleGeometry;
class FrameworkElementTest;
class StoryboardTest;
template<class T> class Delegate;
struct ContextMenuEventArgs;
struct RequestBringIntoViewEventArgs;
struct SizeChangedEventArgs;
struct ToolTipEventArgs;
struct NotifyDictionaryChangedEventArgs;
struct Size;
struct Thickness;
NS_INTERFACE ITimeManager;
NS_INTERFACE IView;

////////////////////////////////////////////////////////////////////////////////////////////////////
enum AncestorNameScopeChangeAction
{
    AncestorNameScopeChangeAction_Attach,
    AncestorNameScopeChangeAction_Detach,
    AncestorNameScopeChangeAction_Change
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct AncestorNameScopeChangedArgs
{
    AncestorNameScopeChangeAction action;
    INameScope* nameScope;
    NameScopeChangedArgs changeArgs;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Noesis::Delegate<void (FrameworkElement*)> AncestorChangedDelegate;
typedef Noesis::Delegate<void(BaseComponent*, const NotifyDictionaryChangedEventArgs&)>
    AncestorResourcesChangedDelegate;
typedef Noesis::Delegate<void (FrameworkElement*, const AncestorNameScopeChangedArgs&)>
    AncestorNameScopeChangedDelegate;
typedef Noesis::Delegate<void (BaseComponent*, const ContextMenuEventArgs&)> 
    ContextMenuEventHandler;
typedef Noesis::Delegate<void (BaseComponent*, const RequestBringIntoViewEventArgs&)>
    RequestBringIntoViewEventHandler;
typedef Noesis::Delegate<void (BaseComponent*, const SizeChangedEventArgs&)> 
    SizeChangedEventHandler;
typedef Noesis::Delegate<void (BaseComponent*, const ToolTipEventArgs&)> 
    ToolTipEventHandler;

template<class T> class TypedCollection;
typedef Noesis::TypedCollection<Noesis::BaseTrigger> TriggerCollection;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

#ifdef FindResource
#undef FindResource
#endif

/// Helper macro to easily connect events to code behind functions when overriding ConnectEvent
#define NS_CONNECT_EVENT(type_, event_, handler_) \
    if (Noesis::String::Equals(event, #event_) && Noesis::String::Equals(handler, #handler_)) \
    { \
        ((type_*)source)->event_() += Noesis::MakeDelegate(this, &SelfClass::handler_); \
        return true; \
    }

/// Helper macro to easily connect attached events to code behind functions on ConnectEvent override
#define NS_CONNECT_ATTACHED_EVENT(type_, event_, handler_) \
    if (Noesis::String::Equals(event, #event_) && Noesis::String::Equals(handler, #handler_)) \
    { \
        ((Noesis::UIElement*)source)->AddHandler(type_::event_##Event, \
            Noesis::MakeDelegate(this, &SelfClass::handler_)); \
        return true; \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Provides GUI framework-level features for user interface elements.
///
/// http://msdn.microsoft.com/en-us/library/system.windows.frameworkelement.aspx
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FrameworkElement: public UIElement, public IUITreeNode
{
public:
    NS_DISABLE_COPY(FrameworkElement)

    FrameworkElement();
    ~FrameworkElement();

    // Property accessors
    //@{

    /// Gets the rendered height of this element.
    float GetActualHeight() const;

    /// Gets the rendered width of this element.
    float GetActualWidth() const;

    /// Gets or sets the context menu element that should appear whenever the context menu is 
    /// requested through user interface (UI) from within this element.
    //@{
    ContextMenu* GetContextMenu() const;
    void SetContextMenu(ContextMenu* menu);
    //@}

    /// Gets or sets the cursor that displays when the mouse pointer is over this element.
    //@{
    Cursor GetCursor() const;
    void SetCursor(Cursor cursor);
    //@}

    /// Gets or sets the data context for an element when it participates in data binding
    //@{
    BaseComponent* GetDataContext() const;
    void SetDataContext(BaseComponent* context);
    //@}

    /// Gets or sets the key to use to reference the style for this control, when theme styles are
    /// used or defined
    //@{
    ResourceKeyType* GetDefaultStyleKey() const;
    void SetDefaultStyleKey(ResourceKeyType* key);
    //@}

    /// Gets or sets a property that enables customization of appearance, effects, or other style 
    /// characteristics that will apply to this element when it captures keyboard focus
    //@{
    Style* GetFocusVisualStyle() const;
    void SetFocusVisualStyle(Style* style);
    //@}

    /// Gets or sets a value that indicates whether this FrameworkElement should force the UI to
    /// render the cursor as declared by this element's Cursor property.
    //@{
    bool GetForceCursor() const;
    void SetForceCursor(bool force);
    //@}

    /// Gets or sets the suggested height of the element.
    //@{
    float GetHeight() const;
    void SetHeight(float height);
    //@}

    /// Gets or sets the horizontal alignment characteristics applied to this element when it is 
    /// composed within a parent element, such as a panel or items control
    //@{
    HorizontalAlignment GetHorizontalAlignment() const;
    void SetHorizontalAlignment(HorizontalAlignment hAlign);
    //@}

    /// Gets or sets a graphics transformation that should apply to this element when layout is 
    /// performed
    //@{
    Transform* GetLayoutTransform() const;
    void SetLayoutTransform(Transform* transform);
    //@}

    /// Gets or sets the outer margin of an element.
    //@{
    const Thickness& GetMargin() const;
    void SetMargin(const Thickness& margin);
    //@}

    /// Gets or sets the maximum height constraint of the element.
    //@{
    float GetMaxHeight() const;
    void SetMaxHeight(float maxHeigth);
    //@}

    /// Gets or sets the maximum width constraint of the element. 
    //@{
    float GetMaxWidth() const;
    void SetMaxWidth(float maxWidth);
    //@}

    /// Gets or sets the minimum height constraint of the element.
    //@{
    float GetMinHeight() const;
    void SetMinHeight(float minHeight);
    //@}

    /// Gets or sets the minimum width constraint of the element.
    //@{
    float GetMinWidth() const;
    void SetMinWidth(float minWidth);
    //@}

    /// Gets or sets the identifying name of the element. The name provides a reference so that 
    /// code-behind, such as event handler code, can refer to a markup element after it is 
    /// constructed during processing by a XAML processor. 
    //@{
    const char* GetName() const;
    void SetName(const char* name);
    //@}

    /// Gets or sets a value that indicates whether this element incorporates style properties from 
    /// theme styles.
    //@{
    bool GetOverridesDefaultStyle() const;
    void SetOverridesDefaultStyle(bool value);
    //@}

    /// Gets or sets the style used by this element when it is rendered
    //@{
    Style* GetStyle() const;
    void SetStyle(Style* style);
    //@}

    /// Gets or sets an arbitrary object value that can be used to store custom information about 
    /// this element
    //@{
    BaseComponent* GetTag() const;
    void SetTag(BaseComponent* tag);
    void SetTag(const char* tag);
    //@}

    /// Gets or sets the tool-tip object that is displayed for this element in the user interface 
    //@{
    BaseComponent* GetToolTip() const;
    void SetToolTip(BaseComponent* tooltip);
    void SetToolTip(const char* tooltip);
    //@}

    /// Gets or sets a value that indicates whether layout rounding should be applied to this
    /// element's size and position during layout
    //@{
    bool GetUseLayoutRounding() const;
    void SetUseLayoutRounding(bool useLayoutRounding);
    //@}

    /// Gets or sets the vertical alignment characteristics applied to this element when it is 
    /// composed within a parent element such as a panel or items control.
    //@{
    VerticalAlignment GetVerticalAlignment() const;
    void SetVerticalAlignment(VerticalAlignment vAlign);
    //@}

    /// Gets or sets the width of the element
    //@{
    float GetWidth() const;
    void SetWidth(float width);
    //@}

    /// Get the trigger collection
    TriggerCollection* GetTriggers() const;

    //@}

    /// Returns a binding expression if the target property has an active binding; otherwise,
    /// returns null
    BindingExpression* GetBindingExpression(const DependencyProperty* dp);

    /// Attaches a binding to this element, based on the provided binding object
    /// \return The binding expression just set
    BaseBindingExpression* SetBinding(const DependencyProperty* dp, BaseBinding* binding);

    /// Attaches a binding to this element, based on the provided source property name as a path
    /// qualification to the data source
    /// \return The binding expression just set
    BindingExpression* SetBinding(const DependencyProperty* dp, const char* path);

    /// Gets a value that indicates whether this element has been loaded for presentation. 
    bool IsLoaded() const;

    /// Attempts to bring this element into view, within any scrollable regions it
    /// is contained within.
    void BringIntoView();

    /// Attempts to bring the provided region size of this element into view, within
    /// any scrollable regions it is contained within.
    void BringIntoView(const Rect& targetRectangle);

    /// Gets the TimeManager that controls current element
    ITimeManager* GetTimeManager() const;

    /// Gets the logical parent element of this element
    FrameworkElement* GetParent() const;

    /// Template management
    //@{

    /// Gets the logical parent or the templated parent when logical parent is not available
    FrameworkElement* GetParentOrTemplatedParent() const;

    /// Gets or sets the template parent of this element. This property is not relevant if the 
    /// element was not created through a template
    //@{
    FrameworkElement* GetTemplatedParent() const;
    void SetTemplatedParent(FrameworkElement* templatedParent,
        FrameworkTemplate* frameworkTemplate);
    //@}

    /// Looks for a named element in the applied template
    BaseComponent* GetTemplateChild(const char* name) const;
    template<class T> T* GetTemplateChild(const char* name) const;

    /// Gets the root element of the applied template
    FrameworkElement* GetTemplateRoot() const;

    /// Gets applied template on this element
    FrameworkTemplate* GetFrameworkTemplate() const;

    /// Applies a template on this element for the provided context
    void ApplyFrameworkTemplate(FrameworkTemplate* frameworkTemplate);

    /// Returns a clone of this element. Used when applying a template
    Ptr<FrameworkElement> Clone(FrameworkElement* parent, FrameworkElement* templatedParent,
        FrameworkTemplate* template_) const;

    //@}

    /// Gets root for VisualState management
    virtual FrameworkElement* GetStateGroupsRoot() const;

    /// Provides accessors that simplifies access to the NameScope registration methods
    //@{
    BaseComponent* FindName(const char* name) const;
    template<class T> T* FindName(const char* name) const;
    ObjectWithNameScope FindNameAndScope(const char* name) const;
    void RegisterName(const char* name, BaseComponent* object);
    void UnregisterName(const char* name);
    void UpdateName(const char* name, BaseComponent* object);
    INameScope::ChangedDelegate& NameScopeChanged();
    //@}

    /// Finds a resource looking in the logical parent chain
    //@{
    BaseComponent* FindResource(IResourceKey* key) const;
    template<class T> T* FindResource(IResourceKey* key) const;
    BaseComponent* TryFindResource(IResourceKey* key) const;
    template<class T> T* TryFindResource(IResourceKey* key) const;
    BaseComponent* FindResource(const char* key) const;
    template<class T> T* FindResource(const char* key) const;
    BaseComponent* TryFindResource(const char* key) const;
    template<class T> T* TryFindResource(const char* key) const;
    //@}

    /// Gets or sets the locally-defined resource dictionary
    //@{
    ResourceDictionary* GetResources() const;
    void SetResources(ResourceDictionary* resources);
    //@}

    /// Notifies when an ancestor has changed its parent
    AncestorChangedDelegate& AncestorChanged();
    /// Notifies when a change occurred in the resources dictionary of this element or an ancestor
    AncestorResourcesChangedDelegate& AncestorResourcesChanged();
    /// Notifies when a NameScope in an ancestor has changed any of its items
    AncestorNameScopeChangedDelegate& AncestorNameScopeChanged();
    /// Occurs just before any context menu on the element is closed
    UIElement::RoutedEvent_<ContextMenuEventHandler> ContextMenuClosing();
    /// Occurs when any context menu on the element is opened
    UIElement::RoutedEvent_<ContextMenuEventHandler> ContextMenuOpening();
    /// Occurs when the data context for this element changes
    UIElement::Event_<DependencyPropertyChangedEventHandler> DataContextChanged();
    /// Occurs when this element is initialized
    UIElement::Event_<EventHandler> Initialized();
    /// Occurs when the element is laid out, rendered, and ready for interaction
    UIElement::RoutedEvent_<RoutedEventHandler> Loaded();
    /// Occurs when BringIntoView is called on this element
    UIElement::RoutedEvent_<RequestBringIntoViewEventHandler> RequestBringIntoView();
    /// Occurs when either the ActualHeight or the ActualWidth properties change value on this 
    /// element
    UIElement::RoutedEvent_<SizeChangedEventHandler> SizeChanged();
    /// Occurs just before any tooltip on the element is closed
    UIElement::RoutedEvent_<ToolTipEventHandler> ToolTipClosing();
    /// Occurs when any tooltip on the element is opened
    UIElement::RoutedEvent_<ToolTipEventHandler> ToolTipOpening();
    /// Occurs when the element is removed from within an element tree of loaded elements
    UIElement::RoutedEvent_<RoutedEventHandler> Unloaded();

    /// Attaches specified event to code-behind content
    virtual bool ConnectEvent(BaseComponent* source, const char* event, const char* handler);

    /// From UIElement
    //@{
    bool MoveFocus(const TraversalRequest& request) override;
    DependencyObject* PredictFocus(FocusNavigationDirection direction) override;
    //@}

    /// From IUITreeNode
    //@{
    IUITreeNode* GetNodeParent() const override;
    void SetNodeParent(IUITreeNode* parent) override;
    BaseComponent* FindNodeResource(IResourceKey* key,
        bool fullElementSearch) const override;
    BaseComponent* FindNodeName(const char* name) const override;
    ObjectWithNameScope FindNodeNameAndScope(const char* name) const override;
    //@}}

    NS_IMPLEMENT_INTERFACE_FIXUP

public:
    /// Dependency properties
    //@{
    static const DependencyProperty* ActualHeightProperty;
    static const DependencyProperty* ActualWidthProperty;
    static const DependencyProperty* ContextMenuProperty;
    static const DependencyProperty* CursorProperty;
    static const DependencyProperty* DataContextProperty;
    static const DependencyProperty* DefaultStyleKeyProperty;
    static const DependencyProperty* FocusVisualStyleProperty;
    static const DependencyProperty* ForceCursorProperty;
    static const DependencyProperty* HeightProperty;
    static const DependencyProperty* HorizontalAlignmentProperty;
    static const DependencyProperty* LayoutTransformProperty;
    static const DependencyProperty* MarginProperty;
    static const DependencyProperty* MaxHeightProperty;
    static const DependencyProperty* MaxWidthProperty;
    static const DependencyProperty* MinHeightProperty;
    static const DependencyProperty* MinWidthProperty;
    static const DependencyProperty* NameProperty;
    static const DependencyProperty* OverridesDefaultStyleProperty;
    static const DependencyProperty* StyleProperty;
    static const DependencyProperty* TagProperty;
    static const DependencyProperty* ToolTipProperty;
    static const DependencyProperty* UseLayoutRoundingProperty;
    static const DependencyProperty* VerticalAlignmentProperty;
    static const DependencyProperty* WidthProperty;
    //@}

    /// Dependency events
    //@{
    static const RoutedEvent* LoadedEvent;
    static const RoutedEvent* RequestBringIntoViewEvent;
    static const RoutedEvent* SizeChangedEvent;
    static const RoutedEvent* UnloadedEvent;
    //@}

protected:
    /// Invoked whenever an unhandled ContextMenuClosing routed event reaches this class in its
    /// route. Implement this method to add class handling for this event
    virtual void OnContextMenuClosing(const ContextMenuEventArgs& e);

    /// Invoked whenever an unhandled ContextMenuOpening routed event reaches this class in its
    /// route. Implement this method to add class handling for this event
    virtual void OnContextMenuOpening(const ContextMenuEventArgs& e);

    /// Invoked when the context menu changes
    virtual void OnContextMenuChanged(ContextMenu* oldMenu, ContextMenu* newMenu);

    /// Invoked when the style in use on this element changes, which will invalidate
    /// the layout.
    virtual void OnStyleChanged(Style* oldStyle, Style* newStyle);

    /// Invoked when focus style changes
    virtual void OnFocusStyleChanged(Style* oldStyle, Style* newStyle);

    /// Invoked whenever an unhandled ToolTipClosing routed event reaches this class in its route. 
    /// Implement this method to add class handling for this event.
    virtual void OnToolTipClosing(const ToolTipEventArgs& e);

    /// Invoked whenever the ToolTipOpening routed event reaches this class in its route. Implement 
    /// this method to add class handling for this event.
    virtual void OnToolTipOpening(const ToolTipEventArgs& e);

    /// Performs custom clone work for a derived class
    virtual void CloneOverride(FrameworkElement* clone, FrameworkTemplate* template_) const;

    /// Adds the provided object to the logical tree of this element
    void AddLogicalChild(BaseComponent* child);

    /// Removes the provided object from this element's logical tree. FrameworkElement updates the
    /// affected logical tree parent pointers to keep in sync with this deletion.
    void RemoveLogicalChild(BaseComponent* child);

    /// Gets the number of child logical elements for this element
    /// \remarks Each element implementation will decide how to store logical children
    virtual uint32_t GetLogicalChildrenCount() const;

    /// Returns the specified element in the parent Collection
    /// \remarks Each element implementation will decide how to store logical children
    virtual BaseComponent* GetLogicalChild(uint32_t index) const;

    /// Indicates that logical parent has changed
    virtual void OnLogicalParentChanged(FrameworkElement* oldParent);

    /// Process inherited properties and updates local values if required
    //@{
    void UpdateInheritedProps();
    void EndUpdateInheritedProps();
    //@}

    /// Allows derived classes to use this visual as the default visual child
    //@{
    Visual* GetSingleVisualChild() const;
    void SetSingleVisualChild(Visual* child);
    //@}

    // Property value changed events
    //@{
    virtual void OnWidthChanged(float width);
    virtual void OnHeightChanged(float height);
    //@}

    /// Looks for a DataTemplate in the UI tree that matches the type specified
    static DataTemplate* TryFindTemplate(FrameworkElement* element, const TypeClass* type);

    /// Indicates if the property specified is set as a local value in the template
    bool IsTemplateLocalValue(const DependencyProperty* dp) const;

    /// Invoked when framework template changes
    virtual void OnTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkElement* oldRoot,
        FrameworkTemplate* newTemplate, FrameworkElement* newRoot);

    /// Invoked when templated parent changes
    virtual void OnTemplatedParentChanged(FrameworkElement* oldParent, FrameworkElement* newParent);

    /// When overridden in a derived class, measures the size in layout required for child elements
    /// and determines a size for the FrameworkElement-derived class
    /// \return The size that this element determines it needs during layout, based on its
    /// calculations of child element sizes
    ///
    /// \note Override MeasureOverride to implement custom layout sizing behavior for your element
    /// as it participates in the layout system. Your implementation should do the following:
    ///
    /// 1. Iterate your element's particular collection of children that are part of layout,
    ///    call Measure on each child element.
    /// 2. Immediately get DesiredSize on the child (this is set as a property after Measure
    ///    is called).
    /// 3. Compute the net desired size of the parent based upon the measurement of the
    /// child elements.
    ///
    /// The return value of MeasureOverride should be the element's own desired size, which
    /// then becomes the measure input for the parent element of the current element. This
    /// same process continues through the layout system until the root element is reached
    ///
    /// During this process, child elements might return a larger DesiredSize size than the
    /// initial availableSize to indicate that the child element wants more space. This might
    /// be handled in your own implementation by introducing a scrollable region, by resizing
    /// the parent control, by establishing some manner of stacked order, or any number of
    /// solutions for measuring or arranging content
    virtual Size MeasureOverride(const Size& availableSize);

    /// When overridden in a derived class, positions child elements and determines a size for
    /// a FrameworkElement derived class
    /// \param finalSize The final area within the parent that this element should use to arrange
    /// itself and its children
    /// \return The actual size used
    ///
    /// \note The implementation pattern should call Arrange on each visible child element, and
    /// pass the final desired size for each child element as the finalRect parameter. Parent
    /// elements should call Arrange on each child, otherwise the child elements will not be
    /// rendered
    virtual Size ArrangeOverride(const Size& finalSize);

    /// Calculates min and max limits depending on width, height and limit values
    void GetMinMax(float width, float height,
        float& minW, float& minH, float& maxW, float& maxH) const;

    /// From BaseRefCounted
    //@{
    int32_t OnDestroy() const override;
    //@}

    /// From DependencyObject
    //@{
    void OnInit() override;
    void OnPostInit() override;
    bool OnPropertyChanged(const DependencyPropertyChangedEventArgs& args) override;
    bool OnSubPropertyChanged(const DependencyProperty* prop) override;
    void OnObjectValueSet(BaseComponent* oldValue, BaseComponent* newValue) override;
    ProviderValue GetProviderValue(const DependencyProperty* dp) const override;
    ProviderValue GetNonCachedProviderValue(const DependencyProperty* dp) const override;
    uint8_t GetNonCachedValueProvider(const DependencyProperty* dp) const override;
    //@}

    /// From Visual
    //@{
    uint32_t GetVisualChildrenCount() const override;
    Visual* GetVisualChild(uint32_t index) const override;
    void OnConnectToView(IView* view) override;
    void OnConnectToViewChildren() override;
    void OnDisconnectFromView() override;
    void OnVisualOffsetChanged() override;
    void OnVisualTransformChanged() override;
    //@}

    /// From UIElement
    //@{
    Size MeasureCore(const Size& availableSize) override;
    void ArrangeCore(const Rect& finalRect) override;
    Ptr<Geometry> GetLayoutClip(const Size& layoutSlotSize) const override;
    UIElement* GetUIParentCore() const override;
    void OnGotFocus(const RoutedEventArgs& e) override;
    void OnRenderSizeChanged(const SizeChangedInfo& sizeInfo) override;
    //@}

private:
    bool IsStyleTriggerProviderEnabled() const;
    void StyleTriggerProviderEnable();
    void StyleTriggerProviderDisable();
    void ImplicitStyleProviderEnable();
    void ImplicitStyleProviderDisable();
    void DefaultStyleProviderEnable();
    void DefaultStyleProviderDisable();

    TemplateLocalValueProvider* GetTemplateLocalValues() const;

    Style* GetDefaultStyle() const;

    FocusVisualLayer* GetFocusVisualLayer() const;
    void SetFocusVisualLayer(FocusVisualLayer* focusVisual);

    BaseComponent* TryFindResource(IResourceKey* key, FrameworkElement* boundaryElement,
        bool themeSearch) const;
    BaseComponent* TryFindResourceInElement(IResourceKey* key,
        bool fullElementSearch) const;

    void OnFrameworkTemplateChanged(FrameworkTemplate* oldTemplate, FrameworkTemplate* newTemplate);

    void RemoveTemplatedParent();

    struct InheritedValueInfo
    {
        const DependencyProperty* dp;
        const void* val;
        const void* cur;
    };

    typedef NsVector<InheritedValueInfo> InheritedProps;
    struct InheritedComparer;

    void ResetInheritanceParent();
    FrameworkElement* GetInheritanceParent() const;
    void StoreAncestorInheritedProps(InheritedProps& props);
    void UpdateStoredInheritedProps(InheritedProps& props);
    void CheckInheritedPropsChanges(InheritedProps& props);
    void PropagateInheritedPropsChanges(const DependencyProperty* prop, bool isSubPropertyChange,
        const void* oldValue, const void* newValue);
    void InvalidateInheritedProperties();
    void OnLogicalAncestorChanged(FrameworkElement* element, bool traverseChildren,
        bool updateInherited, InheritedProps* parentProps);

    void OnResourcesChanged(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);
    void OnAncestorResourcesChanged(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);
    void OnNameScopeChanged(INameScope* sender, const NameScopeChangedArgs& args);
    void OnAncestorNameScopeChanged(FrameworkElement* element,
        const AncestorNameScopeChangedArgs& args);

    Size ApplyTransformToSize(Transform* transform, const Size& size);
    INameScope* GetNameScope() const;
    void SetParent(FrameworkElement* parent);
    void CheckPropertyMetadata(const DependencyProperty* prop, bool isSubPropertyChange,
        const void* oldValue, const void* newValue);
    Point CalculateArrangeOffset(const Size& clientSize,
        const Size& renderSize, HorizontalAlignment hAlign,
        VerticalAlignment vAlign) const;

    /// Loaded/Unloaded events
    //@{
    friend class ViewLayout;
    void RaiseLoaded();
    void OnLoaded();
    void OnUnloaded();
    //@}

    /// Management of framework element flags
    //@{
    void SetElementFlag(uint32_t flag);
    void ClearElementFlag(uint32_t flag);
    bool CheckElementFlag(uint32_t flag) const;
    //@}

    void CloneRoutedEvents(FrameworkElement* clone) const;
    void CloneEvents(FrameworkElement* clone) const;
    void CloneResources(FrameworkElement* clone) const;
    void CloneInputBindings(FrameworkElement* clone) const;
    void CloneCommandBindings(FrameworkElement* clone) const;
    void CloneTriggers(FrameworkElement* clone) const;

    void AddClonedResources(IResourceKey* key, BaseComponent* value);

    void TransferAnimations();
    void EnsureTimeManager(bool createTimeManager = true);
    bool EnsureTimeManager(FrameworkElement* parent);

    void EnsureResources();
    void ConnectResources();
    void DisconnectResources();

    void EnsureTriggers();
    void RegisterTriggers();
    void UnregisterTriggers();

    bool CancelLoadedRequest();

    EventHandler& GetDataContextChangedEvent();
    EventHandler& GetInitializedEvent();

    static void OnCursorChanged(DependencyObject* dob, const DependencyPropertyChangedEventArgs& e);
    static void StaticOnQueryCursor(BaseComponent* obj, const EventArgs& e);

    static bool ValidateSize(const void* value);
    static bool ValidateMinSize(const void* value);
    static bool ValidateMaxSize(const void* value);

    static bool ValidateMargin(const void* value);

private:
    friend class UIElementCollection;
    friend class ItemCollection;
    friend struct LogicalTreeHelper;
    friend class ContextMenu;
    friend struct ContextMenuService;
    friend class ToolTip;
    friend struct ToolTipService;
    friend class UserControl;
    friend class ContentControl;
    friend class Inline;
    friend class FocusVisualLayer;
    friend class DynamicResourceExpression;
    friend struct TemplatedParentTriggerProvider;
    friend class TemplateLocalValueProvider;
    friend struct ImplicitStyleProvider;
    friend struct StyleTriggerProvider;
    friend struct TemplateTriggerProvider;
    friend struct DefaultStyleTriggerProvider;
    friend struct DefaultStyleSetterProvider;
    friend struct InheritedPropertyProvider;
    friend class FrameworkElementTest;
    friend class StoryboardTest;

    // Animation time manager used for this element
    struct TimeManagerRef;
    Ptr<TimeManagerRef> mTimeManagerRef;

    // Parent node in the XAML tree
    IUITreeNode* mOwner;

    // Logical parent of this element
    FrameworkElement* mParent;

    // Control that contains this element as part of its template visual tree
    FrameworkElement* mTemplatedParent;

    // Template applied to this framework element
    static const DependencyProperty* TemplateProperty;

    // Stores local values for elements that are part of a template
    static const DependencyProperty* TemplateLocalValuesProperty;

    // Default style if one is available for this element type
    static const DependencyProperty* DefaultStyleProperty;

    // Layer that renders focus visual style
    static const DependencyProperty* FocusVisualLayerProperty;
    struct FocusVisualContainer;

    // Stores layout transforms and clipping geometries
    static const DependencyProperty* ArrangeTransformGroupProperty;
    static const DependencyProperty* ArrangeTransformLayoutOffsetProperty;
    static const DependencyProperty* ArrangeTransformRenderOffsetPreProperty;
    static const DependencyProperty* ArrangeTransformRenderOffsetPostProperty;
    static const DependencyProperty* ArrangeClipBoundsProperty;
    static const DependencyProperty* ArrangeClipAvailableProperty;
    static const DependencyProperty* ArrangeClipAvailableTransformProperty;
    static const DependencyProperty* ArrangeClipCombineProperty;

    // Prevents triggers to enter into an infinite recursion when invalidating
    NsVector<const DependencyProperty*> mInvalidatedProperties;

    // Element resources dictionary
    Ptr<ResourceDictionary> mResources;

    // Trigger collection
    Ptr<TriggerCollection> mTriggers;

    // Default storage for a single visual child
    Ptr<Visual> mVisualChild;

    // Desired size without applying clipping
    Size mUnclippedSize;

    // Inherited properties set in the ancestors
    InheritedProps mInheritedProps;

    // FrameworkElement flags
    int32_t mElementFlags;

    // Element request to raise Loaded event when layout finalizes
    LayoutRequest mLoadedRequest;

    // Delegates
    //@{
    AncestorChangedDelegate mAncestorChanged;
    AncestorResourcesChangedDelegate mAncestorResourcesChanged;
    AncestorNameScopeChangedDelegate mAncestorNameScopeChanged;
    //@}

    NS_DECLARE_REFLECTION(FrameworkElement, UIElement)
};

NS_WARNING_POP

}

#include <NsGui/FrameworkElement.inl>

#endif
