////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__


#include <NsCore/Noesis.h>

#include <float.h>


namespace Noesis
{

/// Tests if a number is a power of 2
inline bool IsPow2(uint32_t x);

/// Gets nearest power of 2
inline uint32_t NearestPow2(uint32_t x);

/// Gets next power of 2
inline uint32_t NextPow2(uint32_t x);

/// Gets previous power of 2
inline uint32_t PrevPow2(uint32_t x);

/// Returns whether the specified value is close to 1 within the order of epsilon
inline bool IsOne(float val, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether the specified value is close to 0 within the order of epsilon 
inline bool IsZero(float val, float epsilon = 10.0f * FLT_EPSILON);

/// Returns whether or not two floats are within epsilon of each other
inline bool AreClose(float a, float b, float epsilon = 10.0f * FLT_EPSILON);

/// Tests if a number is NaN
//@{
inline bool IsNaN(float val);
inline bool IsNaN(double val);
//@}

/// Tests if a number is finite (not infinity and not Nan)
inline bool IsFinite(float val);

/// Tests if a number is positive infinity
inline bool IsPositiveInfinity(float val);

/// Tests if a number is negative infinity
inline bool IsNegativeInfinity(float val);

/// Tests if a number is positive infinity or negative infinity
inline bool IsInfinity(float val);

/// Float to Signed Int conversion with truncation towards zero
inline int Trunc(float val);

/// Float to Signed Int conversion rounding to nearest integer
inline int Round(float val);

/// Calculates the floor of a value
inline float Floor(float val);

/// Calculates the ceil of a value
inline float Ceil(float val);

/// Max returns the greater of its two arguments
template<class T> const T& Max(const T& a, const T& b);

/// Min returns the lesser of its two arguments
template<class T> const T& Min(const T& a, const T& b);

/// Clips a value between a minimum and a maximum
template<class T> T Clip(T val, T min, T max);

/// \return a linear interpolation between x and y. 0 <= t <= 1
inline float Lerp(float x, float y, float t);

/// Standard math functions
inline float Pow(float base, float exponent);
inline float Sqr(float v);
inline float Sqrt(float v);
inline float Sin(float v);
inline float Cos(float v);
inline float ASin(float v);
inline float ACos(float v);
inline float Tan(float v);
inline float ATan2(float v0, float v1);
inline int32_t Abs(int32_t v);
inline float Abs(float v);

}

/// Inline Include
#include <NsMath/Utils.inl>

#endif
