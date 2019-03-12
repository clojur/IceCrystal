////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
// [CR #868]
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __DRAWING_POINT_H__
#define __DRAWING_POINT_H__


#include <NsCore/Noesis.h>
#include <NsDrawing/TypesApi.h>
#include <NsDrawing/Size.h>
#include <NsMath/Vector.h>
#include <NsCore/NSTLForwards.h>


namespace Noesis
{

struct Point;
struct Pointi;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Point. Represents an x- and y-coordinate pair in two-dimensional space.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Point: public Noesis::Vector2f
{
    /// Default constructor that creates a (0,0) point
    inline Point();

    /// Constructor for x, y
    inline Point(float x, float y);

    /// Construct from Pointi
    inline Point(const Pointi& point);

    /// Constructor from Vector
    inline Point(const Vector2f& v);

    /// Constructor from size
    explicit inline Point(const Size& size);

    /// Copy constructor
    inline Point(const Point& point);

    /// Copy operator
    inline Point& operator=(const Point& point);

    /// Generates a string representation of the point
    /// The string has the following form: "x,y"
    NS_DRAWING_TYPES_API NsString ToString() const;

    /// Tries to parse a Point from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Point& result);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pointi. Represents an x- and y-coordinate pair in integer two-dimensional space.
////////////////////////////////////////////////////////////////////////////////////////////////////
struct Pointi: public Noesis::Vector2i
{
    /// Default constructor that creates a (0,0) point
    inline Pointi();

    /// Constructor for x, y
    inline Pointi(int x, int y);

    /// Constructor from float Point
    inline Pointi(const Point& p);

    /// Constructor from Vector
    inline Pointi(const Vector2i& v);

    /// Constructor from size
    explicit inline Pointi(const Sizei& size);

    /// Copy constructor
    inline Pointi(const Pointi& point);

    /// Copy operator
    inline Pointi& operator=(const Pointi& point);

    /// Generates a string representation of the point
    /// The string has the following form: "x,y"
    NS_DRAWING_TYPES_API NsString ToString() const;

    /// Tries to parse a Pointi from a string
    NS_DRAWING_TYPES_API static bool TryParse(const char* str, Pointi& result);
};

}

#include <NsDrawing/Point.inl>

#endif
