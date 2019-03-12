////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #1197]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PROPERTYMETADATA_H__
#define __GUI_PROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/DependencySystemApi.h>
#include <NsGui/ValueStorageManager.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/Delegate.h>


namespace Noesis
{

class DependencyProperty;
class DependencyObject;
struct DependencyPropertyChangedEventArgs;

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef Delegate<void (DependencyObject* d, const DependencyPropertyChangedEventArgs& e)>
    PropertyChangedCallback;

////////////////////////////////////////////////////////////////////////////////////////////////////
typedef bool (*CoerceValueCallback)(const DependencyObject* d, const void* value,
    void* coercedValue);

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// PropertyMetadata. Defines certain behavior aspects of a dependency property as it is applied to
/// a specific type, including conditions it was registered with.
///
/// WPF PropertyMetadata fields:
///  - DefaultValue
///  - PropertyChangedCallback
///  - CoerceValueCallback
///  - IsSealed
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_DEPENDENCYSYSTEM_API PropertyMetadata: public BaseComponent
{
public:
    /// Helper functions to create a new PropertyMetadata
    //@{
    inline static Ptr<PropertyMetadata> Create();

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue);

    inline static Ptr<PropertyMetadata> Create(const PropertyChangedCallback& changed);

    inline static Ptr<PropertyMetadata> Create(CoerceValueCallback coerce);

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue,
        const PropertyChangedCallback& changed);

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue,
        CoerceValueCallback coerce);

    template<class T>
    inline static Ptr<PropertyMetadata> Create(const T& defaultValue,
        const PropertyChangedCallback& changed, CoerceValueCallback coerce);
    //@}

    ~PropertyMetadata();

    inline bool HasDefaultValue() const;
    inline const void* GetDefaultValue() const;
    inline Ptr<BaseComponent> GetDefaultValueObject() const;
    void SetDefaultValue(ValueStorageManager* defaultValueManager, const void* defaultValue);

    inline ValueStorageManager* GetValueManager() const;

    inline bool HasPropertyChangedCallback() const;
    inline const PropertyChangedCallback& GetPropertyChangedCallback() const;
    void SetPropertyChangedCallback(const PropertyChangedCallback& changed);

    inline bool HasCoerceValueCallback() const;
    inline CoerceValueCallback GetCoerceValueCallback() const;
    void SetCoerceValueCallback(CoerceValueCallback coerce);

    inline bool IsUncached() const;
    inline bool Uncached() const;

    void SetAsUncached();

    /// Remove inherited values from ancestors
    virtual void ClearInheritedValues();

    /// Inherit ancestor values if they are not local in this class
    virtual void Merge(const PropertyMetadata* ancestor);

    /// Notifies listeners when metadata is destroyed
    typedef Delegate<void(PropertyMetadata* m)> DestroyedDelegate;
    DestroyedDelegate& Destroyed();

protected:
    /// Constructors
    //@{
    PropertyMetadata();
    PropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue);
    PropertyMetadata(const PropertyChangedCallback& changed);
    PropertyMetadata(CoerceValueCallback coerce);
    PropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        const PropertyChangedCallback& changed);
    PropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        CoerceValueCallback coerce);
    PropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        const PropertyChangedCallback& changed, CoerceValueCallback coerce);
    //@}

    /// From BaseComponent
    //@{
    int32_t OnDestroy() const;
    //@}

private:
    void ResetDefaultValue();

private:
    ValueStorage mDefaultValue;
    ValueStorageManager* mDefaultValueManager;
    PropertyChangedCallback mLocalChanged;
    PropertyChangedCallback mChanged;
    CoerceValueCallback mCoerce;

    DestroyedDelegate mDestroyed;

    enum PropertyFlags
    {
        PropertyFlags_None = 0,
        PropertyFlags_Default = 1 << 1,
        PropertyFlags_Coerce = 1 << 2,
        PropertyFlags_Changed = 1 << 3,
        PropertyFlags_Uncached = 1 << 4
    };

    /// Flag to control if properties value are local or inherited
    uint32_t mLocalFlags;

    /// Tells if the property must not be cached in the DependencyObject internal hash
    bool mUncached;

    NS_DECLARE_REFLECTION(PropertyMetadata, BaseComponent)
};

NS_WARNING_POP

}

/// Inline Include
#include <NsGui/PropertyMetadata.inl>

#endif
