////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_COLORS_H__
#define __GUI_COLORS_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsDrawing/Color.h>


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Declares a set of predefined Color objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API Colors
{
    /// List of predefined colors
    //@{
    static Color AliceBlue(); // #FFF0F8FF
    static Color AntiqueWhite(); // #FFFAEBD7
    static Color Aqua(); // #FF00FFFF
    static Color Aquamarine(); // #FF7FFFD4
    static Color Azure(); // #FFF0FFFF
    static Color Beige(); // #FFF5F5DC
    static Color Bisque(); // #FFFFE4C4
    static Color Black(); // #FF000000
    static Color BlanchedAlmond(); // #FFFFEBCD
    static Color Blue(); // #FF0000FF
    static Color BlueViolet(); // #FF8A2BE2
    static Color Brown(); // #FFA52A2A
    static Color BurlyWood(); // #FFDEB887
    static Color CadetBlue(); // #FF5F9EA0
    static Color Chartreuse(); // #FF7FFF00
    static Color Chocolate(); // #FFD2691E
    static Color Coral(); // #FFFF7F50
    static Color CornflowerBlue(); // #FF6495ED
    static Color Cornsilk(); // #FFFFF8DC
    static Color Crimson(); // #FFDC143C
    static Color Cyan(); // #FF00FFFF
    static Color DarkBlue(); // #FF00008B
    static Color DarkCyan(); // #FF008B8B
    static Color DarkGoldenrod(); // #FFB8860B
    static Color DarkGray(); // #FFA9A9A9
    static Color DarkGreen(); // #FF006400
    static Color DarkKhaki(); // #FFBDB76B
    static Color DarkMagenta(); // #FF8B008B
    static Color DarkOliveGreen(); // #FF556B2F
    static Color DarkOrange(); // #FFFF8C00
    static Color DarkOrchid(); // #FF9932CC
    static Color DarkRed(); // #FF8B0000
    static Color DarkSalmon(); // #FFE9967A
    static Color DarkSeaGreen(); // #FF8FBC8F
    static Color DarkSlateBlue(); // #FF483D8B
    static Color DarkSlateGray(); // #FF2F4F4F
    static Color DarkTurquoise(); // #FF00CED1
    static Color DarkViolet(); // #FF9400D3
    static Color DeepPink(); // #FFFF1493
    static Color DeepSkyBlue(); // #FF00BFFF
    static Color DimGray(); // #FF696969
    static Color DodgerBlue(); // #FF1E90FF
    static Color Firebrick(); // #FFB22222
    static Color FloralWhite(); // #FFFFFAF0
    static Color ForestGreen(); // #FF228B22
    static Color Fuchsia(); // #FFFF00FF
    static Color Gainsboro(); // #FFDCDCDC
    static Color GhostWhite(); // #FFF8F8FF
    static Color Gold(); // #FFFFD700
    static Color Goldenrod(); // #FFDAA520
    static Color Gray(); // #FF808080
    static Color Green(); // #FF008000
    static Color GreenYellow(); // #FFADFF2F
    static Color Honeydew(); // #FFF0FFF0
    static Color HotPink(); // #FFFF69B4
    static Color IndianRed(); // #FFCD5C5C
    static Color Indigo(); // #FF4B0082
    static Color Ivory(); // #FFFFFFF0
    static Color Khaki(); // #FFF0E68C
    static Color Lavender(); // #FFE6E6FA
    static Color LavenderBlush(); // #FFFFF0F5
    static Color LawnGreen(); // #FF7CFC00
    static Color LemonChiffon(); // #FFFFFACD
    static Color LightBlue(); // #FFADD8E6
    static Color LightCoral(); // #FFF08080
    static Color LightCyan(); // #FFE0FFFF
    static Color LightGoldenrodYellow(); // #FFFAFAD2
    static Color LightGray(); // #FFD3D3D3
    static Color LightGreen(); // #FF90EE90
    static Color LightPink(); // #FFFFB6C1
    static Color LightSalmon(); // #FFFFA07A
    static Color LightSeaGreen(); // #FF20B2AA
    static Color LightSkyBlue(); // #FF87CEFA
    static Color LightSlateGray(); // #FF778899
    static Color LightSteelBlue(); // #FFB0C4DE
    static Color LightYellow(); // #FFFFFFE0
    static Color Lime(); // #FF00FF00
    static Color LimeGreen(); // #FF32CD32
    static Color Linen(); // #FFFAF0E6
    static Color Magenta(); // #FFFF00FF
    static Color Maroon(); // #FF800000
    static Color MediumAquamarine(); // #FF66CDAA
    static Color MediumBlue(); // #FF0000CD
    static Color MediumOrchid(); // #FFBA55D3
    static Color MediumPurple(); // #FF9370DB
    static Color MediumSeaGreen(); // #FF3CB371
    static Color MediumSlateBlue(); // #FF7B68EE
    static Color MediumSpringGreen(); // #FF00FA9A
    static Color MediumTurquoise(); // #FF48D1CC
    static Color MediumVioletRed(); // #FFC71585
    static Color MidnightBlue(); // #FF191970
    static Color MintCream(); // #FFF5FFFA
    static Color MistyRose(); // #FFFFE4E1
    static Color Moccasin(); // #FFFFE4B5
    static Color NavajoWhite(); // #FFFFDEAD
    static Color Navy(); // #FF000080
    static Color OldLace(); // #FFFDF5E6
    static Color Olive(); // #FF808000
    static Color OliveDrab(); // #FF6B8E23
    static Color Orange(); // #FFFFA500
    static Color OrangeRed(); // #FFFF4500
    static Color Orchid(); // #FFDA70D6
    static Color PaleGoldenrod(); // #FFEEE8AA
    static Color PaleGreen(); // #FF98FB98
    static Color PaleTurquoise(); // #FFAFEEEE
    static Color PaleVioletRed(); // #FFDB7093
    static Color PapayaWhip(); // #FFFFEFD5
    static Color PeachPuff(); // #FFFFDAB9
    static Color Peru(); // #FFCD853F
    static Color Pink(); // #FFFFC0CB
    static Color Plum(); // #FFDDA0DD
    static Color PowderBlue(); // #FFB0E0E6
    static Color Purple(); // #FF800080
    static Color Red(); // #FFFF0000
    static Color RosyBrown(); // #FFBC8F8F
    static Color RoyalBlue(); // #FF4169E1
    static Color SaddleBrown(); // #FF8B4513
    static Color Salmon(); // #FFFA8072
    static Color SandyBrown(); // #FFF4A460
    static Color SeaGreen(); // #FF2E8B57
    static Color SeaShell(); // #FFFFF5EE
    static Color Sienna(); // #FFA0522D
    static Color Silver(); // #FFC0C0C0
    static Color SkyBlue(); // #FF87CEEB
    static Color SlateBlue(); // #FF6A5ACD
    static Color SlateGray(); // #FF708090
    static Color Snow(); // #FFFFFAFA
    static Color SpringGreen(); // #FF00FF7F
    static Color SteelBlue(); // #FF4682B4
    static Color Tan(); // #FFD2B48C
    static Color Teal(); // #FF008080
    static Color Thistle(); // #FFD8BFD8
    static Color Tomato(); // #FFFF6347
    static Color Transparent(); // #00FFFFFF
    static Color Turquoise(); // #FF40E0D0
    static Color Violet(); // #FFEE82EE
    static Color Wheat(); // #FFF5DEB3
    static Color White(); // #FFFFFFFF
    static Color WhiteSmoke(); // #FFF5F5F5
    static Color Yellow(); // #FFFFFF00
    static Color YellowGreen(); // #FF9ACD32
    //@}
};

}

#endif
