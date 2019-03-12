////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #696]
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Error.h>


namespace Noesis
{

const int32_t ObjectAlreadyDeleted = int32_t(0xF3EEF3EE);


////////////////////////////////////////////////////////////////////////////////////////////////////
BaseRefCounted::BaseRefCounted()
{
    mRefCount = 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
BaseRefCounted::~BaseRefCounted()
{
    NS_ASSERT(mRefCount != ObjectAlreadyDeleted);
    // mRefCount == 1 is a valid number. Even if we force to always use Ptr<>, for example, if
    // the constructor BaseRefCounted() is successfully invoked but the constructor of an
    // inheriting class raise an exception, ~BaseRefCounted() will be invoked with 1 references
    NS_ASSERT(mRefCount == 1 || mRefCount == 0);

#if NS_ASSERTS_ENABLED
    mRefCount = ObjectAlreadyDeleted;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t BaseRefCounted::AddReference() const
{
    return InternalAddReference();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t BaseRefCounted::Release() const
{
    return InternalRelease();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t BaseRefCounted::GetNumReferences() const
{
    return mRefCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t BaseRefCounted::OnDestroy() const
{
#ifdef NS_TRACK_COMPONENTS
    TrackObjectDealloc(GetClassType()->GetName());
#endif
    NS_ASSERT(mRefCount != ObjectAlreadyDeleted);
    delete this;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t BaseRefCounted::InternalAddReference() const
{
    NS_ASSERT(mRefCount != ObjectAlreadyDeleted);
    return ++mRefCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t BaseRefCounted::InternalRelease() const
{
    NS_ASSERT(mRefCount > 0);
    
    int32_t ref = --mRefCount;

    if (ref == 0)
    {
        return OnDestroy();
    }

    return ref;
}

}
