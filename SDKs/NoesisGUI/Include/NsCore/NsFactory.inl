////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Ptr.h>
#include <NsCore/BaseComponent.h>
#include <NsCore/ComponentFactory.h>
#include <NsCore/Kernel.h>
#include <NsCore/DynamicCast.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline Noesis::Ptr<T> NsCreateComponent(NsSymbol classId)
{
    return NsDynamicCast<Noesis::Ptr<T> >(
        NsGetKernel()->GetComponentFactory()->CreateComponent(classId));
}
