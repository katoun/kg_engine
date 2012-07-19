/*
-----------------------------------------------------------------------------
Copyright (c) 2006-2012 Catalin Alexandru Nastase

KG game engine (http://k-game.sourceforge.net) is made available under the LGPL License (http://www.gnu.org/copyleft/lgpl.html)

Under the LGPL you may use KG game engine for any purpose you wish, as long as you:
1. Release any modifications to the KG game engine source back to the community
2. Pass on the source to KG game engine with all the copyrights intact, or link back to a place where the source code can be obtained (http://k-game.sourceforge.net)
3. Make it clear where you have customised it.
The above is a precis, please do read the full license agreement.
-----------------------------------------------------------------------------
*/

#ifndef _MATH_H_
#define _MATH_H_

#include <core/Config.h>

namespace core
{

#undef min
#undef max

const float EPSILON			= 1.192092896e-07f;										//! Constant often used when comparing float values.
const unsigned int UNSIGNEDINT_MAX	= 0xffffffff;											//! Constant for max unsigned int.
const float FLOAT_MIN			= 1.175494351e-38f;										//! Constant for min float.
const float FLOAT_MAX			= 3.402823466e+38f;										//! Constant for max float.
const float PI				= 3.1415926535897932384626433832795f;					//! Constant for PI.
const float HALF_PI			= 1.5707963267948966192313216916398f;//0.5f * PI;		//! Constant for PI/2.
const float DEGTORAD			= 0.0174532925199432957692369076848f;//PI / 180.0f;		//! 32bit Constant for converting from degrees to radians
const float RADTODEG			= 57.295779513082320876798154814105f;//180.0f / PI;		//! 32bit constant for converting from radians to degrees
const float ONE_THIRD			= 0.3333333333333333333333333333333f;					//! Constant for 1/3.

//! returns minimum of two values. Own implementation to get rid of the STL (VS6 problems)
inline float ENGINE_PUBLIC_EXPORT min(const float a, const float b);

//! returns maximum of two values. Own implementation to get rid of the STL (VS6 problems)
inline float ENGINE_PUBLIC_EXPORT max(const float a, const float b);

//! returns minimum of three values.
inline float ENGINE_PUBLIC_EXPORT min(const float a, const float b, const float c);

//! returns maximum of three values.
inline float ENGINE_PUBLIC_EXPORT max(const float a, const float b, const float c);

//! returns if a value equals the other one, taking
//! rounding errors into account
inline bool ENGINE_PUBLIC_EXPORT equals(const float a, const float b);

//! returns abs of two values. Own implementation to get rid of STL (VS6 problems)
inline float ENGINE_PUBLIC_EXPORT abs(float val);

inline float ENGINE_PUBLIC_EXPORT acos(float val);

inline float ENGINE_PUBLIC_EXPORT asin(float val);

inline float ENGINE_PUBLIC_EXPORT atan(float val);

inline float ENGINE_PUBLIC_EXPORT atan2(float valy, float valx);

inline float ENGINE_PUBLIC_EXPORT cos(float val);

inline float ENGINE_PUBLIC_EXPORT sin(float val);

inline float ENGINE_PUBLIC_EXPORT tan(float val);

inline float ENGINE_PUBLIC_EXPORT ceil(float val);

inline float ENGINE_PUBLIC_EXPORT floor(float val);

inline float ENGINE_PUBLIC_EXPORT clamp(float val, float min, float max);

inline float ENGINE_PUBLIC_EXPORT exp(float val);

inline float ENGINE_PUBLIC_EXPORT log(float val);

inline float ENGINE_PUBLIC_EXPORT sqr(float val);

inline float ENGINE_PUBLIC_EXPORT sqrt(float val);

} // end namespace core

#endif