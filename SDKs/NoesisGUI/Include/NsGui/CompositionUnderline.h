////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COMPOSITIONUNDERLINE_H__
#define __GUI_COMPOSITIONUNDERLINE_H__


#include <NsCore/Noesis.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
enum CompositionLineStyle
{
    CompositionLineStyle_None = 0,
    CompositionLineStyle_Solid = 1,
    CompositionLineStyle_Dot = 2,
    CompositionLineStyle_Dash = 3,
    CompositionLineStyle_Squiggle = 4
};

////////////////////////////////////////////////////////////////////////////////////////////////////
struct CompositionUnderline
{
    uint32_t start;
    uint32_t end;
    CompositionLineStyle style;
    bool bold;
};

}

#endif
