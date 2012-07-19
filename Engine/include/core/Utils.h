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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <core/Config.h>

#include <string>
#include <vector>

namespace core
{

inline ENGINE_PUBLIC_EXPORT void stringTrim(std::string& str, const std::string& whitespace = " \t\n", bool left = true, bool right = true);
inline ENGINE_PUBLIC_EXPORT void stringToLower(std::string& str);
inline ENGINE_PUBLIC_EXPORT void stringToUpper(std::string& str);

inline ENGINE_PUBLIC_EXPORT signed int stringToInt(std::string& str);
inline ENGINE_PUBLIC_EXPORT float stringToFloat(std::string& str);

inline ENGINE_PUBLIC_EXPORT std::string intToString(unsigned int i);
inline ENGINE_PUBLIC_EXPORT std::string intToString(signed int i);
inline ENGINE_PUBLIC_EXPORT std::string floatToString(float f);

inline ENGINE_PUBLIC_EXPORT std::vector<std::string> splitString(const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);

inline ENGINE_PUBLIC_EXPORT bool powerOfTwo(unsigned int num);

//! Convert N bit color channel value to P bits. It fills P bits with the bit pattern repeated.
inline ENGINE_PUBLIC_EXPORT unsigned int fixedToFixed(unsigned int value, unsigned int n, unsigned int p);
//! Convert floating point color channel value between 0.0 and 1.0 (otherwise clamped) to integer of a certain number of bits. Works for any value of bits between 0 and 31.
inline ENGINE_PUBLIC_EXPORT unsigned int floatToFixed(const float value, const unsigned int bits);
//! Fixed point to float.
inline ENGINE_PUBLIC_EXPORT float fixedToFloat(unsigned int value, unsigned int bits);
//! Convert a float32 to a float16 (NV_half_float).Courtesy of OpenEXR.
inline ENGINE_PUBLIC_EXPORT unsigned short int floatToHalf(float i);
//! Convert a float16 (NV_half_float) to a float32.Courtesy of OpenEXR.
inline ENGINE_PUBLIC_EXPORT float halfToFloat(unsigned short int y);

static const std::string STRING_BLANK = "";

} // end namespace core

#endif