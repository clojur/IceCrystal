////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BINDINGEXPRESSION_H__
#define __GUI_BINDINGEXPRESSION_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Ptr.h>
#include <NsGui/CoreApi.h>
#include <NsGui/PropertyPath.h>
#include <NsGui/Binding.h>
#include <NsGui/BaseBindingExpression.h>


namespace Noesis
{

NS_INTERFACE IValueConverter;
NS_INTERFACE IResourceKey;
NS_INTERFACE INameScope;
class DependencyObject;
class DependencyProperty;
class FrameworkElement;
class CollectionView;
struct PropertyChangedEventArgs;
struct NotifyDictionaryChangedEventArgs;
struct NotifyCollectionChangedEventArgs;
struct DependencyPropertyChangedEventArgs;
struct RoutedEventArgs;
struct EventArgs;
struct AncestorNameScopeChangedArgs;
struct NameScopeChangedArgs;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// BindingExpression. Provides high-level access to the definition of a binding, which connects the
/// properties of binding target objects (UI elements), and any data source (for example, a
/// database, an XML file, or any object that contains data).
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API BindingExpression: public BaseBindingExpression
{
public:
    ~BindingExpression();

    /// Binding from which this expression was created
    /// \prop
    Binding* GetParentBinding() const;

    /// From BaseBindingExpression
    //@{
    void UpdateTarget() const;
    void UpdateSource() const;
    //@}

    /// From IExpression
    //@{
    Ptr<BaseComponent> Evaluate() const;
    Ptr<IExpression> Reapply(DependencyObject* targetObject,
        const DependencyProperty* targetProperty) const;
    BeforeSetAction BeforeSet(DependencyObject* obj, const DependencyProperty* prop, 
        const void* value, bool valueChanged);
    void AfterSet(DependencyObject* obj, const DependencyProperty* prop, 
        const void* value, bool valueChanged);
    //@}

private:
    friend class Binding;
    BindingExpression(BaseBinding* binding, DependencyObject* targetObject,
        const DependencyProperty* targetProperty);

    bool IsAttached() const;

    void Register();
    void Unregister();

    void RegisterTarget(FrameworkElement* target, Binding* binding);
    void UnregisterTarget(FrameworkElement* target);

    void RegisterScope();
    void UnregisterScope();

    void Initialize();
    void Shutdown();

    void AddPathElement(const PathElement& element, void* context);

    void RegisterSource();
    void UnregisterSource();

    struct WeakPathElement;
    uint32_t GetRegisteredFlags(uint32_t end, const WeakPathElement& element) const;
    void RegisterNotification(const WeakPathElement& element, uint32_t registeredFlags);
    void UnregisterNotification(const WeakPathElement& element, uint32_t registeredFlags);
    bool IsSelf() const;
    bool IsSourceReadOnly() const;
    Ptr<BaseComponent> GetValue(const WeakPathElement& element, const Type*& valueType) const;

    /// Checks if a converter is needed, creating one when required
    bool CheckConverter(const Type* sourceType, BaseComponent* sourceVal) const;

    void UpdateSourceInternal(const void* value) const;
    void TransferSourceValue(const void* value) const;
    Ptr<BaseComponent> GetSourceValue() const;

    void InvalidateSource(bool invalidateTargetProperty = true);

    // Source change notification
    //@{
    void OnDataContextParentChanged(FrameworkElement* oldParent);

    void OnDataContextChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);

    void OnTargetAncestorChanged(FrameworkElement* oldParent);

    void OnAncestorNameScopeChanged(FrameworkElement* sender,
        const AncestorNameScopeChangedArgs& args);

    void OnSourceNameScopeChanged(INameScope* sender, const NameScopeChangedArgs& args);
    //@}

    // Property change notifications
    //@{
    void OnDependencyPropertyChanged(BaseComponent* sender,
        const DependencyPropertyChangedEventArgs& args);

    void OnNotifyPropertyChanged(BaseComponent* sender, const PropertyChangedEventArgs& e);

    void OnCollectionChanged(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);
    void OnCollectionReset(BaseComponent* sender,
        const NotifyCollectionChangedEventArgs& args);

    void OnDictionaryChanged(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);
    void OnDictionaryReset(BaseComponent* sender,
        const NotifyDictionaryChangedEventArgs& args);

    void OnCurrentChanged(BaseComponent* sender, const EventArgs& args);
    //@}

    /// Event notification
    //@{
    void OnTargetLostFocus(BaseComponent*, const RoutedEventArgs& args);
    //@}

    // HACK #884
    //@{
    void OnTargetDestroyed(DependencyObject* object);
    void OnSourceDestroyed(DependencyObject* object);
    //@}

    bool IsTargetAlive() const;

private:
    /// Source object for the expression
    BaseComponent* mSource;
    FrameworkElement* mTargetElement;
    INameScope* mNameScope;

    struct WeakPathElement
    {
        BaseComponent* source;
        CollectionView* collection;
        const TypeProperty* property;
        const DependencyProperty* dp;
        int index;
        Ptr<IResourceKey> key;
    };

    /// List of objects to access final source property
    NsVector<WeakPathElement> mPaths;

    /// Effective binding mode (never set to Default because the proper value is retrieved from 
    /// metadata)
    BindingMode mEffectiveBindingMode;

    /// Value converter is used when source and target property types are different, or when set by
    /// the user in the Converter field of the Binding class.
    mutable Ptr<IValueConverter> mEffectiveConverter;

    /// The effective fallback value to assign in case of fail in the binding
    Ptr<BaseComponent> mEffectiveFallback;
    
    /// The effective UpdateSourceTrigger value to use
    UpdateSourceTrigger mEffectiveUpdateSourceTrigger;

    union
    {
        mutable struct
        {
            bool fullReevaluationNeeded : 1;
            bool pathPartiallySolved : 1;
            bool sourceRegistered : 1;
            bool targetDestroyRegistered : 1;
            bool targetLostFocusRegistered : 1;
            bool targetAncestorChangedRegistered : 1;
            bool targetDataContextParentChangedRegistered : 1;
            bool targetDataContextChangedRegistered : 1;
            bool targetAncestorNameScopeChangedRegistered : 1;
            bool propertyChangeRegistered : 1;
            bool updatingSource : 1;
            bool transferingSourceValue : 1;
            bool doNothing : 1;
            bool checkConverter : 1;
            bool oneTimeEvaluated : 1;
        } mFlags;

        // To quickly set all flags to 0
        uint32_t mAllFlags;
    };

    NS_DECLARE_REFLECTION(BindingExpression, BaseBindingExpression)
};

NS_WARNING_POP

}

#endif
