////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_SIZECHANGEDINFO_H__
#define __GUI_SIZECHANGEDINFO_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsGui/RoutedEvent.h>
#include <NsCore/ReflectionDeclare.h>
#include <NsDrawing/Size.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Reports a size change. It is used as a parameter in OnRenderSizeChanged overrides.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct SizeChangedInfo
{
    Size newSize;
    Size previousSize;
    bool widthChanged;
    bool heightChanged;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API SizeChangedEventArgs: public RoutedEventArgs
{
    SizeChangedInfo sizeChangedInfo;

    SizeChangedEventArgs(BaseComponent* s, const RoutedEvent* e, 
        const SizeChangedInfo& sizeChangedInfo);
};

}

#endif
