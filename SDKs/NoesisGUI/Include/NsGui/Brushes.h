////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __GUI_BRUSHES_H__
#define __GUI_BRUSHES_H__


#include <NsCore/Noesis.h>
#include <NsGui/CoreApi.h>
#include <NsCore/Ptr.h>
#include <NsCore/ReflectionDeclare.h>


namespace Noesis
{

// Forward declaration
//@{
class Color;
class SolidColorBrush;
//@}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Declares a set of predefined SolidColorBrush objects.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct NS_GUI_CORE_API Brushes
{
    /// List of predefined brushes
    //@{
    static SolidColorBrush* AliceBlue(); // #FFF0F8FF
    static SolidColorBrush* AntiqueWhite(); // #FFFAEBD7
    static SolidColorBrush* Aqua(); // #FF00FFFF
    static SolidColorBrush* Aquamarine(); // #FF7FFFD4
    static SolidColorBrush* Azure(); // #FFF0FFFF
    static SolidColorBrush* Beige(); // #FFF5F5DC
    static SolidColorBrush* Bisque(); // #FFFFE4C4
    static SolidColorBrush* Black(); // #FF000000
    static SolidColorBrush* BlanchedAlmond(); // #FFFFEBCD
    static SolidColorBrush* Blue(); // #FF0000FF
    static SolidColorBrush* BlueViolet(); // #FF8A2BE2
    static SolidColorBrush* Brown(); // #FFA52A2A
    static SolidColorBrush* BurlyWood(); // #FFDEB887
    static SolidColorBrush* CadetBlue(); // #FF5F9EA0
    static SolidColorBrush* Chartreuse(); // #FF7FFF00
    static SolidColorBrush* Chocolate(); // #FFD2691E
    static SolidColorBrush* Coral(); // #FFFF7F50
    static SolidColorBrush* CornflowerBlue(); // #FF6495ED
    static SolidColorBrush* Cornsilk(); // #FFFFF8DC
    static SolidColorBrush* Crimson(); // #FFDC143C
    static SolidColorBrush* Cyan(); // #FF00FFFF
    static SolidColorBrush* DarkBlue(); // #FF00008B
    static SolidColorBrush* DarkCyan(); // #FF008B8B
    static SolidColorBrush* DarkGoldenrod(); // #FFB8860B
    static SolidColorBrush* DarkGray(); // #FFA9A9A9
    static SolidColorBrush* DarkGreen(); // #FF006400
    static SolidColorBrush* DarkKhaki(); // #FFBDB76B
    static SolidColorBrush* DarkMagenta(); // #FF8B008B
    static SolidColorBrush* DarkOliveGreen(); // #FF556B2F
    static SolidColorBrush* DarkOrange(); // #FFFF8C00
    static SolidColorBrush* DarkOrchid(); // #FF9932CC
    static SolidColorBrush* DarkRed(); // #FF8B0000
    static SolidColorBrush* DarkSalmon(); // #FFE9967A
    static SolidColorBrush* DarkSeaGreen(); // #FF8FBC8F
    static SolidColorBrush* DarkSlateBlue(); // #FF483D8B
    static SolidColorBrush* DarkSlateGray(); // #FF2F4F4F
    static SolidColorBrush* DarkTurquoise(); // #FF00CED1
    static SolidColorBrush* DarkViolet(); // #FF9400D3
    static SolidColorBrush* DeepPink(); // #FFFF1493
    static SolidColorBrush* DeepSkyBlue(); // #FF00BFFF
    static SolidColorBrush* DimGray(); // #FF696969
    static SolidColorBrush* DodgerBlue(); // #FF1E90FF
    static SolidColorBrush* Firebrick(); // #FFB22222
    static SolidColorBrush* FloralWhite(); // #FFFFFAF0
    static SolidColorBrush* ForestGreen(); // #FF228B22
    static SolidColorBrush* Fuchsia(); // #FFFF00FF
    static SolidColorBrush* Gainsboro(); // #FFDCDCDC
    static SolidColorBrush* GhostWhite(); // #FFF8F8FF
    static SolidColorBrush* Gold(); // #FFFFD700
    static SolidColorBrush* Goldenrod(); // #FFDAA520
    static SolidColorBrush* Gray(); // #FF808080
    static SolidColorBrush* Green(); // #FF008000
    static SolidColorBrush* GreenYellow(); // #FFADFF2F
    static SolidColorBrush* Honeydew(); // #FFF0FFF0
    static SolidColorBrush* HotPink(); // #FFFF69B4
    static SolidColorBrush* IndianRed(); // #FFCD5C5C
    static SolidColorBrush* Indigo(); // #FF4B0082
    static SolidColorBrush* Ivory(); // #FFFFFFF0
    static SolidColorBrush* Khaki(); // #FFF0E68C
    static SolidColorBrush* Lavender(); // #FFE6E6FA
    static SolidColorBrush* LavenderBlush(); // #FFFFF0F5
    static SolidColorBrush* LawnGreen(); // #FF7CFC00
    static SolidColorBrush* LemonChiffon(); // #FFFFFACD
    static SolidColorBrush* LightBlue(); // #FFADD8E6
    static SolidColorBrush* LightCoral(); // #FFF08080
    static SolidColorBrush* LightCyan(); // #FFE0FFFF
    static SolidColorBrush* LightGoldenrodYellow(); // #FFFAFAD2
    static SolidColorBrush* LightGray(); // #FFD3D3D3
    static SolidColorBrush* LightGreen(); // #FF90EE90
    static SolidColorBrush* LightPink(); // #FFFFB6C1
    static SolidColorBrush* LightSalmon(); // #FFFFA07A
    static SolidColorBrush* LightSeaGreen(); // #FF20B2AA
    static SolidColorBrush* LightSkyBlue(); // #FF87CEFA
    static SolidColorBrush* LightSlateGray(); // #FF778899
    static SolidColorBrush* LightSteelBlue(); // #FFB0C4DE
    static SolidColorBrush* LightYellow(); // #FFFFFFE0
    static SolidColorBrush* Lime(); // #FF00FF00
    static SolidColorBrush* LimeGreen(); // #FF32CD32
    static SolidColorBrush* Linen(); // #FFFAF0E6
    static SolidColorBrush* Magenta(); // #FFFF00FF
    static SolidColorBrush* Maroon(); // #FF800000
    static SolidColorBrush* MediumAquamarine(); // #FF66CDAA
    static SolidColorBrush* MediumBlue(); // #FF0000CD
    static SolidColorBrush* MediumOrchid(); // #FFBA55D3
    static SolidColorBrush* MediumPurple(); // #FF9370DB
    static SolidColorBrush* MediumSeaGreen(); // #FF3CB371
    static SolidColorBrush* MediumSlateBlue(); // #FF7B68EE
    static SolidColorBrush* MediumSpringGreen(); // #FF00FA9A
    static SolidColorBrush* MediumTurquoise(); // #FF48D1CC
    static SolidColorBrush* MediumVioletRed(); // #FFC71585
    static SolidColorBrush* MidnightBlue(); // #FF191970
    static SolidColorBrush* MintCream(); // #FFF5FFFA
    static SolidColorBrush* MistyRose(); // #FFFFE4E1
    static SolidColorBrush* Moccasin(); // #FFFFE4B5
    static SolidColorBrush* NavajoWhite(); // #FFFFDEAD
    static SolidColorBrush* Navy(); // #FF000080
    static SolidColorBrush* OldLace(); // #FFFDF5E6
    static SolidColorBrush* Olive(); // #FF808000
    static SolidColorBrush* OliveDrab(); // #FF6B8E23
    static SolidColorBrush* Orange(); // #FFFFA500
    static SolidColorBrush* OrangeRed(); // #FFFF4500
    static SolidColorBrush* Orchid(); // #FFDA70D6
    static SolidColorBrush* PaleGoldenrod(); // #FFEEE8AA
    static SolidColorBrush* PaleGreen(); // #FF98FB98
    static SolidColorBrush* PaleTurquoise(); // #FFAFEEEE
    static SolidColorBrush* PaleVioletRed(); // #FFDB7093
    static SolidColorBrush* PapayaWhip(); // #FFFFEFD5
    static SolidColorBrush* PeachPuff(); // #FFFFDAB9
    static SolidColorBrush* Peru(); // #FFCD853F
    static SolidColorBrush* Pink(); // #FFFFC0CB
    static SolidColorBrush* Plum(); // #FFDDA0DD
    static SolidColorBrush* PowderBlue(); // #FFB0E0E6
    static SolidColorBrush* Purple(); // #FF800080
    static SolidColorBrush* Red(); // #FFFF0000
    static SolidColorBrush* RosyBrown(); // #FFBC8F8F
    static SolidColorBrush* RoyalBlue(); // #FF4169E1
    static SolidColorBrush* SaddleBrown(); // #FF8B4513
    static SolidColorBrush* Salmon(); // #FFFA8072
    static SolidColorBrush* SandyBrown(); // #FFF4A460
    static SolidColorBrush* SeaGreen(); // #FF2E8B57
    static SolidColorBrush* SeaShell(); // #FFFFF5EE
    static SolidColorBrush* Sienna(); // #FFA0522D
    static SolidColorBrush* Silver(); // #FFC0C0C0
    static SolidColorBrush* SkyBlue(); // #FF87CEEB
    static SolidColorBrush* SlateBlue(); // #FF6A5ACD
    static SolidColorBrush* SlateGray(); // #FF708090
    static SolidColorBrush* Snow(); // #FFFFFAFA
    static SolidColorBrush* SpringGreen(); // #FF00FF7F
    static SolidColorBrush* SteelBlue(); // #FF4682B4
    static SolidColorBrush* Tan(); // #FFD2B48C
    static SolidColorBrush* Teal(); // #FF008080
    static SolidColorBrush* Thistle(); // #FFD8BFD8
    static SolidColorBrush* Tomato(); // #FFFF6347
    static SolidColorBrush* Transparent(); // #00FFFFFF
    static SolidColorBrush* Turquoise(); // #FF40E0D0
    static SolidColorBrush* Violet(); // #FFEE82EE
    static SolidColorBrush* Wheat(); // #FFF5DEB3
    static SolidColorBrush* White(); // #FFFFFFFF
    static SolidColorBrush* WhiteSmoke(); // #FFF5F5F5
    static SolidColorBrush* Yellow(); // #FFFFFF00
    static SolidColorBrush* YellowGreen(); // #FF9ACD32
    //@}

    /// Returns the predefined solid color brush that corresponds to the given color
    static Ptr<SolidColorBrush> FromColor(const Color& color);

    NS_DECLARE_REFLECTION(Brushes, NoParent)
};

}

#endif
