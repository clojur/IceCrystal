////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_AMBIENTPROPERTYMETADATA_H__
#define __GUI_AMBIENTPROPERTYMETADATA_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsCore/TypeMetaData.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// AmbientPropertyMetaData. Makes the value of the property available in the parser context to 
/// objects that exist as child objects in an object tree.
////////////////////////////////////////////////////////////////////////////////////////////////////
class NS_GUI_CORE_API AmbientPropertyMetaData: public TypeMetaData
{
private:
    NS_DECLARE_REFLECTION(AmbientPropertyMetaData, TypeMetaData)
};

}

#endif
