////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_PROPERTYPATH_H__
#define __GUI_PROPERTYPATH_H__


#include <NsCore/Noesis.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/String.h>
#include <NsCore/Ptr.h>
#include <NsCore/Symbol.h>
#include <NsCore/Vector.h>
#include <NsGui/CoreApi.h>


namespace Noesis
{

class TypeProperty;
class Collection;
class DependencyProperty;
class CollectionView;
class PropertyPathTest;
template<class T> class Delegate;
NS_INTERFACE IResourceKey;

NS_WARNING_PUSH
NS_MSVC_WARNING_DISABLE(4251 4275)

////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API PathElement
{
    PathElement();
    PathElement(const PathElement& pe);
    ~PathElement();
    
    PathElement& operator=(const PathElement& pe);

    // If you add a new member here, remember to add the proper code in copy ctor and operator=
    Ptr<BaseComponent> source;
    Ptr<CollectionView> collection;
    const TypeProperty* property;
    const DependencyProperty* dp;
    int index;
    Ptr<IResourceKey> key;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// PropertyPath
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API PropertyPath: public BaseComponent
{
public:
    PropertyPath();
    PropertyPath(const char* str);
    PropertyPath(const DependencyProperty* dp);
    ~PropertyPath();

    /// Creates and caches a property path that points to a property
    /// Returns Null if path does not corresponds to a valid property name
    static PropertyPath* Create(const char* path);

    /// Gets or sets path
    //@{
    const char* GetPath() const;
    void SetPath(const char* path);
    //@}

    /// Returns false if path cannot be resolved for the provided source
    typedef NsVector<PathElement> Elements;
    bool GetPathElements(BaseComponent* source, bool insertLastValue, Elements& elements) const;

    /// Returns false if path cannot be resolved for the provided source
    typedef Noesis::Delegate<void (const PathElement&, void*)> EnumPathElementsDelegate;
    bool EnumPathElements(BaseComponent* source, bool enumLastValue,
        const EnumPathElementsDelegate& delegate, void* context = 0) const;

private:
    void AddPathElements(const PathElement& pathElement, void* context) const;
    bool UpdatePathItems() const;
    const NsString& RebuildPath() const;

private:
    friend class PropertyPathTest;

    mutable NsString mPath;

    struct PathItem
    {
        bool slash;
        NsSymbol owner;
        NsSymbol prop;

        struct IndexKey
        {
            int index;
            NsString key;

            /// Constructor
            IndexKey();
        };

        NsVector<IndexKey> keys;

        /// Constructor
        PathItem();
    };

    typedef NsVector<PathItem> PathItems;
    mutable PathItems mPathItems;

    NS_DECLARE_REFLECTION(PropertyPath, BaseComponent)
};

NS_WARNING_POP

}

#endif
