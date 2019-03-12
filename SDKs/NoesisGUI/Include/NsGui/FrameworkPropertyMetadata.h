////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __CORE_FRAMEWORKPROPERTYMETADATA_H__
#define __CORE_FRAMEWORKPROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/UIPropertyMetadata.h>
#include <NsGui/UpdateSourceTrigger.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// FrameworkOptions. Specifies the types of framework-level property behavior that pertain to a
/// particular dependency property in the property system.
////////////////////////////////////////////////////////////////////////////////////////////////////
enum FrameworkOptions
{
    FrameworkOptions_None = 1 << 0,
    FrameworkOptions_AffectsMeasure = 1 << 1,
    FrameworkOptions_AffectsArrange = 1 << 2,
    FrameworkOptions_AffectsParentMeasure = 1 << 3,
    FrameworkOptions_AffectsParentArrange = 1 << 4,
    FrameworkOptions_AffectsRender = 1 << 5,
    FrameworkOptions_Inherits = 1 << 6,
    FrameworkOptions_OverridesInheritanceBehavior = 1 << 7,
    FrameworkOptions_NotDataBindable = 1 << 8,
    FrameworkOptions_BindsTwoWayByDefault = 1 << 9,
    //FrameworkOptions_Journal = 1 << 10,
    FrameworkOptions_SubPropertiesDoNotAffectRender = 1 << 11,
    FrameworkOptions_UpdateSourceTriggerLostFocus = 1 << 12,
    FrameworkOptions_UpdateSourceTriggerExplicit = 1 << 13
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// FrameworkPropertyMetadata. Reports or applies metadata for a dependency property, specifically
/// adding framework-specific property system characteristics.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API FrameworkPropertyMetadata: public UIPropertyMetadata
{
public:
    /// Helper functions to create a new PropertyMetadata
    //@{
    inline static Ptr<FrameworkPropertyMetadata> Create();
    inline static Ptr<FrameworkPropertyMetadata> Create(int32_t options);
    inline static Ptr<FrameworkPropertyMetadata> Create(PropertyChangedCallback changed);
    inline static Ptr<FrameworkPropertyMetadata> Create(CoerceValueCallback coerce);
    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, int32_t options);
    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue,
        PropertyChangedCallback changed);
    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue,
        CoerceValueCallback coerce);
    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, int32_t options,
        PropertyChangedCallback changed);
    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, int32_t options,
        CoerceValueCallback coerce);
    template<class T>
    inline static Ptr<FrameworkPropertyMetadata> Create(const T& defaultValue, int32_t options,
        PropertyChangedCallback changed, CoerceValueCallback coerce);
    //@}

    void SetOptions(int32_t options);

    /// Indicates if the property affects measure layout pass
    inline bool AffectsMeasure() const;

    /// Indicates if the property affects arrange layout pass
    inline bool AffectsArrange() const;

    /// Indicates if the property affects parent measure layout pass
    inline bool AffectsParentMeasure() const;

    /// Indicates if the property affects parent arrange layout pass
    inline bool AffectsParentArrange() const;

    /// Indicates if the property affects render pass
    inline bool AffectsRender() const;

    /// Indicates if the property inherits its value from logical tree ancestors
    inline bool Inherits() const;

    /// Indicates whether the property value inheritance evaluation should span across certain
    /// content boundaries in the logical tree of elements
    inline bool OverridesInheritanceBehavior() const;

    /// Indicates whether the dependency property supports data binding
    inline bool IsNotDataBindable() const;

    /// Indicates whether the property binds two-way by default
    inline bool BindsTwoWayByDefault() const;

    /// Gets the default for UpdateSourceTrigger to use when bindings for the property with this 
    /// metadata are applied, which have their UpdateSourceTrigger set to Default.
    UpdateSourceTrigger DefaultUpdateSourceTrigger() const;

    /// Indicates whether sub-properties of the dependency property do not affect the rendering of
    /// the containing object
    inline bool SubPropertiesDoNotAffectRender() const;

    /// From PropertyMetadata
    //@{
    void ClearInheritedValues();
    void Merge(const PropertyMetadata* ancestor);
    //@}

protected:
    // Constructors
    //@{
    FrameworkPropertyMetadata();
    FrameworkPropertyMetadata(int32_t options);
    FrameworkPropertyMetadata(PropertyChangedCallback changed);
    FrameworkPropertyMetadata(CoerceValueCallback coerce);
    FrameworkPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        PropertyChangedCallback changed);
    FrameworkPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        CoerceValueCallback coerce);
    FrameworkPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        int32_t options);
    FrameworkPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        int32_t options, PropertyChangedCallback changed);
    FrameworkPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        int32_t options, CoerceValueCallback coerce);
    FrameworkPropertyMetadata(ValueStorageManager* defaultValueManager, const void* defaultValue,
        int32_t options, PropertyChangedCallback changed, CoerceValueCallback coerce);
    //@}

private:
    int32_t mLocalOptions;
    int32_t mFrameworkOptions;

    NS_DECLARE_REFLECTION(FrameworkPropertyMetadata, UIPropertyMetadata)
};

}

#include <NsGui/FrameworkPropertyMetadata.inl>

#endif
