/*
-----------------------------------------------------------------------------
KG game engine (http://katoun.github.com/kg_engine) is made available under the MIT License.

Copyright (c) 2006-2013 Catalin Alexandru Nastase

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
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

inline ENGINE_PUBLIC_EXPORT int stringToInt(std::string& str);
inline ENGINE_PUBLIC_EXPORT float stringToFloat(std::string& str);

inline ENGINE_PUBLIC_EXPORT std::string intToString(unsigned int i);
inline ENGINE_PUBLIC_EXPORT std::string intToString(int i);
inline ENGINE_PUBLIC_EXPORT std::string floatToString(float f);

inline ENGINE_PUBLIC_EXPORT void stringReplaceChar(std::string& str, const char src, const char dest);

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

} // end namespace core

#endif