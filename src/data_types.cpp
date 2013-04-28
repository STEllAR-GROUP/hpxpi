//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpxpi/xpi.h>

#if defined(__cplusplus)
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Intrinsic types [4.2.1]
XPI_Type XPI_VOID = { 0 };               // void
XPI_Type XPI_ERROR = { 0 };              // XPI_Err
XPI_Type XPI_BOOL = { 0 };               // bool
XPI_Type XPI_CHAR = { 0 };               // char
XPI_Type XPI_SHORT = { 0 };              // short
XPI_Type XPI_INT = { 0 };                // int
XPI_Type XPI_LONG = { 0 };               // long
XPI_Type XPI_LONG_LONG = { 0 };          // long long
XPI_Type XPI_UNSIGNED_CHAR = { 0 };      // unsigned char
XPI_Type XPI_UNSIGNED_INT = { 0 };       // unsigned int
XPI_Type XPI_UNSIGNED_SHORT = { 0 };     // unsigned short
XPI_Type XPI_UNSIGNED_LONG = { 0 };      // unsigned long
XPI_Type XPI_UNSIGNED_LONG_LONG = { 0 }; // unsigned long long
XPI_Type XPI_FLOAT = { 0 };              // float
XPI_Type XPI_DOUBLE = { 0 };             // double
XPI_Type XPI_LONG_DOUBLE = { 0 };        // long double
XPI_Type XPI_FLOAT_COMPLEX = { 0 };      // Complex float
XPI_Type XPI_DOUBLE_COMPLEX = { 0 };     // Complex double
XPI_Type XPI_LONG_DOUBLE_COMPLEX = { 0 };    // Complex long double
XPI_Type XPI_UINT8_T = { 0 };            // uint8_t
XPI_Type XPI_UINT16_T = { 0 };           // uint16_t
XPI_Type XPI_UINT32_T = { 0 };           // uint32_t
XPI_Type XPI_UINT64_T = { 0 };           // uint64_t
XPI_Type XPI_INT8_T = { 0 };             // int8_t
XPI_Type XPI_INT16_T = { 0 };            // int16_t
XPI_Type XPI_INT32_T = { 0 };            // int32_t
XPI_Type XPI_INT64_T = { 0 };            // int64_t
XPI_Type XPI_SIZE_T = { 0 };             // size_t

#if defined(__cplusplus)
}
#endif

